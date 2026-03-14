/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "DX12Device.h"
#include "DX12Buffer.h"
#include "DX12Shader.h"
#include "DX12Swapchain.h"
#include "DX12Texture.h"

namespace Graphics {

auto DX12Device::create() -> std::expected<std::unique_ptr<DX12Device>, std::string>
{
    std::unique_ptr<DX12Device> device(new DX12Device);
    return device;
}

auto DX12Device::create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string>
{
    return DX12Buffer::create(config);
}

auto DX12Device::create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string>
{
    return DX12Shader::create(config);
}

auto DX12Device::create_swapchain(UI::Window* window, Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string>
{
    return DX12Swapchain::create(window, config);
}

auto DX12Device::create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string>
{
    return DX12Texture::create(config);
}

}
