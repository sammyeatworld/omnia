/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <format>

#include <LibRHI/Vulkan/VkRenderPass.h>
#include <LibRHI/Vulkan/VkRenderTarget.h>
#include <LibRHI/Vulkan/VkSwapchain.h>

namespace RHI {

auto VkRenderPass::create(const RHI::RenderPass::Configuration& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<RHI::VkRenderPass>, std::string>
{
    std::unique_ptr<VkRenderPass> render_pass(new VkRenderPass);
    render_pass->m_device = device;
    render_pass->m_config = config;

    auto result = render_pass->create_render_pass();
    if (!result.has_value()) {
        return std::unexpected(result.error());
    }

    return render_pass;
}

VkRenderPass::~VkRenderPass()
{
    if (m_handle != VK_NULL_HANDLE) {
        vkDestroyRenderPass(m_device->handle(), m_handle, nullptr);
    }
}

auto VkRenderPass::handle() const -> ::VkRenderPass
{
    return m_handle;
}

void VkRenderPass::begin(CommandBuffer const* command_buffer, RenderTarget const* render_target) const
{
    auto const* vk_cmd = to_vk(command_buffer);
    auto const* vk_render_target = to_vk(render_target);

    VkRenderPassBeginInfo const render_pass_begin_info {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = nullptr,
        .renderPass = m_handle,
        .framebuffer = vk_render_target->framebuffer(),
        .renderArea = {
            .offset = { 0, 0 },
            .extent = vk_render_target->extent() },
        .clearValueCount = static_cast<u32>(m_clear_values.size()),
        .pClearValues = m_clear_values.data()
    };

    vkCmdBeginRenderPass(vk_cmd->handle(), &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
}

void VkRenderPass::end(CommandBuffer const* command_buffer) const
{
    auto vk_cmd = to_vk(command_buffer);

    vkCmdEndRenderPass(vk_cmd->handle());
}

auto VkRenderPass::create_render_pass() -> std::expected<void, std::string>
{
    std::vector<VkAttachmentDescription> color_attachment_descriptions;
    std::vector<VkAttachmentReference> color_attachment_references;

    u32 attachment_index = 0;
    // clang-format off
    for (auto const& color_attachment : m_config.color_attachments) {
        VkAttachmentDescription color_attachment_description {
            .flags = 0,
            .format = to_vk(color_attachment.format),
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = to_vk(color_attachment.load_op),
            .storeOp = to_vk(color_attachment.store_op),
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };
        color_attachment_descriptions.push_back(std::move(color_attachment_description));
        color_attachment_references.emplace_back(attachment_index++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        VkClearValue const clear_value {
            .color = {
                .float32 = {
                    color_attachment.clear_color.x,
                    color_attachment.clear_color.y,
                    color_attachment.clear_color.z,
                    color_attachment.clear_color.w
                }
            }
        };
        m_clear_values.push_back(clear_value);
    }
    // clang-format on

    VkSubpassDescription const subpass_description {
        .flags = 0,
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = nullptr,
        .colorAttachmentCount = static_cast<u32>(color_attachment_references.size()),
        .pColorAttachments = color_attachment_references.data(),
        .pResolveAttachments = nullptr,
        .pDepthStencilAttachment = nullptr,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = nullptr
    };

    VkSubpassDependency const subpass_dependency {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .dependencyFlags = 0,
    };

    VkRenderPassCreateInfo const render_pass_create_info {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .attachmentCount = static_cast<u32>(color_attachment_descriptions.size()),
        .pAttachments = color_attachment_descriptions.data(),
        .subpassCount = 1,
        .pSubpasses = &subpass_description,
        .dependencyCount = 1,
        .pDependencies = &subpass_dependency
    };

    if (auto result = vkCreateRenderPass(m_device->handle(), &render_pass_create_info, nullptr, &m_handle); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to create Vulkan render pass: {}", string_VkResult(result)));
    }
    return {};
}

auto to_vk(RenderPass::LoadOp load_op) -> VkAttachmentLoadOp
{
    switch (load_op) {
        using enum RenderPass::LoadOp;
    case Load:
        return VK_ATTACHMENT_LOAD_OP_LOAD;
    case Clear:
        return VK_ATTACHMENT_LOAD_OP_CLEAR;
    case DontCare:
        return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    }
    return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
}

auto to_vk(RenderPass::StoreOp store_op) -> VkAttachmentStoreOp
{
    switch (store_op) {
        using enum RenderPass::StoreOp;
    case Store:
        return VK_ATTACHMENT_STORE_OP_STORE;
    case DontCare:
        return VK_ATTACHMENT_STORE_OP_DONT_CARE;
    }
    return VK_ATTACHMENT_STORE_OP_DONT_CARE;
}

auto to_vk(RenderPass const* render_pass) -> RHI::VkRenderPass const*
{
    return static_cast<RHI::VkRenderPass const*>(render_pass);
}

}
