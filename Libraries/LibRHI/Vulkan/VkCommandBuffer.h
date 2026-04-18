/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Noncopyable.h>
#include <LibRHI/CommandBuffer.h>
#include <LibRHI/Vulkan/VkDevice.h>

namespace RHI {

class VkCommandBuffer final : public CommandBuffer {
    OA_MAKE_NONCOPYABLE(VkCommandBuffer);
    OA_MAKE_DEFAULT_MOVABLE(VkCommandBuffer);

public:
    ~VkCommandBuffer() override;

    static auto create(VkCommandPool command_pool, RHI::VkDevice const* device) -> std::expected<VkCommandBuffer, std::string>;

    auto handle() const -> ::VkCommandBuffer;
    void reset() const override;

    void begin() const override;
    void end() const override;

    void begin_render_pass(RenderPass const* render_pass, RenderTarget const* render_target) const override;
    void end_render_pass() const override;

    void bind_vertex_buffer(Buffer const* vertex_buffer) const override;
    void bind_pipeline(Pipeline const* pipeline) const override;

    void draw(u32 vertex_count, u32 instance_count, u32 first_vertex, u32 first_instance) const override;

    void set_viewport(u32 x, u32 y, u32 width, u32 height) const override;
    void set_scissor(u32 x, u32 y, u32 width, u32 height) const override;
private:
    VkCommandBuffer() = default;
private:
    ::VkCommandBuffer m_handle {};
};

auto to_vk(CommandBuffer const* command_buffer) -> VkCommandBuffer const*;

}
