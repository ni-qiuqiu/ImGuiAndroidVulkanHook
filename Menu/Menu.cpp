#include <pthread.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <android/log.h>
#include <android/native_window.h>
#include <android/input.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_android.h>

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/imgui_impl_vulkan.h"

#include "../Dobby/dobby.h"
#include <mutex>
#include <dlfcn.h>

#define LOG_TAG "VulkanMenu"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define MAX_FRAMES_IN_FLIGHT 2

std::unordered_map<std::string, PFN_vkVoidFunction> vkFunctionMap;


ANativeWindow *g_NativeWindow = nullptr;            
VkInstance g_Instance = VK_NULL_HANDLE;             
VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE; 
VkDevice g_Device = VK_NULL_HANDLE;                 
VkQueue g_GraphicsQueue = VK_NULL_HANDLE;           
uint32_t g_QueueFamilyIndex = UINT32_MAX;           
VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE; 
VkCommandPool g_CommandPool = VK_NULL_HANDLE;       
VkRenderPass g_RenderPass = VK_NULL_HANDLE;         
VkSurfaceKHR g_Surface = VK_NULL_HANDLE;            
VkSwapchainKHR g_Swapchain = VK_NULL_HANDLE;        
VkExtent2D g_SwapChainExtent = {0, 0};              
VkFormat g_SwapChainFormat = VK_FORMAT_UNDEFINED;   
std::vector<VkImage> g_SwapChainImages;             
std::vector<VkImageView> g_SwapChainImageViews;     
std::vector<VkFramebuffer> g_Framebuffers;          

VkQueue g_QueuePresent = VK_NULL_HANDLE; 


struct FrameData
{
    VkCommandBuffer commandBuffer;       
    VkSemaphore imageAvailableSemaphore; 
    VkSemaphore renderFinishedSemaphore; 
    VkFence inFlightFence;               
    bool used;                           
};
std::vector<FrameData> g_FrameData; 
uint32_t g_CurrentFrame = 0;        


bool g_ImGuiInitialized = false; 
bool g_InitInProgress = false;   
bool g_MenuVisible = true;      
bool g_InSubmit = false;         




VkResult (*original_vkQueueSubmit)(VkQueue queue, uint32_t submitCount, const VkSubmitInfo *pSubmits, VkFence fence);
VkResult (*original_vkCreateInstance)(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkInstance *pInstance);
VkResult (*original_vkCreateDevice)(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDevice *pDevice);
VkResult (*original_vkCreateSwapchainKHR)(VkDevice device, const VkSwapchainCreateInfoKHR *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkSwapchainKHR *pSwapchain);
VkResult (*original_vkCreateAndroidSurfaceKHR)(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkSurfaceKHR *pSurface);
VkResult (*original_vkQueuePresentKHR)(VkQueue queue, const VkPresentInfoKHR* pPresentInfo);

PFN_vkVoidFunction (*original_vkGetInstanceProcAddr)(VkInstance instance, const char *pName);


uint32_t findGraphicsQueueFamily()
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            return i;
        }
    }
    return UINT32_MAX;
}


bool createCommandPool()
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = g_QueueFamilyIndex;

    if (vkCreateCommandPool(g_Device, &poolInfo, nullptr, &g_CommandPool) != VK_SUCCESS)
    {
        LOGE("Failed to create command pool");
        return false;
    }

    LOGD("Command pool created successfully");
    return true;
}

