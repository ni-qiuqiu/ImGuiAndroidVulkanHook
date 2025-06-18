#pragma once
#include <android/log.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unwind.h>    
#include <dlfcn.h>     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_TAG "VulkanMenu"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define MAX_STACK_FRAMES 64


static int sHandledSignals[] = {
    SIGSEGV  
    // SIGABRT,  
    // SIGBUS    
    // SIGPIPE
};


struct sigaction sOldHandlers[NSIG];


typedef struct {
    void** frames;
    size_t frame_count;
    size_t max_depth;
} backtrace_state_t;


static _Unwind_Reason_Code unwind_callback(struct _Unwind_Context* context, void* arg) {
    backtrace_state_t* state = (backtrace_state_t*)arg;
    
    if (state->frame_count < state->max_depth) {
        void* pc = (void*)_Unwind_GetIP(context);
        if (pc) {
            state->frames[state->frame_count++] = pc;
        }
    }
    
    return (state->frame_count >= state->max_depth) ? _URC_END_OF_STACK : _URC_NO_REASON;
}


static void get_timestamp(char* buffer, size_t size) {
    time_t rawtime;
    struct tm* timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}


static void capture_stack_trace() {
    void* frames[MAX_STACK_FRAMES];
    backtrace_state_t state = {frames, 0, MAX_STACK_FRAMES};
    
  
    _Unwind_Backtrace(unwind_callback, &state);
    
    LOGD("\ncapture_stack_trace (%zu leveles):", state.frame_count);
    
  
    for (size_t i = 0; i < state.frame_count; i++) {
        Dl_info info;
        void* addr = frames[i];
        
    
        if (dladdr(addr, &info) && info.dli_sname) {
          
            LOGD("#%02zu: %p %s+%lx (%s)", 
                i, 
                addr, 
                info.dli_sname, 
                (char*)addr - (char*)info.dli_saddr,
                info.dli_fname ? info.dli_fname : "unknown");
        } else {
            
            LOGD("#%02zu: %p <unknown symbol>", i, addr);
        }
    }
}


static void signal_handler(int signo, siginfo_t* info, void* context) {
    
    static volatile sig_atomic_t handling_crash = 0;
    if (handling_crash) {
        _exit(1);  
    }
    handling_crash = 1;
    
   
    const char* sig_name;
    switch (signo) {
        case SIGSEGV: sig_name = "SIGSEGV"; break;
        case SIGABRT: sig_name = "SIGABRT"; break;
        case SIGFPE: sig_name = "SIGFPE"; break;
        case SIGILL: sig_name = "SIGILL"; break;
        case SIGBUS: sig_name = "SIGBUS"; break;
        default: sig_name = "unknown signal"; break;
    }
    LOGE("Caught fatal signal: %d (%s)", signo, sig_name);
    
    char timestamp[64];
    get_timestamp(timestamp, sizeof(timestamp));
    
    char header[512];
    snprintf(header, sizeof(header), 
             "==== crash report ====\n"
             "time: %s\n"
             "signal: %d (%s)\n"
             "pid: %d\n"
             "tid: %ld\n",
             timestamp, signo, sig_name, getpid(), (long)gettid());
    
    LOGD("%s", header);
    

    if (info) {
        char fault_addr[128];
        snprintf(fault_addr, sizeof(fault_addr), 
                 "fault address: %p\n"
                 "fault reason: %d\n",
                 info->si_addr, info->si_code);
        LOGD("%s", fault_addr);
        LOGE("fault address: %p, reason: %d", info->si_addr, info->si_code);
    }
    
    capture_stack_trace();
    
    abort();
   
}


void register_crash_handlers() {
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    
   
    action.sa_sigaction = signal_handler;
   
    action.sa_flags = SA_SIGINFO;
    
 
    size_t signals_count = sizeof(sHandledSignals) / sizeof(sHandledSignals[0]);
    for (size_t i = 0; i < signals_count; i++) {
        int signo = sHandledSignals[i];
        
       
        sigaction(signo, NULL, &sOldHandlers[signo]);
        
        
        if (sigaction(signo, &action, NULL) != 0) {
            LOGE("failed to register signal %d ", signo);
        } else {
            LOGD("successfully registered signal %d ", signo);
        }
    }
    
    LOGD("crash handlers registered");
}

