/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>
#include <LibRHI/Device.h>

namespace RHI {

class DX12Device final : public Device {
    OA_MAKE_NONCOPYABLE(DX12Device);
    OA_MAKE_NONMOVABLE(DX12Device);

public:
    static auto create() -> std::expected<std::unique_ptr<DX12Device>, std::string>;

    ~DX12Device() override;

    auto physical_devices() const -> std::vector<std::string_view> override;
    auto select_physical_device(std::string_view name) -> bool override;

    auto create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string> override;
    auto create_pipeline(Pipeline::Configuration const& config) const -> std::expected<std::unique_ptr<Pipeline>, std::string> override;
    auto create_render_pass(RenderPass::Configuration const& config) const -> std::expected<std::unique_ptr<RenderPass>, std::string> override;
    auto create_render_target(RenderTarget::Configuration const& config) const -> std::expected<std::unique_ptr<RenderTarget>, std::string> override;
    auto create_resource_layout(ResourceLayout::Configuration const& config) const -> std::expected<std::unique_ptr<ResourceLayout>, std::string> override;
    auto create_resource_set(ResourceSet::Configuration const& config) -> std::expected<std::unique_ptr<ResourceSet>, std::string> override;
    auto create_sampler(Sampler::Configuration const& config) const -> std::expected<std::unique_ptr<Sampler>, std::string> override;
    auto create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string> override;
    auto create_swapchain(Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string> override;
    auto create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string> override;
private:
    DX12Device() = default;
};

}
