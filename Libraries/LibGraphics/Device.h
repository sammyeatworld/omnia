/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "Buffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Swapchain.h"
#include "Texture.h"
#include <Common/Noncopyable.h>
#include <LibGraphics/Export.h>
#include <LibUI/Forward.h>

#include <expected>
#include <memory>
#include <string>

namespace Graphics {

class Device {
    OA_MAKE_NONCOPYABLE(Device);
    OA_MAKE_NONMOVABLE(Device);

public:
    static auto GRAPHICS_API create(Renderer::API api) -> std::expected<std::unique_ptr<Device>, std::string>;

    virtual ~Device() = default;

    virtual auto create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string> = 0;
    virtual auto create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string> = 0;
    virtual auto create_swapchain(UI::Window* window, Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string> = 0;
    virtual auto create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string> = 0;
protected:
    Device() = default;
};

}
