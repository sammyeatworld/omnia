/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <format>

#include <LibRHI/Vulkan/VkBuffer.h>

namespace RHI {

auto VkBuffer::create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkBuffer>, std::string>
{
    std::unique_ptr<VkBuffer> buffer(new VkBuffer);
    buffer->m_config = config;
    buffer->m_device = device;

    return buffer->create_buffer()
        .and_then([&]() {
            return buffer->upload_data();
        })
        .transform([&]() {
            return std::move(buffer);
        });
}

VkBuffer::~VkBuffer()
{
    if (m_handle != VK_NULL_HANDLE) {
        vmaDestroyBuffer(m_device->allocator(), m_handle, m_allocation);
    }
}

void VkBuffer::set_data(void const* data, u64 size)
{
    m_config.data = data;
    m_config.size = size;
    auto upload_result = upload_data();
}

auto VkBuffer::handle() const -> ::VkBuffer
{
    return m_handle;
}

auto VkBuffer::create_buffer() -> std::expected<void, std::string>
{
    VkBufferCreateInfo buffer_create_info {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = m_config.size,
        .usage = to_vk(m_config.usage),
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
    };
    if (m_config.usage == BufferUsage::Vertex || m_config.usage == BufferUsage::Index) {
        buffer_create_info.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }

    VmaAllocationCreateInfo allocation_create_info {
        .flags = 0,
        .usage = VMA_MEMORY_USAGE_AUTO,
        .requiredFlags = 0,
        .preferredFlags = 0,
        .memoryTypeBits = 0,
        .pool = nullptr,
        .pUserData = nullptr,
        .priority = 0.0F,
    };
    if (m_config.usage == BufferUsage::Vertex || m_config.usage == BufferUsage::Index) {
        allocation_create_info.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    } else if (m_config.usage == BufferUsage::Uniform) {
        allocation_create_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
        allocation_create_info.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }

    if (auto result = vmaCreateBuffer(m_device->allocator(), &buffer_create_info, &allocation_create_info, &m_handle, &m_allocation, &m_allocation_info); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to create Vulkan buffer: {}", string_VkResult(result)));
    }
    return {};
}

auto VkBuffer::upload_data() -> std::expected<void, std::string>
{
    if (m_config.data == nullptr || m_config.size == 0) {
        return {};
    }

    if (m_config.usage == BufferUsage::Uniform || m_config.usage == BufferUsage::Storage) {
        std::memcpy(m_allocation_info.pMappedData, m_config.data, m_config.size);
        return {};
    }

    VkBufferCreateInfo const staging_buffer_create_info {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = m_config.size,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
    };

    VmaAllocationCreateInfo const staging_allocation_create_info {
        .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
        .requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .preferredFlags = 0,
        .memoryTypeBits = 0,
        .pool = nullptr,
        .pUserData = nullptr,
        .priority = 0.0F,
    };

    VmaAllocationInfo staging_allocation_info {};

    ::VkBuffer staging_buffer {};
    VmaAllocation staging_allocation {};
    if (auto result = vmaCreateBuffer(m_device->allocator(), &staging_buffer_create_info, &staging_allocation_create_info, &staging_buffer, &staging_allocation, &staging_allocation_info); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to create Vulkan staging buffer: {}", string_VkResult(result)));
    }

    std::memcpy(staging_allocation_info.pMappedData, m_config.data, static_cast<std::size_t>(m_config.size));

    auto cmd = m_device->begin_single_transfer_command();
    if (!cmd) {
        vmaDestroyBuffer(m_device->allocator(), staging_buffer, staging_allocation);
        return std::unexpected(cmd.error());
    }

    VkBufferCopy const copy_region {
        .srcOffset = 0,
        .dstOffset = 0,
        .size = m_config.size
    };
    vkCmdCopyBuffer(cmd.value(), staging_buffer, m_handle, 1, &copy_region);
    m_device->end_single_transfer_command(cmd.value());
    vmaDestroyBuffer(m_device->allocator(), staging_buffer, staging_allocation);

    return {};
}

auto to_vk(Buffer const* buffer) -> RHI::VkBuffer const*
{
    return static_cast<RHI::VkBuffer const*>(buffer);
}

auto to_vk(BufferUsage usage) -> VkBufferUsageFlags
{
    switch (usage) {
    case BufferUsage::Vertex:
        return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    case BufferUsage::Index:
        return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    case BufferUsage::Uniform:
        return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    case BufferUsage::Storage:
        return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    }
}

}
