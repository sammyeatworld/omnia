/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <format>

#include <LibRHI/Vulkan/VkDevice.h>
#include <LibRHI/Vulkan/VkPipeline.h>
#include <LibRHI/Vulkan/VkRenderPass.h>
#include <LibRHI/Vulkan/VkResourceLayout.h>
#include <LibRHI/Vulkan/VkShader.h>

namespace RHI {

auto VkPipeline::create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkPipeline>, std::string>
{
    std::unique_ptr<VkPipeline> pipeline(new VkPipeline);
    pipeline->m_config = config;
    pipeline->m_device = device;

    auto* vk_render_pass = to_vk(config.render_pass)->handle();
    auto* vk_vertex_shader = to_vk(config.vertex_shader)->handle();
    auto* vk_fragment_shader = to_vk(config.fragment_shader)->handle();
    std::vector<VkPipelineShaderStageCreateInfo> const shader_stages {
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vk_vertex_shader,
            .pName = "main",
            .pSpecializationInfo = nullptr,
        },
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = vk_fragment_shader,
            .pName = "main",
            .pSpecializationInfo = nullptr,
        }
    };

    VkVertexInputBindingDescription const vertex_input_binding_description {
        .binding = 0,
        .stride = config.vertex_binding.stride,
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };

    std::vector<VkVertexInputAttributeDescription> vertex_input_attribute_descriptions;
    for (auto const& attribute : config.vertex_binding.attributes) {
        vertex_input_attribute_descriptions.push_back({
            .location = attribute.location,
            .binding = 0,
            .format = to_vk(attribute.format),
            .offset = attribute.offset,
        });
    }

    VkPipelineVertexInputStateCreateInfo const vertex_input_state_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = &vertex_input_binding_description,
        .vertexAttributeDescriptionCount = static_cast<u32>(vertex_input_attribute_descriptions.size()),
        .pVertexAttributeDescriptions = vertex_input_attribute_descriptions.data(),
    };

    VkPipelineInputAssemblyStateCreateInfo const input_assembly_state_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };

    VkPipelineViewportStateCreateInfo const viewport_state_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .viewportCount = 1,
        .pViewports = nullptr,
        .scissorCount = 1,
        .pScissors = nullptr,
    };

    VkPipelineRasterizationStateCreateInfo const rasterization_state_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = to_vk(config.rasterization.polygon_mode),
        .cullMode = to_vk(config.rasterization.cull_mode),
        .frontFace = to_vk(config.rasterization.front_face),
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0F,
        .depthBiasClamp = 0.0F,
        .depthBiasSlopeFactor = 0.0F,
        .lineWidth = 1.0F,
    };

    VkPipelineMultisampleStateCreateInfo const multisample_state_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1.0F,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE,
    };

    VkPipelineColorBlendAttachmentState const color_blend_attachment_state {
        .blendEnable = VK_FALSE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    };

    VkPipelineColorBlendStateCreateInfo const color_blend_state_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &color_blend_attachment_state,
        .blendConstants = { 0.0F, 0.0F, 0.0F, 0.0F },
    };

    VkPipelineLayoutCreateInfo pipeline_layout_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr,
    };

    std::vector<VkDescriptorSetLayout> descriptor_set_layouts;
    descriptor_set_layouts.reserve(config.resource_layouts.size());
    for (auto const* resource_layout : config.resource_layouts) {
        descriptor_set_layouts.push_back(to_vk(resource_layout)->handle());
    }
    if (!descriptor_set_layouts.empty()) {
        pipeline_layout_create_info.setLayoutCount = static_cast<u32>(descriptor_set_layouts.size());
        pipeline_layout_create_info.pSetLayouts = descriptor_set_layouts.data();
    }

    if (auto result = vkCreatePipelineLayout(device->handle(), &pipeline_layout_create_info, nullptr, &pipeline->m_layout); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to create vulkan pipeline layout: {}", string_VkResult(result)));
    }

    VkDynamicState const dynamic_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    VkPipelineDynamicStateCreateInfo const dynamic_state_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .dynamicStateCount = static_cast<u32>(std::size(dynamic_states)),
        .pDynamicStates = dynamic_states,
    };

    VkGraphicsPipelineCreateInfo const pipeline_create_info {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stageCount = static_cast<u32>(shader_stages.size()),
        .pStages = shader_stages.data(),
        .pVertexInputState = &vertex_input_state_create_info,
        .pInputAssemblyState = &input_assembly_state_create_info,
        .pTessellationState = nullptr,
        .pViewportState = &viewport_state_create_info,
        .pRasterizationState = &rasterization_state_create_info,
        .pMultisampleState = &multisample_state_create_info,
        .pDepthStencilState = nullptr,
        .pColorBlendState = &color_blend_state_create_info,
        .pDynamicState = &dynamic_state_create_info,
        .layout = pipeline->m_layout,
        .renderPass = vk_render_pass,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = -1,
    };

    if (auto result = vkCreateGraphicsPipelines(device->handle(), VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &pipeline->m_handle); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to create vulkan graphics pipeline: {}", string_VkResult(result)));
    }

    return pipeline;
}