VkCommandBuffer createCommandBuffer()
{
    if (g_Device == VK_NULL_HANDLE || g_CommandPool == VK_NULL_HANDLE)
    {
        LOGD("Device or CommandPool is null");
        return VK_NULL_HANDLE;
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = g_CommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    VkResult result = vkAllocateCommandBuffers(g_Device, &allocInfo, &commandBuffer);
    if (result == VK_SUCCESS)
    {
        LOGD("Command buffer created successfully");
    }
    else
    {
        LOGD("Failed to create command buffer: %d", result);
        return VK_NULL_HANDLE;
    }
    
    return commandBuffer;
}




bool createSyncObjects()
{
    g_FrameData.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = g_CommandPool;
    allocInfo.commandBufferCount = 1;

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; 

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkAllocateCommandBuffers(g_Device, &allocInfo, &g_FrameData[i].commandBuffer) != VK_SUCCESS ||
            vkCreateSemaphore(g_Device, &semaphoreInfo, nullptr, &g_FrameData[i].imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(g_Device, &semaphoreInfo, nullptr, &g_FrameData[i].renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(g_Device, &fenceInfo, nullptr, &g_FrameData[i].inFlightFence) != VK_SUCCESS)
        {
            return false;
        }
        g_FrameData[i].used = false;
    }

    return true;
}


bool createDescriptorPool()
{
    VkDescriptorPoolSize pool_sizes[] = {
        {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
    };

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
    poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    poolInfo.pPoolSizes = pool_sizes;

    if (vkCreateDescriptorPool(g_Device, &poolInfo, nullptr, &g_DescriptorPool) != VK_SUCCESS)
    {
        LOGE("Failed to create descriptor pool");
        return false;
    }

    LOGD("Descriptor pool created successfully");
    return true;
}


VkRenderPass createRenderPass()
{
    
    VkAttachmentDescription attachment{};
    attachment.format = g_SwapChainFormat;
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;    // Important: LOAD to preserve Unity rendering
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //  STORE to preserve Unity rendering
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // present layout
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;   // to present layout

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &attachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VkRenderPass renderPass;
    VkResult result = vkCreateRenderPass(g_Device, &renderPassInfo, nullptr, &renderPass);
    if (result != VK_SUCCESS)
    {
        LOGE("Failed to create render pass: %d", result);
        return VK_NULL_HANDLE;
    }

    LOGD("Render pass created successfully");
    return renderPass;
}


bool createFramebuffers()
{
    g_SwapChainImageViews.resize(g_SwapChainImages.size());
    g_Framebuffers.resize(g_SwapChainImages.size());

    for (size_t i = 0; i < g_SwapChainImages.size(); i++)
    {
        
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = g_SwapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = g_SwapChainFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(g_Device, &createInfo, nullptr, &g_SwapChainImageViews[i]) != VK_SUCCESS)
        {
            return false;
        }

        
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = g_RenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &g_SwapChainImageViews[i];
        framebufferInfo.width = g_SwapChainExtent.width;
        framebufferInfo.height = g_SwapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(g_Device, &framebufferInfo, nullptr, &g_Framebuffers[i]) != VK_SUCCESS)
        {
            return false;
        }
    }

    return true;
}


void imageLayoutTransition(VkCommandBuffer& commandBuffer, 
                VkImage& image, VkImageLayout oldLayout, VkImageLayout newLayout,
                bool bATTACHMENT) {
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    VkPipelineStageFlags srcStage, dstStage;
    
    if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR && 
        newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        srcStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        dstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && 
               newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        srcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dstStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    } else {
       
        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        srcStage = bATTACHMENT ? VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT : 
                                 VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        dstStage = bATTACHMENT ? VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT : 
                                 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        srcStage, dstStage,
        0, 0, nullptr, 0, nullptr, 1, &barrier
    );
}


bool initializeImGui()
{
    try
    {
      
        if (!createCommandPool() || !createDescriptorPool() || !createSyncObjects())
            return false;

      
        g_RenderPass = createRenderPass();
        if (g_RenderPass == VK_NULL_HANDLE)
            return false;

        
        if (!createFramebuffers())
            return false;

       
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        
        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        
        
        int width = ANativeWindow_getWidth(g_NativeWindow); 
        int height = ANativeWindow_getHeight(g_NativeWindow); 
        LOGD("width: %d, height: %d", width, height);
        io.DisplaySize = ImVec2((float)width, (float)height);
        
        
        ImGui::StyleColorsDark();
        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowBorderSize = 1.0f;
        style.WindowRounding = 0.0f;
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.AntiAliasedLines = true;
        style.AntiAliasedFill = true;

        
        if (!ImGui_ImplAndroid_Init(g_NativeWindow))
        {
            LOGE("ImGui_ImplAndroid_Init failed");
            return false;
        }
     
        ImGui_ImplVulkan_InitInfo init_info{};
        init_info.Instance = g_Instance;
        init_info.PhysicalDevice = g_PhysicalDevice;
        init_info.Device = g_Device;
        init_info.QueueFamily = g_QueueFamilyIndex;
        init_info.Queue = g_QueuePresent;
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = g_DescriptorPool;
        init_info.RenderPass = g_RenderPass;
        init_info.MinImageCount = g_SwapChainImages.size();
        init_info.ImageCount = g_SwapChainImages.size();
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;

        if (!ImGui_ImplVulkan_Init(&init_info))
        {
            LOGE("ImGui_ImplVulkan_Init failed");
            return false;
        }

        return true;
    }
    catch (...)
    {
        LOGE("ImGui initialization exception");
        return false;
    }
}



