/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <expected>
#include <memory>
#include <string>

#include <LibRHI/ResourceLayout.h>
#include <LibRHI/Vulkan/VkDevice.h>

namespace RHI {

class VkResourceLayout : public ResourceLayout {
    OA_MAKE_NONCOPYABLE(VkResourceLayout);
    OA_MAKE_DEFAULT_MOVABLE(VkResourceLayout);

public:
    static auto create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkResourceLayout>, std::string>;

    ~VkResourceLayout() override;

    auto handle() const -> VkDescriptorSetLayout;
private:
    VkResourceLayout() = default;
private:
    RHI::VkDevice const* m_device {};
    VkDescriptorSetLayout m_handle {};
};

auto to_vk(ResourceLayout const* layout) -> VkResourceLayout const*;
auto to_vk(ResourceType type) -> VkDescriptorType;

}