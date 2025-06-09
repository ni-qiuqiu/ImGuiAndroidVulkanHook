# WIP : ImGui Android Vulkan Hook

This project demonstrates an overlay implementation using ImGui with Vulkan for Android applications. It includes function hooks to inject a custom graphical menu and intercept touch events. Designed for educational and experimental purposes only.

## TODO

> This project adapt to ue4(which use vulkan)

- [ ]  Fix touch event handling for mod menu.
- [ ]  Fix screen rotation for imgui.
- [ ]  Fix a series of problems caused by using `Home`.

## Test

![images](./test/image.png)

## Features

- Vulkan-based rendering for ImGui overlays.
- Hooking Vulkan functions (`vkQueueSubmit`, `vkCreateSwapchainKHR`,`vkQueuePresentKHR`) to integrate ImGui.
- Customizable mod menu example with touch event handling.
- Android Native Window support.

## Requirements

- **Android NDK** for building native libraries.
- Vulkan-capable Android device.
- External libraries:
  - [ImGui v1.95.x](https://github.com/ocornut/imgui)
  - [Dobby Hooking Library](https://github.com/jmpews/Dobby)
  - Vulkan SDK