void drawMenu()
{
    if (!g_MenuVisible)
        return;

   
    static bool firstTime = true;
    if (firstTime)
    {
        ImGuiIO &io = ImGui::GetIO();
        float width = io.DisplaySize.x;
        float height = io.DisplaySize.y;
        LOGD("display size: %f, %f", io.DisplaySize.x, io.DisplaySize.y);
        
       
        float menuWidth = width * 0.3f;  
        float menuHeight = height * 0.6f; 
        float posX = width * 0.05f;  
        float posY = height * 0.1f;  
        
        ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight), ImGuiCond_FirstUseEver);
        
        firstTime = false;
    }

    
    ImGui::Begin("inject menu", &g_MenuVisible, ImGuiWindowFlags_NoCollapse);

   
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 1.0f, 1.0f));
    ImGui::Text("inject menu");
    ImGui::Separator();
    ImGui::PopStyleColor();

    
    ImGui::Text("window size: %dx%d", g_SwapChainExtent.width, g_SwapChainExtent.height);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

   
    static bool toggleOption1 = false;
    static bool toggleOption2 = false;
    static float sliderValue = 50.0f;
    static int radioSelection = 0;
    static char inputText[128] = "hello imgui";

    ImGui::Checkbox("checkbox", &toggleOption1);
    ImGui::Checkbox("checkbox2", &toggleOption2);
    ImGui::SliderFloat("slider", &sliderValue, 0.0f, 100.0f, "%.1f");

    ImGui::ShowDemoWindow(&g_MenuVisible);

    ImGui::Spacing();
    ImGui::Text("select one:");
    ImGui::RadioButton("selectA", &radioSelection, 0); ImGui::SameLine();
    ImGui::RadioButton("selectB", &radioSelection, 1); ImGui::SameLine();
    ImGui::RadioButton("selectC", &radioSelection, 2);


    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

   
    if (ImGui::Button("test button", ImVec2(-1, 40)))
    {
        LOGD("button clicked");
    }

    ImGui::End();
}


bool renderImGui(uint32_t imageIndex)
{
    try
    {
        FrameData &currentFrame = g_FrameData[g_CurrentFrame];

        // wait for fence to be signaled
        vkWaitForFences(g_Device, 1, &currentFrame.inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetCommandBuffer(currentFrame.commandBuffer, 0);
        vkResetFences(g_Device, 1, &currentFrame.inFlightFence);

        // begin command buffer recording
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        if (vkBeginCommandBuffer(currentFrame.commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            return false;
        }

        // imageLayoutTransition : PRESENT_SRC -> COLOR_ATTACHMENT
        imageLayoutTransition(
            currentFrame.commandBuffer, 
            g_SwapChainImages[imageIndex],
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            false
        );

        // BeginRenderPass
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = g_RenderPass;
        renderPassInfo.framebuffer = g_Framebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = g_SwapChainExtent;

        vkCmdBeginRenderPass(currentFrame.commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
        // RenderDrawData ImGui
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), currentFrame.commandBuffer);
        
        vkCmdEndRenderPass(currentFrame.commandBuffer);

        // imageLayoutTransition : COLOR_ATTACHMENT -> PRESENT_SRC
        imageLayoutTransition(
            currentFrame.commandBuffer, 
            g_SwapChainImages[imageIndex],
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            true
        );

        // end command buffer recording
        if (vkEndCommandBuffer(currentFrame.commandBuffer) != VK_SUCCESS)
        {
            return false;
        }

        // submit command buffer to queue
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &currentFrame.commandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &currentFrame.renderFinishedSemaphore;

        if (vkQueueSubmit(g_QueuePresent, 1, &submitInfo, currentFrame.inFlightFence) != VK_SUCCESS)
        {
            return false;
        }

        // update current frame
        currentFrame.used = true;
        g_CurrentFrame = (g_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        
        return true;
    }
    catch (...)
    {
        LOGE("ImGui rendering exception");
        return false;
    }
}


void (*old_input)(void *event, void *exAb, void *exAc);
void hook_input(void *event, void *exAb, void *exAc)
{
    old_input(event, exAb, exAc);
    if (g_ImGuiInitialized && event)
    {
        AInputEvent* inputEvent = (AInputEvent*)event;
        
        ImGui_ImplAndroid_HandleInputEvent(inputEvent);
        
    }
    return;
}

// ------------------------------ HOOK IMPLEMENTATIONS ------------------------------
VkResult hooked_vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) {

if (g_Device == VK_NULL_HANDLE)
{
    //LOGD("vkQueuePresentKHR: vulkan device is null");
    return original_vkQueuePresentKHR(queue, pPresentInfo);
}


    
    if (!g_ImGuiInitialized && g_NativeWindow ) {
        initializeImGui();
        g_ImGuiInitialized = true;
        g_MenuVisible = true;
        LOGD("ImGui initialized");
    }

    try {
        if (pPresentInfo && pPresentInfo->swapchainCount > 0) {
            uint32_t imageIndex = pPresentInfo->pImageIndices[0];
            
           
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplAndroid_NewFrame();
            ImGui::NewFrame();

           
            drawMenu();
            
           
            ImGui::EndFrame();
            ImGui::Render();
            renderImGui(imageIndex);
        }
    }
    catch (const std::exception &e) {
        LOGE("vkQueuePresentKHR exception: %s", e.what());
    }

    
    return original_vkQueuePresentKHR(queue, pPresentInfo);
}


VkResult hooked_vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo *pSubmits, VkFence fence) {
    
    g_QueuePresent = queue;
    
   
    return original_vkQueueSubmit(queue, submitCount, pSubmits, fence);
}


