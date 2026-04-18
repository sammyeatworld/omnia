/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <format>

#include <LibRHI/Vulkan/VkResourceLayout.h>
#include <LibRHI/Vulkan/VkResourceSet.h>

namespace RHI {

auto VkResourceSet::create(ResourceSet::Configuration const& config, RHI::VkDevice* device) -> std::expected<std::unique_ptr<VkResourceSet>, std::string>
{
    std::unique_ptr<VkResourceSet> resource_set(new VkResourceSet);

    auto layout_handle = to_vk(config.layout)->handle();
    VkDescriptorSetAllocateInfo descriptor_set_allocate_info {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = nullptr,
        .descriptorPool = device->descriptor_pool(),
        .descriptorSetCount = 1,
        .pSetLayouts = &layout_handle,
    };

    auto result = vkAllocateDescriptorSets(device->handle(), &descriptor_set_allocate_info, &resource_set->m_handle);
    if (result == VK_ERROR_OUT_OF_POOL_MEMORY) {
        if (auto new_pool = device->grow_descriptor_pool()) {
            descriptor_set_allocate_info.descriptorPool = *new_pool;
            if (auto regrow_result = vkAllocateDescriptorSets(device->handle(), &descriptor_set_allocate_info, &resource_set->m_handle); regrow_result != VK_SUCCESS) {
                return std::unexpected(std::format("Failed to allocate descriptor set after growing pool: {}", string_VkResult(result)));
            }
        }
    } else if (result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to allocate descriptor set: {}", string_VkResult(result)));
    }

    return resource_set;
}

VkResourceSet::~VkResourceSet()
{
}

auto VkResourceSet::handle() const -> VkDescriptorSet
{
    return m_handle;
}

void VkResourceSet::set_uniform_buffer(u32 binding, Buffer const* buffer)
{
    (void)binding;
    (void)buffer;
}

void VkResourceSet::set_texture(u32 binding, Texture const* texture)
{
    (void)binding;
    (void)texture;
}

auto to_vk(ResourceSet const* resource_set) -> VkResourceSet const*
{
    return static_cast<VkResourceSet const*>(resource_set);
}

}
