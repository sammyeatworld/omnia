/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "VkDevice.h"
#include "VkBuffer.h"
#include "VkShader.h"
#include "VkSwapchain.h"
#include "VkTexture.h"
#ifdef OA_OS_WINDOWS
#    include <LibUI/Platform/Win32/WindowWin32.h>
#endif

#include <format>
#include <print>
#include <vector>

namespace RHI {

static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT message_type, VkDebugUtilsMessengerCallbackDataEXT const* callback_data, [[maybe_unused]] void* user_data)
{
    if (severity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        return VK_FALSE;

    switch (severity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        std::println(stderr, "Vulkan Debug: {}", callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
        break;
    }

    return VK_FALSE;
}

auto VkDevice::create(Configuration const& config) -> std::expected<std::unique_ptr<VkDevice>, std::string>
{
    std::unique_ptr<VkDevice> device(new VkDevice);

    VkApplicationInfo const app_info {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "Omnia App",
        .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
        .pEngineName = "Omnia Engine",
        .engineVersion = VK_MAKE_VERSION(0, 1, 0),
        .apiVersion = VK_API_VERSION_1_3,
    };

    std::vector<char const*> required_extensions {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_PLATFORM_SURFACE_EXTENSION_NAME,
    };

    if (config.enable_debug_layer) {
        required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    VkInstanceCreateInfo instance_info {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<u32>(required_extensions.size()),
        .ppEnabledExtensionNames = required_extensions.data(),
    };

    if (config.enable_debug_layer) {
        char const* instance_layers[] {
            "VK_LAYER_KHRONOS_validation"
        };

        u32 layer_count = 0;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        std::vector<VkLayerProperties> available_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

        bool validation_layer_found = false;
        for (auto const& layerProperty : available_layers) {
            if (strcmp(layerProperty.layerName, instance_layers[0]) == 0) {
                validation_layer_found = true;
                break;
            }
        }
        if (!validation_layer_found) {
            return std::unexpected("Vulkan validation layer requested but not available.");
        }

        instance_info.enabledLayerCount = 1;
        instance_info.ppEnabledLayerNames = instance_layers;
    }

    if (auto result = vkCreateInstance(&instance_info, nullptr, &device->m_instance); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to create Vulkan instance: {}", string_VkResult(result)));
    }

    if (config.enable_debug_layer) {
        VkDebugUtilsMessengerCreateInfoEXT const debugMessengerCreateInfo {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = nullptr,
            .flags = 0,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = vk_debug_callback,
            .pUserData = nullptr
        };

        auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(device->m_instance, "vkCreateDebugUtilsMessengerEXT"));
        if (auto result = func(device->m_instance, &debugMessengerCreateInfo, nullptr, &device->m_debug_messenger); result != VK_SUCCESS) {
            return std::unexpected(std::format("Failed to create Vulkan debug messenger: {}", string_VkResult(result)));
        }
    }

#ifdef OA_OS_WINDOWS
    auto const* window = static_cast<UI::WindowWin32 const*>(config.window);

    VkWin32SurfaceCreateInfoKHR const surface_info {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .hinstance = window->instance(),
        .hwnd = window->handle()
    };

    if (auto result = vkCreateWin32SurfaceKHR(device->m_instance, &surface_info, nullptr, &device->m_surface); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to create Vulkan surface: {}", string_VkResult(result)));
    }
#endif

    return device;
}

VkDevice::~VkDevice()
{
    if (m_surface != nullptr) {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    }
    if (m_debug_messenger != nullptr) {
        auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
        func(m_instance, m_debug_messenger, nullptr);
    }
    if (m_instance != nullptr) {
        vkDestroyInstance(m_instance, nullptr);
    }
}

auto VkDevice::create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string>
{
    return VkBuffer::create(config);
}

auto VkDevice::create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string>
{
    return VkShader::create(config);
}

auto VkDevice::create_swapchain(Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string>
{
    return VkSwapchain::create(config);
}

auto VkDevice::create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string>
{
    return VkTexture::create(config);
}

}
