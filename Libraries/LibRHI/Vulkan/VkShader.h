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
#include <LibRHI/Shader.h>
#include <LibRHI/Vulkan/VkDevice.h>

namespace RHI {

class VkShader final : public Shader {
public:
    static auto create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkShader>, std::string>;

    ~VkShader() override;

    auto config() const -> Configuration const& override;
private:
    VkShader() = default;
private:
    Configuration m_config;
    RHI::VkDevice const* m_device {};
    VkShaderModule m_handle {};
};

}