VkResult hooked_vkCreateInstance(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkInstance *pInstance) {
    VkResult result = original_vkCreateInstance(pCreateInfo, pAllocator, pInstance);
    
    if (result == VK_SUCCESS) {
        g_Instance = *pInstance;
        LOGD("capture vulkan instance: %p", g_Instance);
    }
    
    return result;
}


VkResult hooked_vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDevice *pDevice) {
    VkResult result = original_vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
    
    if (result == VK_SUCCESS) {
        g_PhysicalDevice = physicalDevice;
        g_Device = *pDevice;
        
       
        g_QueueFamilyIndex = findGraphicsQueueFamily();
        if (g_QueueFamilyIndex != UINT32_MAX) {
            vkGetDeviceQueue(g_Device, g_QueueFamilyIndex, 0, &g_GraphicsQueue);
            createCommandPool();
        }
        
        LOGD("capture vulkan device: %p", g_Device);
    }
    
    return result;
}


VkResult hooked_vkCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkSwapchainKHR *pSwapchain) {
    VkResult result = original_vkCreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
    
    if (result == VK_SUCCESS) {
      


       
        g_Swapchain = *pSwapchain;
        g_SwapChainExtent = pCreateInfo->imageExtent;
        g_Surface = pCreateInfo->surface;
        g_SwapChainFormat = pCreateInfo->imageFormat;
        
        
        uint32_t imageCount;
        vkGetSwapchainImagesKHR(device, g_Swapchain, &imageCount, nullptr);
        g_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, g_Swapchain, &imageCount, g_SwapChainImages.data());
        
        LOGD("capture vulkan swapchain: %p, size: %dx%d", 
             g_Swapchain, g_SwapChainExtent.width, g_SwapChainExtent.height);
    }
    
    return result;
}


VkResult hooked_vkCreateAndroidSurfaceKHR(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkSurfaceKHR *pSurface) {
   
   VkResult result = original_vkCreateAndroidSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
   
    if (pCreateInfo && pCreateInfo->window) {
        g_NativeWindow = pCreateInfo->window;
        LOGD("capture android native window: %p", g_NativeWindow);
    }
    
    return result;
}


PFN_vkVoidFunction hooked_vkGetInstanceProcAddr(VkInstance instance, const char *pName) {
    if (instance != VK_NULL_HANDLE && g_Instance == VK_NULL_HANDLE) {
        g_Instance = instance;
    }
    
    PFN_vkVoidFunction result = original_vkGetInstanceProcAddr(instance, pName);
    vkFunctionMap[pName] = result;
    return result;
}


//TODO: fix touch
void (*initializeMotionEventOrigin)(void* thiz, void* event, void* msg);
void initializeMotionEventReplace(void *thiz, void *event, void *msg) {
    
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    
    initializeMotionEventOrigin(thiz, event, msg);
}


