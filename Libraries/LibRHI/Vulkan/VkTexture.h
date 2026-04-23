/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <expected>
#include <memory>
#include <string>

#include <Common/Noncopyable.h>
#include <LibRHI/Texture.h>
#include <LibRHI/Vulkan/VkDevice.h>

namespace RHI {

class VkTexture final : public Texture {
    OA_MAKE_NONCOPYABLE(VkTexture);
    OA_MAKE_NONMOVABLE(VkTexture);

public:
    static auto create_owned(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkTexture>, std::string>;
    static auto create_borrowed(Configuration const& config, RHI::VkDevice const* device, VkImage image, VkImageView image_view) -> std::expected<std::unique_ptr<VkTexture>, std::string>;

    ~VkTexture() override;

    auto config() const -> Configuration const& override;
    auto image() const -> VkImage;
    auto image_view() const -> VkImageView;
    auto sampler() const -> VkSampler;
private:
    VkTexture() = default;
private:
    Configuration m_config {};
    RHI::VkDevice const* m_device {};
    VkImage m_image {};
    VkImageView m_image_view {};
    VkSampler m_sampler {};
    VmaAllocation m_allocation {};
    bool m_owned = false;
};

auto to_rhi(VkFormat format) -> TextureFormat;
auto to_vk(TextureFormat format) -> VkFormat;
auto to_vk(Texture const* texture) -> VkTexture const*;

}
