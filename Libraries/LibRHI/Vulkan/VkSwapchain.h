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
    static auto create(Configuration const& config, const VkDevice* device) -> std::expected<std::unique_ptr<VkSwapchain>, std::string>;

    ~VkSwapchain() override;

    void present() override;
    auto config() const -> Configuration const& override;
private:
    VkSwapchain() = default;

    auto select_surface_format() const -> VkSurfaceFormatKHR;
    auto select_present_mode() const -> VkPresentModeKHR;
    auto select_swap_extent() const -> VkExtent2D;
    auto select_image_count() const -> u32;
private:
    Configuration m_config;
    VkSwapchainKHR m_swapchain {};
    const VkDevice* m_device {};
    std::vector<VkImage> m_images {};
};

}
