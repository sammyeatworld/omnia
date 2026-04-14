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

    auto handle() const -> void* override;
    void reset() const override;
    void begin() const override;
    void end() const override;
private:
    VkCommandBuffer() = default;
private:
    ::VkCommandBuffer m_handle {};
};

}
