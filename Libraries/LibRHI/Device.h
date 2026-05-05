/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>
#include <Common/Types.h>
#include <LibRHI/Buffer.h>
#include <LibRHI/CommandBuffer.h>
#include <LibRHI/Export.h>
#include <LibRHI/Pipeline.h>
#include <LibRHI/RenderPass.h>
#include <LibRHI/RenderTarget.h>
#include <LibRHI/ResourceLayout.h>
#include <LibRHI/ResourceSet.h>
#include <LibRHI/Sampler.h>
#include <LibRHI/Shader.h>
#include <LibRHI/Swapchain.h>
#include <LibRHI/Texture.h>
#include <LibPlatform/Forward.h>

#include <expected>
#include <memory>
#include <string>
#include <vector>

namespace RHI {

class Device {
    OA_MAKE_NONCOPYABLE(Device);
    OA_MAKE_DEFAULT_MOVABLE(Device);

public:
    enum class API : u8 {
        Vulkan = 0,
        D3D12,
        Metal
    };

    struct Configuration {
        API api;
        bool enable_debug_layer;
        Platform::Window const* window;
    };

    static auto RHI_API create(Configuration const& config) -> std::expected<std::unique_ptr<Device>, std::string>;

    virtual ~Device() = default;

    virtual auto physical_devices() const -> std::vector<std::string_view> = 0;
    virtual auto select_physical_device(std::string_view name) -> bool = 0;

    virtual auto create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string> = 0;
    virtual auto create_pipeline(Pipeline::Configuration const& config) const -> std::expected<std::unique_ptr<Pipeline>, std::string> = 0;
    virtual auto create_render_pass(RenderPass::Configuration const& config) const -> std::expected<std::unique_ptr<RenderPass>, std::string> = 0;
    virtual auto create_render_target(RenderPass const* render_pass, Texture const* texture, Texture const* depth_texture = nullptr) const -> std::expected<std::unique_ptr<RenderTarget>, std::string> = 0;
    virtual auto create_resource_layout(ResourceLayout::Configuration const& config) const -> std::expected<std::unique_ptr<ResourceLayout>, std::string> = 0;
    virtual auto create_resource_set(ResourceSet::Configuration const& config) -> std::expected<std::unique_ptr<ResourceSet>, std::string> = 0;
    virtual auto create_sampler(Sampler::Configuration const& config) const -> std::expected<std::unique_ptr<Sampler>, std::string> = 0;
    virtual auto create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string> = 0;
    virtual auto create_swapchain(Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string> = 0;
    virtual auto create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string> = 0;
protected:
    Device() = default;
};

}
