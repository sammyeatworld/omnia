/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibRHI/Vulkan/VkPhysicalDevice.h>

namespace RHI {

VkPhysicalDevice::VkPhysicalDevice(::VkPhysicalDevice handle, VkSurfaceKHR surface)
    : m_handle(handle)
    , m_surface(surface)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(m_handle, &properties);
    m_name = properties.deviceName;

    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(handle, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(handle, &queue_family_count, queue_families.data());

    for (u32 i = 0; i < queue_family_count; i++) {
        if ((queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0U) {
            m_queue_family_indices.graphics = i;
        } else if ((queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0U) {
            m_queue_family_indices.transfer = i;
        }

        VkBool32 present_support = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(handle, i, m_surface, &present_support);
        if (present_support != VK_FALSE) {
            m_queue_family_indices.present = i;
        }
    }

    u32 surface_format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(handle, m_surface, &surface_format_count, nullptr);
    m_surface_formats.resize(surface_format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(handle, m_surface, &surface_format_count, m_surface_formats.data());

    u32 present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(handle, m_surface, &present_mode_count, nullptr);
    m_present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(handle, m_surface, &present_mode_count, m_present_modes.data());
}

auto VkPhysicalDevice::is_suitable() const -> bool
{
    if (m_queue_family_indices.graphics == -1U || m_queue_family_indices.present == -1U) {
        return false;
    }

    if (m_surface_formats.empty() || m_present_modes.empty()) {
        return false;
    }

    std::vector<char const*> const required_extensions {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    u32 extension_count = 0;
    vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extension_count, available_extensions.data());

    for (auto const* required_extension : required_extensions) {
        bool extension_found = false;
        for (auto const& extension : available_extensions) {
            if (strcmp(extension.extensionName, required_extension) == 0) {
                extension_found = true;
                break;
            }
        }
        if (!extension_found) {
            return false;
        }
    }

    return true;
}

auto VkPhysicalDevice::handle() const -> ::VkPhysicalDevice
{
    return m_handle;
}

auto VkPhysicalDevice::name() const -> std::string const&
{
    return m_name;
}

auto VkPhysicalDevice::queue_family_indices() const -> QueueFamilyIndices const&
{
    return m_queue_family_indices;
}

auto VkPhysicalDevice::surface_formats() const -> std::vector<VkSurfaceFormatKHR> const&
{
    return m_surface_formats;
}

auto VkPhysicalDevice::present_modes() const -> std::vector<VkPresentModeKHR> const&
{
    return m_present_modes;
}

auto VkPhysicalDevice::surface_capabilities() const -> VkSurfaceCapabilitiesKHR
{
    VkSurfaceCapabilitiesKHR surface_capabilities {};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_handle, m_surface, &surface_capabilities);
    return surface_capabilities;
}

}
