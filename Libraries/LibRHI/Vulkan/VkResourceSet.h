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
#include <LibRHI/ResourceSet.h>
#include <LibRHI/Vulkan/VkDevice.h>

namespace RHI {

class VkResourceSet : public ResourceSet {
    OA_MAKE_NONCOPYABLE(VkResourceSet);
    OA_MAKE_DEFAULT_MOVABLE(VkResourceSet);

public:
    static auto create(ResourceSet::Configuration const& config, RHI::VkDevice* device) -> std::expected<std::unique_ptr<VkResourceSet>, std::string>;

    ~VkResourceSet() override;

    auto handle() const -> VkDescriptorSet;

    void set_uniform_buffer(u32 binding, Buffer const* buffer) override;
    void set_texture(u32 binding, Texture const* texture) override;
private:
    VkResourceSet() = default;
private:
    VkDescriptorSet m_handle {};
    RHI::VkDevice const* m_device {};
};

auto to_vk(ResourceSet const* resource_set) -> VkResourceSet const*;

}
