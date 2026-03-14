/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "Device.h"
#include <Common/Noncopyable.h>

namespace Graphics {

class VkDevice final: public Device {
    OA_MAKE_NONCOPYABLE(VkDevice);
    OA_MAKE_NONMOVABLE(VkDevice);

public:
    static auto create() -> std::expected<std::unique_ptr<VkDevice>, std::string>;

    auto create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string> override;
    auto create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string> override;
    auto create_swapchain(UI::Window* window, Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string> override;
    auto create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string> override;
private:
    VkDevice() = default;
};

}
