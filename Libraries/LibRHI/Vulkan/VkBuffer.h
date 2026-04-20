/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <expected>
#include <memory>
#include <string>

#include <Common/Noncopyable.h>
#include <LibRHI/Buffer.h>
#include <LibRHI/Vulkan/VkDevice.h>

namespace RHI {

class VkBuffer final : public Buffer {
    OA_MAKE_NONCOPYABLE(VkBuffer);
    OA_MAKE_DEFAULT_MOVABLE(VkBuffer);

public:
    static auto create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkBuffer>, std::string>;

    ~VkBuffer() override;

    void set_data(void const* data, u64 size) override;
    auto handle() const -> ::VkBuffer;
private:
    VkBuffer() = default;

    auto create_buffer() -> std::expected<void, std::string>;
    auto upload_data() -> std::expected<void, std::string>;
private:
    Configuration m_config {};
    RHI::VkDevice const* m_device {};
    ::VkBuffer m_handle {};
    VmaAllocation m_allocation {};
    VmaAllocationInfo m_allocation_info {};
};

auto to_vk(Buffer const* buffer) -> RHI::VkBuffer const*;
auto to_vk(BufferUsage usage) -> VkBufferUsageFlags;

}
