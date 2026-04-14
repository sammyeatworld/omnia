/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>
#include <Common/Types.h>
#include <LibRHI/Buffer.h>
#include <LibRHI/Export.h>
#include <LibRHI/Shader.h>
#include <LibRHI/Swapchain.h>
#include <LibRHI/Texture.h>
#include <LibUI/Forward.h>

#include <expected>
#include <memory>
#include <string>
#include <vector>

namespace RHI {

class Device {
    OA_MAKE_NONCOPYABLE(Device);
    OA_MAKE_NONMOVABLE(Device);

public:
    enum class API : u8 {
        Vulkan = 0,
        D3D12,
        Metal
    };

    struct Configuration {
        API api;
        bool enable_debug_layer;
        UI::Window const* window;
    };

    static auto RHI_API create(Configuration const& config) -> std::expected<std::unique_ptr<Device>, std::string>;

    virtual ~Device() = default;

    virtual auto physical_devices() const -> std::vector<std::string_view> = 0;
    virtual auto select_physical_device(std::string_view name) -> bool = 0;

    virtual auto create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string> = 0;
    virtual auto create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string> = 0;
    virtual auto create_swapchain(Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string> = 0;
    virtual auto create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string> = 0;
protected:
    Device() = default;
};

}
