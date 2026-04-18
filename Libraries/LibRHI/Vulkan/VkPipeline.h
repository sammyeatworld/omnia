/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <expected>
#include <memory>
#include <string>

#include <LibRHI/Pipeline.h>
#include <LibRHI/Vulkan/VkDevice.h>

namespace RHI {

class VkPipeline final : public Pipeline {
    OA_MAKE_NONCOPYABLE(VkPipeline);
    OA_MAKE_DEFAULT_MOVABLE(VkPipeline);

public:
    static auto create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkPipeline>, std::string>;

    ~VkPipeline() override;

    auto handle() const -> ::VkPipeline;
    void bind(CommandBuffer const* command_buffer) const override;
private:
    VkPipeline() = default;
private:
    Configuration m_config;
    RHI::VkDevice const* m_device {};
    ::VkPipeline m_handle {};
    VkPipelineLayout m_layout {};
};

auto to_vk(Pipeline const* pipeline) -> VkPipeline const*;
auto to_vk(CullMode cull_mode) -> VkCullModeFlags;
auto to_vk(FrontFace front_face) -> VkFrontFace;
auto to_vk(PolygonMode polygon_mode) -> VkPolygonMode;
auto to_vk(CompareOp compare_op) -> VkCompareOp;
auto to_vk(AttributeFormat format) -> VkFormat;

}
