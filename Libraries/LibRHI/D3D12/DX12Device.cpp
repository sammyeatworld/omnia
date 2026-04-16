/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibRHI/D3D12/DX12Buffer.h>
#include <LibRHI/D3D12/DX12Device.h>
#include <LibRHI/D3D12/DX12Shader.h>
#include <LibRHI/D3D12/DX12Swapchain.h>
#include <LibRHI/D3D12/DX12Texture.h>

namespace RHI {

auto DX12Device::create() -> std::expected<std::unique_ptr<DX12Device>, std::string>
{
    std::unique_ptr<DX12Device> device(new DX12Device);
    return device;
}

DX12Device::~DX12Device()
{
}

auto DX12Device::physical_devices() const -> std::vector<std::string_view>
{
    return {};
}

auto DX12Device::select_physical_device(std::string_view name) -> bool
{
    (void)name;
    return false;
}

auto DX12Device::create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string>
{
    return DX12Buffer::create(config);
}

auto DX12Device::create_render_pass(RenderPass::Configuration const& config) const -> std::expected<std::unique_ptr<RenderPass>, std::string>
{
    (void)config;
    return {};
}

auto DX12Device::create_render_target(const RHI::RenderPass* render_pass, const RHI::Texture* texture) const -> std::expected<std::unique_ptr<RenderTarget>, std::string>
{
    (void)render_pass;
    (void)texture;
    return {};
}

auto DX12Device::create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string>
{
    return DX12Shader::create(config);
}

auto DX12Device::create_swapchain(Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string>
{
    return DX12Swapchain::create(config);
}

auto DX12Device::create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string>
{
    return DX12Texture::create(config);
}

}