VkPipeline::~VkPipeline()
{
    if (m_handle != VK_NULL_HANDLE) {
        vkDestroyPipeline(m_device->handle(), m_handle, nullptr);
    }
    if (m_layout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(m_device->handle(), m_layout, nullptr);
    }
}

auto VkPipeline::handle() const -> ::VkPipeline
{
    return m_handle;
}

auto VkPipeline::layout() const -> VkPipelineLayout
{
    return m_layout;
}

auto to_vk(Pipeline const* pipeline) -> VkPipeline const*
{
    return static_cast<VkPipeline const*>(pipeline);
}

auto to_vk(CullMode cull_mode) -> VkCullModeFlags
{
    switch (cull_mode) {
    case CullMode::None:
        return VK_CULL_MODE_NONE;
    case CullMode::Front:
        return VK_CULL_MODE_FRONT_BIT;
    case CullMode::Back:
        return VK_CULL_MODE_BACK_BIT;
    }
}

auto to_vk(FrontFace front_face) -> VkFrontFace
{
    switch (front_face) {
    case FrontFace::Clockwise:
        return VK_FRONT_FACE_CLOCKWISE;
    case FrontFace::CounterClockwise:
        return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    }
}

auto to_vk(PolygonMode polygon_mode) -> VkPolygonMode
{
    switch (polygon_mode) {
    case PolygonMode::Fill:
        return VK_POLYGON_MODE_FILL;
    case PolygonMode::Line:
        return VK_POLYGON_MODE_LINE;
    case PolygonMode::Point:
        return VK_POLYGON_MODE_POINT;
    }
}

auto to_vk(CompareOp compare_op) -> VkCompareOp
{
    switch (compare_op) {
    case CompareOp::Never:
        return VK_COMPARE_OP_NEVER;
    case CompareOp::Less:
        return VK_COMPARE_OP_LESS;
    case CompareOp::Equal:
        return VK_COMPARE_OP_EQUAL;
    case CompareOp::LessOrEqual:
        return VK_COMPARE_OP_LESS_OR_EQUAL;
    case CompareOp::Greater:
        return VK_COMPARE_OP_GREATER;
    case CompareOp::NotEqual:
        return VK_COMPARE_OP_NOT_EQUAL;
    case CompareOp::GreaterOrEqual:
        return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case CompareOp::Always:
        return VK_COMPARE_OP_ALWAYS;
    }
}

auto to_vk(AttributeFormat format) -> VkFormat
{
    switch (format) {
    case AttributeFormat::Float32Vec4:
        return VK_FORMAT_R32G32B32A32_SFLOAT;
    case AttributeFormat::Float32Vec3:
        return VK_FORMAT_R32G32B32_SFLOAT;
    case AttributeFormat::Float32Vec2:
        return VK_FORMAT_R32G32_SFLOAT;
    case AttributeFormat::Float32:
        return VK_FORMAT_R32_SFLOAT;
    }
}

}
