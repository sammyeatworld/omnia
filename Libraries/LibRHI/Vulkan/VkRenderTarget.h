/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <expected>
#include <memory>
#include <string>

#include <LibRHI/RenderTarget.h>
#include <LibRHI/Vulkan/VkDevice.h>

namespace RHI {

class VkRenderTarget final : public RenderTarget {
    OA_MAKE_NONCOPYABLE(VkRenderTarget);
    OA_MAKE_DEFAULT_MOVABLE(VkRenderTarget);

public:
    static auto create(RenderPass const* render_pass, Texture const* texture, Texture const* depth_texture, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkRenderTarget>, std::string>;

    auto framebuffer() const -> VkFramebuffer;
    auto extent() const -> VkExtent2D;

    ~VkRenderTarget() override;
private:
    VkRenderTarget() = default;
private:
    RHI::VkDevice const* m_device {};
    VkExtent2D m_extent {};
    VkFramebuffer m_framebuffer {};
};

auto to_vk(RenderTarget const* render_target) -> VkRenderTarget const*;

}
