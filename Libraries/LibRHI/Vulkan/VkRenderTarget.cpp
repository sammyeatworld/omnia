/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <cassert>
#include <format>

#include <LibRHI/Vulkan/VkRenderPass.h>
#include <LibRHI/Vulkan/VkRenderTarget.h>
#include <LibRHI/Vulkan/VkTexture.h>

namespace RHI {

auto VkRenderTarget::create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkRenderTarget>, std::string>
{
    assert(config.textures.size() > 0);

    std::unique_ptr<VkRenderTarget> render_target(new VkRenderTarget);
    render_target->m_device = device;
    render_target->m_width = config.textures[0]->width();
    render_target->m_height = config.textures[0]->height();

    std::vector<VkImageView> attachments;

    for (auto const* texture : config.textures) {
        assert(texture->width() == render_target->m_width);
        assert(texture->height() == render_target->m_height);

        auto* vk_texture = to_vk(texture);
        attachments.push_back(vk_texture->image_view());
    }

    if (config.depth_texture != nullptr) {
        attachments.push_back(to_vk(config.depth_texture)->image_view());
    }

    auto const* vk_render_pass = to_vk(config.render_pass);
    VkFramebufferCreateInfo const framebuffer_create_info {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .renderPass = vk_render_pass->handle(),
        .attachmentCount = static_cast<u32>(attachments.size()),
        .pAttachments = attachments.data(),
        .width = render_target->m_width,
        .height = render_target->m_height,
        .layers = 1
    };

    if (auto result = vkCreateFramebuffer(device->handle(), &framebuffer_create_info, nullptr, &render_target->m_framebuffer); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to create framebuffer: {}", string_VkResult(result)));
    }

    return render_target;
}

VkRenderTarget::~VkRenderTarget()
{
    if (m_framebuffer != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(m_device->handle(), m_framebuffer, nullptr);
    }
}

auto VkRenderTarget::framebuffer() const -> VkFramebuffer
{
    return m_framebuffer;
}

auto VkRenderTarget::width() const -> u32
{
    return m_width;
}

auto VkRenderTarget::height() const -> u32
{
    return m_height;
}

auto to_vk(RenderTarget const* render_target) -> VkRenderTarget const*
{
    return static_cast<VkRenderTarget const*>(render_target);
}

}
