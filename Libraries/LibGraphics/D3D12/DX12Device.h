/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "Device.h"
#include <Common/Noncopyable.h>

namespace Graphics {

class DX12Device final : public Device {
    OA_MAKE_NONCOPYABLE(DX12Device);
    OA_MAKE_NONMOVABLE(DX12Device);

public:
    static auto create() -> std::expected<std::unique_ptr<DX12Device>, std::string>;

    auto create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string> override;
    auto create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string> override;
    auto create_swapchain(UI::Window* window, Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string> override;
    auto create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string> override;
private:
    DX12Device() = default;
};

}
