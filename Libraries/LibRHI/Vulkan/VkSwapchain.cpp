/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <algorithm>
#include <cassert>
#include <format>

#include <LibRHI/Vulkan/VkRenderTarget.h>
#include <LibRHI/Vulkan/VkSwapchain.h>
#include <LibUI/Platform/Window.h>

namespace RHI {

auto VkSwapchain::create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkSwapchain>, std::string>
{
    std::unique_ptr<VkSwapchain> swapchain(new VkSwapchain);

    swapchain->m_device = device;
    swapchain->m_config = config;

    return swapchain->create_swapchain()
        .and_then([&]() {
            return swapchain->create_images();
        })
        .and_then([&]() {
            return swapchain->create_command_buffers();
        })
        .and_then([&]() {
            return swapchain->create_sync_objects();
        })
        .transform([&]() {
            return std::move(swapchain);
        });
}

VkSwapchain::~VkSwapchain()
{
    for (auto* semaphore : m_image_available_semaphores) {
        vkDestroySemaphore(m_device->handle(), semaphore, nullptr);
    }
    for (auto* semaphore : m_render_finished_semaphores) {
        vkDestroySemaphore(m_device->handle(), semaphore, nullptr);
    }
    for (auto* fence : m_in_flight_fences) {
        vkDestroyFence(m_device->handle(), fence, nullptr);
    }
    if (m_handle != nullptr) {
        vkDestroySwapchainKHR(m_device->handle(), m_handle, nullptr);
    }
}

auto VkSwapchain::width() const -> u32
{
    return m_extent.width;
}

auto VkSwapchain::height() const -> u32
{
    return m_extent.height;
}

auto VkSwapchain::format() const -> Graphics::TextureFormat
{
    return to_graphics(m_surface_format.format);
}

auto VkSwapchain::textures() const -> std::vector<std::unique_ptr<Texture>> const&
{
    return m_textures;
}

auto VkSwapchain::is_dirty() const -> bool
{
    return m_is_dirty;
}

auto VkSwapchain::recreate(const RHI::Swapchain::Configuration& config) -> std::expected<void, std::string>
{
    assert(m_handle != VK_NULL_HANDLE);

    m_current_frame = 0;
    m_is_dirty = false;
    m_config = config;
    m_textures.clear();
    m_images.clear();

    vkDestroySwapchainKHR(m_device->handle(), m_handle, nullptr);
    return create_swapchain()
        .and_then([&]() {
            return create_images();
        });
}

void VkSwapchain::wait_idle() const
{
    for (auto* fence : m_in_flight_fences) {
        vkWaitForFences(m_device->handle(), 1, &fence, VK_TRUE, std::numeric_limits<u64>::max());
    }
}

auto VkSwapchain::begin_frame() -> std::optional<Frame>
{
    vkWaitForFences(m_device->handle(), 1, &m_in_flight_fences[m_current_frame], VK_TRUE, std::numeric_limits<u64>::max());
    u32 image_index = 0;
    if (auto result = vkAcquireNextImageKHR(m_device->handle(), m_handle, std::numeric_limits<u64>::max(), m_image_available_semaphores[m_current_frame], VK_NULL_HANDLE, &image_index); result == VK_ERROR_OUT_OF_DATE_KHR) {
        m_is_dirty = true;
        return std::nullopt;
    }
    vkResetFences(m_device->handle(), 1, &m_in_flight_fences[m_current_frame]);

    m_command_buffers[m_current_frame].reset();
    m_command_buffers[m_current_frame].begin();

    return Frame {
        .cmd = &m_command_buffers[m_current_frame],
        .image_index = image_index,
        .frame_index = m_current_frame
    };
}

void VkSwapchain::end_frame(Frame const& frame)
{
    auto* vk_cmd_handle = to_vk(frame.cmd)->handle();
    frame.cmd->end();

    VkPipelineStageFlags const wait_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo const submit_info {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_image_available_semaphores[m_current_frame],
        .pWaitDstStageMask = &wait_flags,
        .commandBufferCount = 1,
        .pCommandBuffers = &vk_cmd_handle,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &m_render_finished_semaphores[m_current_frame]
    };
    vkQueueSubmit(m_device->graphics_queue(), 1, &submit_info, m_in_flight_fences[m_current_frame]);

    VkPresentInfoKHR const present_info {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_render_finished_semaphores[m_current_frame],
        .swapchainCount = 1,
        .pSwapchains = &m_handle,
        .pImageIndices = &frame.image_index,
        .pResults = nullptr
    };
    vkQueuePresentKHR(m_device->present_queue(), &present_info);

    m_current_frame = (m_current_frame + 1) % m_config.frames_in_flight;
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
    return {
        .width = width,
        .height = height
    };
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

auto VkSwapchain::create_swapchain() -> std::expected<void, std::string>
{
    m_surface_format = select_surface_format();
    m_extent = select_swap_extent();
    auto image_count = select_image_count();
    auto present_mode = select_present_mode();

    VkSwapchainCreateInfoKHR const swapchain_create_info {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = m_device->surface(),
        .minImageCount = image_count,
        .imageFormat = m_surface_format.format,
        .imageColorSpace = m_surface_format.colorSpace,
        .imageExtent = m_extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = m_device->selected_physical_device()->surface_capabilities().currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    if (auto result = vkCreateSwapchainKHR(m_device->handle(), &swapchain_create_info, nullptr, &m_handle); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to create Vulkan swapchain: {}", string_VkResult(result)));
    }
    return {};
}

auto VkSwapchain::create_images() -> std::expected<void, std::string>
{
    u32 actual_image_count = 0;
    if (auto result = vkGetSwapchainImagesKHR(m_device->handle(), m_handle, &actual_image_count, nullptr); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to retrieve Vulkan swapchain images: {}", string_VkResult(result)));
    }
    m_images.resize(actual_image_count);
    if (auto result = vkGetSwapchainImagesKHR(m_device->handle(), m_handle, &actual_image_count, m_images.data()); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to retrieve Vulkan swapchain images: {}", string_VkResult(result)));
    }

    m_textures.reserve(m_images.size());

    for (auto* image : m_images) {
        Texture::Configuration const texture_config {
            .width = m_extent.width,
            .height = m_extent.height,
            .format = to_graphics(m_surface_format.format),
            .usage = Graphics::TextureUsage::RenderTarget,
            .data = {}
        };

        VkImageViewCreateInfo const image_view_create_info {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = m_surface_format.format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY },
            .subresourceRange = { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1 }
        };

        VkImageView image_view {};
        if (auto result = vkCreateImageView(m_device->handle(), &image_view_create_info, nullptr, &image_view); result != VK_SUCCESS) {
            return std::unexpected(std::format("Failed to create Vulkan swapchain image view: {}", string_VkResult(result)));
        }

        auto texture = VkTexture::create_borrowed(texture_config, m_device, image, image_view);
        if (!texture.has_value()) {
            return std::unexpected(std::format("Failed to create Vulkan swapchain texture: {}", texture.error()));
        }
        m_textures.push_back(std::move(texture.value()));
    }

    return {};
}

auto VkSwapchain::create_command_buffers() -> std::expected<void, std::string>
{
    m_command_buffers.reserve(m_config.frames_in_flight);
    for (i32 i = 0; i < m_config.frames_in_flight; i++) {
        auto command_buffer = VkCommandBuffer::create(m_device->graphics_pool(), m_device);
        if (!command_buffer.has_value()) {
            return std::unexpected(command_buffer.error());
        }
        m_command_buffers.push_back(std::move(command_buffer.value()));
    }

    return {};
}

auto VkSwapchain::create_sync_objects() -> std::expected<void, std::string>
{
    m_image_available_semaphores.resize(m_config.frames_in_flight);
    m_render_finished_semaphores.resize(m_config.frames_in_flight);
    m_in_flight_fences.resize(m_config.frames_in_flight);

    VkSemaphoreCreateInfo const semaphore_create_info {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0
    };

    VkFenceCreateInfo const fence_create_info {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    for (i32 i = 0; i < m_config.frames_in_flight; i++) {
        if (auto result = vkCreateSemaphore(m_device->handle(), &semaphore_create_info, nullptr, &m_image_available_semaphores[i]); result != VK_SUCCESS) {
            return std::unexpected(std::format("Failed to create Vulkan image available semaphore: {}", string_VkResult(result)));
        }
        if (auto result = vkCreateSemaphore(m_device->handle(), &semaphore_create_info, nullptr, &m_render_finished_semaphores[i]); result != VK_SUCCESS) {
            return std::unexpected(std::format("Failed to create Vulkan render finished semaphore: {}", string_VkResult(result)));
        }
        if (auto result = vkCreateFence(m_device->handle(), &fence_create_info, nullptr, &m_in_flight_fences[i]); result != VK_SUCCESS) {
            return std::unexpected(std::format("Failed to create Vulkan in-flight fence: {}", string_VkResult(result)));
        }
    }
    return {};
}

}
