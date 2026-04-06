/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <unordered_map>

#include <Common/Noncopyable.h>
#include <LibRHI/Device.h>
#include <LibRHI/Vulkan/VkCommon.h>
#include <LibRHI/Vulkan/VkPhysicalDevice.h>

namespace RHI {

class VkDevice final : public Device {
    OA_MAKE_NONCOPYABLE(VkDevice);
    OA_MAKE_NONMOVABLE(VkDevice);

public:
    static auto create(Configuration const& config) -> std::expected<std::unique_ptr<VkDevice>, std::string>;

    ~VkDevice() override;

    auto handle() const -> ::VkDevice;
    auto surface() const -> VkSurfaceKHR;
    auto selected_physical_device() const -> VkPhysicalDevice const*;
    auto physical_devices() const -> std::vector<std::string_view> override;
    auto select_physical_device(std::string_view name) -> bool override;

    auto create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string> override;
    auto create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string> override;
    auto create_swapchain(Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string> override;
    auto create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string> override;
private:
    VkDevice() = default;

    auto recreate_logical_device() -> bool;
    void release_logical_device();
private:
    VkInstance m_instance {};
    VkSurfaceKHR m_surface {};
    VkDebugUtilsMessengerEXT m_debug_messenger {};
    RHI::VkPhysicalDevice* m_physical_device {};
    ::VkDevice m_logical_device {};
    VkQueue m_graphics_queue {};
    VkQueue m_present_queue {};
    std::unordered_map<std::string_view, RHI::VkPhysicalDevice> m_physical_devices {};
};

}
