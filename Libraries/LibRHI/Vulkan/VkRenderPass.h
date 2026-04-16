/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <expected>
#include <memory>
#include <string>

#include <LibRHI/RenderPass.h>
#include <LibRHI/Vulkan/VkCommon.h>
#include <LibRHI/Vulkan/VkDevice.h>

namespace RHI {

class VkRenderPass final : public RenderPass {
    OA_MAKE_NONCOPYABLE(VkRenderPass);
    OA_MAKE_DEFAULT_MOVABLE(VkRenderPass);

public:
    static auto create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<RHI::VkRenderPass>, std::string>;

    ~VkRenderPass() override;

    auto handle() const -> ::VkRenderPass;

    void begin(CommandBuffer const* command_buffer, RenderTarget const* render_target) const override;
    void end(CommandBuffer const* command_buffer) const override;
private:
    VkRenderPass() = default;

    auto create_render_pass() -> std::expected<void, std::string>;
private:
    Configuration m_config {};
    ::VkRenderPass m_handle {};
    RHI::VkDevice const* m_device {};
    std::vector<VkClearValue> m_clear_values;
};

auto to_vk(RenderPass::LoadOp load_op) -> VkAttachmentLoadOp;
auto to_vk(RenderPass::StoreOp store_op) -> VkAttachmentStoreOp;
auto to_vk(RenderPass const* render_pass) -> RHI::VkRenderPass const*;

}
