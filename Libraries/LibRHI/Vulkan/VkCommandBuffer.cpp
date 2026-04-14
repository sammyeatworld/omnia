/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <format>

#include <LibRHI/Vulkan/VkCommandBuffer.h>

namespace RHI {

auto VkCommandBuffer::create(VkCommandPool command_pool, const RHI::VkDevice* device) -> std::expected<VkCommandBuffer, std::string>
{
    RHI::VkCommandBuffer command_buffer;
    VkCommandBufferAllocateInfo const command_buffer_allocate_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };
    if (auto result = vkAllocateCommandBuffers(device->handle(), &command_buffer_allocate_info, &command_buffer.m_handle); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to allocate Vulkan command buffer: {}", string_VkResult(result)));
    }
    return command_buffer;
}

VkCommandBuffer::~VkCommandBuffer()
{
}

auto VkCommandBuffer::handle() const -> void*
{
    return m_handle;
}

void VkCommandBuffer::reset() const
{
    vkResetCommandBuffer(m_handle, 0);
}

void VkCommandBuffer::begin() const
{
    VkCommandBufferBeginInfo const command_buffer_begin_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pInheritanceInfo = nullptr
    };
    vkBeginCommandBuffer(m_handle, &command_buffer_begin_info);
}

void VkCommandBuffer::end() const
{
    vkEndCommandBuffer(m_handle);
}

}