bool isLibraryLoaded(const char *libraryName) {
    char line[512];
    FILE *fp = fopen("/proc/self/maps", "rt");
    if (!fp) return false;
    
    bool found = false;
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, libraryName)) {
            found = true;
            break;
        }
    }
    
    fclose(fp);
    return found;
}


void setupVulkanHooks() {

//note: hooking time is very important
    while (!isLibraryLoaded("libvulkan.so")) {
        usleep(100 ); 
    }

    
    void *vkGetInstanceProcAddr_addr = DobbySymbolResolver("libvulkan.so", "vkGetInstanceProcAddr");
    if (vkGetInstanceProcAddr_addr) {
        DobbyHook(vkGetInstanceProcAddr_addr, (void *)hooked_vkGetInstanceProcAddr, (void **)&original_vkGetInstanceProcAddr);
        LOGD("Hook vkGetInstanceProcAddr success");
    }

   
    void *vkCreateInstance_addr = DobbySymbolResolver("libvulkan.so", "vkCreateInstance");
    if (vkCreateInstance_addr) {
        DobbyHook(vkCreateInstance_addr, (void *)hooked_vkCreateInstance, (void **)&original_vkCreateInstance);
        LOGD("Hook vkCreateInstance success");
    }

   
    bool allHooked = false;
    while (!allHooked) {
        static bool createDeviceHooked = false;
        static bool createSurfaceHooked = false;
        static bool createSwapchainHooked = false;
        static bool queueSubmitHooked = false;
        static bool queuePresentHooked = false;

        
        if (vkFunctionMap["vkCreateDevice"] && !createDeviceHooked) {
            DobbyHook((void *)vkFunctionMap["vkCreateDevice"], (void *)hooked_vkCreateDevice, (void **)&original_vkCreateDevice);
            createDeviceHooked = true;
        }

        if (vkFunctionMap["vkCreateAndroidSurfaceKHR"] && !createSurfaceHooked) {
            DobbyHook((void *)vkFunctionMap["vkCreateAndroidSurfaceKHR"], (void *)hooked_vkCreateAndroidSurfaceKHR, (void **)&original_vkCreateAndroidSurfaceKHR);
            createSurfaceHooked = true;
        }

        if (vkFunctionMap["vkCreateSwapchainKHR"] && !createSwapchainHooked) {
            DobbyHook((void *)vkFunctionMap["vkCreateSwapchainKHR"], (void *)hooked_vkCreateSwapchainKHR, (void **)&original_vkCreateSwapchainKHR);
            createSwapchainHooked = true;
        }

        if (vkFunctionMap["vkQueueSubmit"] && !queueSubmitHooked) {
            DobbyHook((void *)vkFunctionMap["vkQueueSubmit"], (void *)hooked_vkQueueSubmit, (void **)&original_vkQueueSubmit);
            queueSubmitHooked = true;
        }

        if (vkFunctionMap["vkQueuePresentKHR"] && !queuePresentHooked) {
            DobbyHook((void *)vkFunctionMap["vkQueuePresentKHR"], (void *)hooked_vkQueuePresentKHR, (void **)&original_vkQueuePresentKHR);
            queuePresentHooked = true;
        }

        allHooked = createDeviceHooked && createSurfaceHooked && createSwapchainHooked && 
                   queueSubmitHooked && queuePresentHooked;
        
        if (!allHooked) {
            usleep( 100); 
        }
    }

    LOGD("all vulkan functions hooked");
}


void setupTouchHook() {
    // void *initializeMotionEventAddr = DobbySymbolResolver(
    //     "libinput.so", 
    //     "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");
    
    // if (initializeMotionEventAddr) {
    //     DobbyHook(initializeMotionEventAddr, 
    //              (void *)initializeMotionEventReplace, 
    //              (void **)&initializeMotionEventOrigin);
    //     LOGD("touch event hooked");
    // }

        void *sym_input = DobbySymbolResolver(("/system/lib/libinput.so"),
                                          ("_ZN7android11MotionEvent8copyFromEPKS0_b")); 
    DobbyHook(sym_input, (void *)hook_input, (void **)&old_input);
}


void initializeHooks() {
    setupVulkanHooks();
    setupTouchHook();
    LOGD("ImGui Vulkan Hook initialize success");
}


void *menuThread(void *) {
    initializeHooks();
    return NULL;
}


__attribute__((constructor)) void _init() {
    LOGD("ImGui Vulkan Hook start");
    
    pthread_t threadId;
    pthread_create(&threadId, NULL, menuThread, NULL);
}