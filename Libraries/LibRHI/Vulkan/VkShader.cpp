/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <cassert>
#include <format>

#include <LibRHI/Vulkan/VkShader.h>

namespace RHI {

auto VkShader::create(Configuration const& config, RHI::VkDevice const* device) -> std::expected<std::unique_ptr<VkShader>, std::string>
{
    assert(!config.variants.empty());

    auto variant_it = std::ranges::find_if(config.variants.begin(), config.variants.end(), [](Variant const& variant) {
        return variant.format == Format::SPIRV;
    });
    assert(variant_it != config.variants.end());
    assert(variant_it->bytecode.size() % 4 == 0);

    std::unique_ptr<VkShader> shader(new VkShader);
    shader->m_config = config;
    shader->m_device = device;

    VkShaderModuleCreateInfo const shader_module_create_info {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = variant_it->bytecode.size(),
        .pCode = reinterpret_cast<u32 const*>(variant_it->bytecode.data())
    };
    if (auto result = vkCreateShaderModule(device->handle(), &shader_module_create_info, nullptr, &shader->m_handle); result != VK_SUCCESS) {
        return std::unexpected(std::format("Failed to create vulkan shader module: {}", string_VkResult(result)));
    }

    return shader;
}

VkShader::~VkShader()
{
    if (m_handle != nullptr) {
        vkDestroyShaderModule(m_device->handle(), m_handle, nullptr);
    }
}

auto VkShader::config() const -> Configuration const&
{
    return m_config;
}

}
