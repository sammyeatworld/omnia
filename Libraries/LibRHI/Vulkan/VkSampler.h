/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <expected>
#include <memory>
#include <string>

#include <LibRHI/Sampler.h>
#include <LibRHI/Vulkan/VkDevice.h>

namespace RHI {

class VkSampler final : public Sampler {
    OA_MAKE_NONCOPYABLE(VkSampler);
    OA_MAKE_DEFAULT_MOVABLE(VkSampler);

public:
    static auto create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkSampler>, std::string>;

    ~VkSampler() override;

    auto handle() const -> ::VkSampler;
private:
    VkSampler() = default;
private:
    ::VkSampler m_handle {};
    RHI::VkDevice const* m_device {};
};

auto to_vk(Sampler const* sampler) -> RHI::VkSampler const*;
auto to_vk(Filter filter) -> VkFilter;
auto to_vk(AddressMode address_mode) -> VkSamplerAddressMode;

}
