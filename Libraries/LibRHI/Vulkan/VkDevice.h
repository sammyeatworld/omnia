/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "Device.h"
#include "VkCommon.hpp"
#include <Common/Noncopyable.h>

namespace RHI {

class VkDevice final : public Device {
    OA_MAKE_NONCOPYABLE(VkDevice);
    OA_MAKE_NONMOVABLE(VkDevice);

public:
    static auto create(Configuration const& config) -> std::expected<std::unique_ptr<VkDevice>, std::string>;

    ~VkDevice() override;

    auto create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string> override;
    auto create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string> override;
    auto create_swapchain(Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string> override;
    auto create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string> override;
private:
    VkDevice() = default;
private:
    VkInstance m_instance {};
    VkSurfaceKHR m_surface {};
    VkDebugUtilsMessengerEXT m_debug_messenger {};
};

}
