/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <expected>
#include <memory>
#include <string>

#include <Common/Noncopyable.h>
#include <LibRHI/Swapchain.h>
#include <LibRHI/Vulkan/VkDevice.h>

namespace RHI {

class VkSwapchain final : public Swapchain {
    OA_MAKE_NONCOPYABLE(VkSwapchain);
    OA_MAKE_NONMOVABLE(VkSwapchain);

public:
    static auto create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkSwapchain>, std::string>;

    ~VkSwapchain() override;

    auto begin_frame() -> Frame override;
    void end_frame(Frame const& frame) override;
private:
    VkSwapchain() = default;

    auto select_surface_format() const -> VkSurfaceFormatKHR;
    auto select_present_mode() const -> VkPresentModeKHR;
    auto select_swap_extent() const -> VkExtent2D;
    auto select_image_count() const -> u32;

    auto create_swapchain() -> std::expected<void, std::string>;
    auto create_images() -> std::expected<void, std::string>;
    auto create_command_buffers() -> std::expected<void, std::string>;
    auto create_sync_objects() -> std::expected<void, std::string>;
private:
    Configuration m_config;
    VkSwapchainKHR m_swapchain {};
    RHI::VkDevice const* m_device {};
    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_image_views;

    VkCommandPool m_graphics_command_pool {};
    std::vector<VkCommandBuffer> m_command_buffers;
    std::vector<VkSemaphore> m_image_available_semaphores;
    std::vector<VkSemaphore> m_render_finished_semaphores;
    std::vector<VkFence> m_in_flight_fences;
};

}
