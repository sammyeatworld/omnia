/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibRHI/Vulkan/VkTexture.h>

namespace RHI {

auto VkTexture::create_owned(Configuration const& config, const RHI::VkDevice* device) -> std::expected<std::unique_ptr<VkTexture>, std::string>
{
    std::unique_ptr<VkTexture> texture(new VkTexture);
    texture->m_config = config;
    texture->m_device = device;
    texture->m_owned = true;
    return texture;
}

auto VkTexture::create_borrowed(Configuration const& config, const RHI::VkDevice* device, VkImage image, VkImageView image_view) -> std::expected<std::unique_ptr<VkTexture>, std::string>
{
    std::unique_ptr<VkTexture> texture(new VkTexture);
    texture->m_config = config;
    texture->m_device = device;
    texture->m_image = image;
    texture->m_image_view = image_view;
    return texture;
}

VkTexture::~VkTexture()
{
    if (m_owned) {
    }
    if (m_image_view != VK_NULL_HANDLE) {
        vkDestroyImageView(m_device->handle(), m_image_view, nullptr);
    }
}

auto VkTexture::config() const -> Configuration const&
{
    return m_config;
}

auto VkTexture::image() const -> VkImage
{
    return m_image;
}

auto VkTexture::image_view() const -> VkImageView
{
    return m_image_view;
}

auto to_rhi(VkFormat format) -> Texture::Format
{
    switch (format) {
        using enum Texture::Format;
    case VK_FORMAT_B8G8R8A8_SRGB:
        return B8G8R8A8_SRGB;
    case VK_FORMAT_B8G8R8A8_UNORM:
        return B8G8R8A8_UNORM;
    default:
        return Unknown;
    }
}

auto to_vk(Texture::Format format) -> VkFormat
{
    switch (format) {
        using enum Texture::Format;
    case B8G8R8A8_SRGB:
        return VK_FORMAT_B8G8R8A8_SRGB;
    case B8G8R8A8_UNORM:
        return VK_FORMAT_B8G8R8A8_UNORM;
    default:
        return VK_FORMAT_UNDEFINED;
    }
}

auto to_vk(Texture const* texture) -> VkTexture const*
{
    return static_cast<VkTexture const*>(texture);
}

}
