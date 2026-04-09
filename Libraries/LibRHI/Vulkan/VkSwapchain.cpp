/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <algorithm>
#include <format>

#include <LibRHI/Vulkan/VkSwapchain.h>
#include <LibUI/Platform/Window.h>

namespace RHI {

auto VkSwapchain::create(Configuration const& config, VkDevice const* device) -> std::expected<std::unique_ptr<VkSwapchain>, std::string>
{
    std::unique_ptr<VkSwapchain> swapchain(new VkSwapchain);

    swapchain->m_device = device;
    swapchain->m_config = config;

    auto surface_format = swapchain->select_surface_format();
    auto present_mode = swapchain->select_present_mode();
    auto swap_extent = swapchain->select_swap_extent();
    auto image_count = swapchain->select_image_count();

    VkSwapchainCreateInfoKHR const swapchain_create_info {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = device->surface(),
        .minImageCount = image_count,
        .imageFormat = surface_format.format,
        .imageColorSpace = surface_format.colorSpace,
        .imageExtent = swap_extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = device->selected_physical_device()->surface_capabilities().currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    if (auto result = vkCreateSwapchainKHR(swapchain->m_device->handle(), &swapchain_create_info, nullptr, &swapchain->m_swapchain); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to create Vulkan swapchain: {}", string_VkResult(result)));
    }

    u32 actual_image_count = 0;
    if (auto result = vkGetSwapchainImagesKHR(swapchain->m_device->handle(), swapchain->m_swapchain, &actual_image_count, nullptr); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to retrieve Vulkan swapchain images: {}", string_VkResult(result)));
    }
    swapchain->m_images.resize(actual_image_count);
    if (auto result = vkGetSwapchainImagesKHR(swapchain->m_device->handle(), swapchain->m_swapchain, &actual_image_count, swapchain->m_images.data()); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to retrieve Vulkan swapchain images: {}", string_VkResult(result)));
    }

    swapchain->m_image_views.reserve(swapchain->m_images.size());
    for (auto const& image : swapchain->m_images) {
        VkImageViewCreateInfo const image_view_create_info {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = surface_format.format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY },
            .subresourceRange = { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1 }
        };

        VkImageView image_view {};
        if (auto result = vkCreateImageView(swapchain->m_device->handle(), &image_view_create_info, nullptr, &image_view); result != VK_SUCCESS) {
            return std::unexpected(std::format("Failed to create Vulkan swapchain image view: {}", string_VkResult(result)));
        }
        swapchain->m_image_views.push_back(image_view);
    }

    return swapchain;
}

VkSwapchain::~VkSwapchain()
{
    for (auto* image_view : m_image_views) {
        vkDestroyImageView(m_device->handle(), image_view, nullptr);
    }
    if (m_swapchain != nullptr) {
        vkDestroySwapchainKHR(m_device->handle(), m_swapchain, nullptr);
    }
}

void VkSwapchain::present()
{
}

auto VkSwapchain::config() const -> Configuration const&
{
    return m_config;
}

auto VkSwapchain::select_surface_format() const -> VkSurfaceFormatKHR
{
    auto const& surface_formats = m_device->selected_physical_device()->surface_formats();
    auto surface_format_if = std::ranges::find_if(surface_formats.begin(), surface_formats.end(), [](VkSurfaceFormatKHR const& surface_format) {
        return surface_format.format == VK_FORMAT_B8G8R8A8_SRGB && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    });
    return surface_format_if != surface_formats.end() ? *surface_format_if : surface_formats[0];
}

auto VkSwapchain::select_present_mode() const -> VkPresentModeKHR
{
    auto const& present_modes = m_device->selected_physical_device()->present_modes();
    auto present_mode_if = std::ranges::find(present_modes.begin(), present_modes.end(), VK_PRESENT_MODE_MAILBOX_KHR);
    return present_mode_if != present_modes.end() ? *present_mode_if : VK_PRESENT_MODE_FIFO_KHR;
}

auto VkSwapchain::select_swap_extent() const -> VkExtent2D
{
    auto const& surface_capabilities = m_device->selected_physical_device()->surface_capabilities();

    if (surface_capabilities.currentExtent.width != std::numeric_limits<u32>::max()) {
        return surface_capabilities.currentExtent;
    }

    auto width = std::clamp<u32>(m_config.width, surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
    auto height = std::clamp<u32>(m_config.height, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);
    return { width, height };
}

auto VkSwapchain::select_image_count() const -> u32
{
    auto const& surface_capabilities = m_device->selected_physical_device()->surface_capabilities();
    auto image_count = surface_capabilities.minImageCount + 1;
    if (surface_capabilities.maxImageCount > 0 && image_count > surface_capabilities.maxImageCount) {
        image_count = surface_capabilities.maxImageCount;
    }
    return image_count;
}

}
