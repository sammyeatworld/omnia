/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <format>

#include <LibRHI/Vulkan/VkResourceLayout.h>
#include <LibRHI/Vulkan/VkShader.h>

namespace RHI {

auto VkResourceLayout::create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkResourceLayout>, std::string>
{
    std::unique_ptr<VkResourceLayout> resource_layout(new VkResourceLayout);
    resource_layout->m_device = device;

    std::vector<VkDescriptorSetLayoutBinding> bindings;
    for (auto const& binding : config.bindings) {
        bindings.push_back({ .binding = binding.binding,
            .descriptorType = to_vk(binding.type),
            .descriptorCount = 1,
            .stageFlags = to_vk(binding.stage),
            .pImmutableSamplers = nullptr });
    }
    VkDescriptorSetLayoutCreateInfo const layout_create_info {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .bindingCount = static_cast<uint32_t>(bindings.size()),
        .pBindings = bindings.data(),
    };

    if (auto result = vkCreateDescriptorSetLayout(device->handle(), &layout_create_info, nullptr, &resource_layout->m_handle); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to create descriptor set layout: {}", std::to_string(result)));
    }

    return resource_layout;
}

VkResourceLayout::~VkResourceLayout()
{
    if (m_handle != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(m_device->handle(), m_handle, nullptr);
    }
}

auto VkResourceLayout::handle() const -> VkDescriptorSetLayout
{
    return m_handle;
}

auto to_vk(ResourceLayout const* layout) -> VkResourceLayout const*
{
    return static_cast<VkResourceLayout const*>(layout);
}

auto to_vk(ResourceType type) -> VkDescriptorType
{
    switch (type) {
    case ResourceType::UniformBuffer:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case ResourceType::Texture:
        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    case ResourceType::Sampler:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    }
}

}
