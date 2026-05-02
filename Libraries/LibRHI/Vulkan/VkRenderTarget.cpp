/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <format>

#include <LibRHI/Vulkan/VkRenderPass.h>
#include <LibRHI/Vulkan/VkRenderTarget.h>
#include <LibRHI/Vulkan/VkTexture.h>

namespace RHI {

auto VkRenderTarget::create(RenderPass const* render_pass, Texture const* texture, Texture const* depth_texture, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkRenderTarget>, std::string>
{
    auto const* vk_render_pass = to_vk(render_pass);
    auto const* vk_texture = to_vk(texture);

    std::unique_ptr<VkRenderTarget> render_target(new VkRenderTarget);
    render_target->m_device = device;
    render_target->m_extent = {
        .width = vk_texture->config().width,
        .height = vk_texture->config().height
    };

    std::vector<VkImageView> attachments = { vk_texture->image_view() };
    if (depth_texture != nullptr) {
        attachments.push_back(to_vk(depth_texture)->image_view());
    }

    VkFramebufferCreateInfo const framebuffer_create_info {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .renderPass = vk_render_pass->handle(),
        .attachmentCount = static_cast<u32>(attachments.size()),
        .pAttachments = attachments.data(),
        .width = vk_texture->config().width,
        .height = vk_texture->config().height,
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

auto VkRenderTarget::extent() const -> VkExtent2D
{
    return m_extent;
}

auto to_vk(RenderTarget const* render_target) -> VkRenderTarget const*
{
    return static_cast<VkRenderTarget const*>(render_target);
}

}
