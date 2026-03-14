/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "MTLDevice.h"
#include "MTLBuffer.h"
#include "MTLShader.h"
#include "MTLSwapchain.h"
#include "MTLTexture.h"

namespace Graphics {

auto MTLDevice::create() -> std::expected<std::unique_ptr<MTLDevice>, std::string>
{
    std::unique_ptr<MTLDevice> device(new MTLDevice);
    return device;
}

auto MTLDevice::create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string>
{
    return MTLBuffer::create(config);
}

auto MTLDevice::create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string>
{
    return MTLShader::create(config);
}

auto MTLDevice::create_swapchain(UI::Window* window, Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string>
{
    return MTLSwapchain::create(window, config);
}

auto MTLDevice::create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string>
{
    return MTLTexture::create(config);
}

}
