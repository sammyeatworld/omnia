/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "VkDevice.h"
#include "VkBuffer.h"
#include "VkShader.h"
#include "VkSwapchain.h"
#include "VkTexture.h"

namespace Graphics {

auto VkDevice::create() -> std::expected<std::unique_ptr<VkDevice>, std::string>
{
    std::unique_ptr<VkDevice> device(new VkDevice);
    return device;
}

auto VkDevice::create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string>
{
    return VkBuffer::create(config);
}

auto VkDevice::create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string>
{
    return VkShader::create(config);
}

auto VkDevice::create_swapchain(UI::Window* window, Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string>
{
    return VkSwapchain::create(window, config);
}

auto VkDevice::create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string>
{
    return VkTexture::create(config);
}

}
