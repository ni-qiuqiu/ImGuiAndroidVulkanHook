cmake_minimum_required(VERSION 3.18.1)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wl,--exclude-libs,ALL")





set(CMAKE_SYSTEM_NAME ANDROID) 
set(CMAKE_ANDROID_NDK D:/ndkollvm/android-ndk-r25c.Ollvm/android-ndk-r25c)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake)

set(CMAKE_ANDROID_ARCH_ABI arm64-v8a)
set(ANDROID_ABI arm64-v8a)
set(ANDROID_PLATFORM android-26)
set(ANDROID_NDK ${CMAKE_ANDROID_NDK})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY   
    ${CMAKE_SOURCE_DIR}/outputs/${CMAKE_ANDROID_ARCH_ABI}/
) 

set(CMAKE_CXX_STANDARD 17) 


#------------------- BEGIN Library (Menu)  ------------------------#
project("Menu")
add_library(

        # Sets the name of the library ---> libMenu.so
        Menu
        SHARED

        # Menu
        Menu/Menu.cpp

        # ImGui
        Menu/ImGui/imconfig.h
        Menu/ImGui/imgui.cpp
        Menu/ImGui/imgui.h
        Menu/ImGui/imgui_demo.cpp
        Menu/ImGui/imgui_draw.cpp
        Menu/ImGui/imgui_tables.cpp
        Menu/ImGui/imgui_widgets.cpp
        Menu/ImGui/imstb_rectpack.h
        Menu/ImGui/imstb_textedit.h
        Menu/ImGui/imstb_truetype.h

        Menu/ImGui/backends/imgui_impl_android.cpp
        Menu/ImGui/backends/imgui_impl_android.h
        Menu/ImGui/backends/imgui_impl_vulkan.cpp
        Menu/ImGui/backends/imgui_impl_vulkan.h
)


# Define and configure the Dobby library as an imported static library
add_library(dobby STATIC IMPORTED)

# Set the path to the Dobby library based on the current ABI (ANDROID_ABI)
set_target_properties(dobby PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/Dobby/${ANDROID_ABI}/libdobby.a)

# Link the Menu library with the Dobby static library and log library.
target_link_libraries(Menu PRIVATE dobby log android z vulkan)

#------------------- END Library (Menu)  ------------------------#