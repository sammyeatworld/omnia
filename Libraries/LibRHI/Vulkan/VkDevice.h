/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <unordered_map>
#include <vk_mem_alloc.h>

#include <Common/Noncopyable.h>
#include <LibRHI/Device.h>
#include <LibRHI/Vulkan/VkCommon.h>
#include <LibRHI/Vulkan/VkPhysicalDevice.h>
#include <LibRHI/Vulkan/VkCommandBuffer.h>

namespace RHI {

class VkDevice final : public Device {
    OA_MAKE_NONCOPYABLE(VkDevice);
    OA_MAKE_DEFAULT_MOVABLE(VkDevice);

public:
    static auto create(Configuration const& config) -> std::expected<std::unique_ptr<VkDevice>, std::string>;

    ~VkDevice() override;

    auto descriptor_pool() const -> VkDescriptorPool;
    auto grow_descriptor_pool() -> std::expected<VkDescriptorPool, std::string>;
    auto allocator() const -> VmaAllocator;
    auto handle() const -> ::VkDevice;
    auto surface() const -> VkSurfaceKHR;

    void submit_graphics(RHI::VkCommandBuffer const& command_buffer) const;
    auto graphics_command_buffer() const -> RHI::VkCommandBuffer const&;

    auto graphics_queue() const -> VkQueue;
    auto present_queue() const -> VkQueue;
    auto transfer_queue() const -> VkQueue;

    auto graphics_pool() const -> VkCommandPool;
    auto transfer_pool() const -> VkCommandPool;

    auto selected_physical_device() const -> VkPhysicalDevice const*;
    auto physical_devices() const -> std::vector<std::string_view> override;
    auto select_physical_device(std::string_view name) -> bool override;

    auto create_buffer(Buffer::Configuration const& config) const -> std::expected<std::unique_ptr<Buffer>, std::string> override;
    auto create_pipeline(Pipeline::Configuration const& config) const -> std::expected<std::unique_ptr<Pipeline>, std::string> override;
    auto create_render_pass(RenderPass::Configuration const& config) const -> std::expected<std::unique_ptr<RenderPass>, std::string> override;
    auto create_render_target(RenderPass const* render_pass, Texture const* texture) const -> std::expected<std::unique_ptr<RenderTarget>, std::string> override;
    auto create_resource_layout(ResourceLayout::Configuration const& config) const -> std::expected<std::unique_ptr<ResourceLayout>, std::string> override;
    auto create_resource_set(ResourceSet::Configuration const& config) -> std::expected<std::unique_ptr<ResourceSet>, std::string> override;
    auto create_sampler(Sampler::Configuration const& config) const -> std::expected<std::unique_ptr<Sampler>, std::string> override;
    auto create_shader(Shader::Configuration const& config) const -> std::expected<std::unique_ptr<Shader>, std::string> override;
    auto create_swapchain(Swapchain::Configuration const& config) const -> std::expected<std::unique_ptr<Swapchain>, std::string> override;
    auto create_texture(Texture::Configuration const& config) const -> std::expected<std::unique_ptr<Texture>, std::string> override;
private:
    VkDevice() = default;

    auto create_instance() -> std::expected<void, std::string>;
    auto create_surface() -> std::expected<void, std::string>;
    auto create_logical_device() -> std::expected<void, std::string>;
    auto create_allocator() -> std::expected<void, std::string>;
    auto create_command_pools() -> std::expected<void, std::string>;
    auto create_descriptor_pool() -> std::expected<void, std::string>;
private:
    Configuration m_config {};
    VmaAllocator m_allocator {};
    VkInstance m_instance {};
    VkSurfaceKHR m_surface {};
    std::vector<VkDescriptorPool> m_descriptor_pools;
    u32 m_descriptor_pool_capacity = 16U;
    VkDebugUtilsMessengerEXT m_debug_messenger {};

    RHI::VkPhysicalDevice* m_physical_device {};
    ::VkDevice m_logical_device {};
    std::unordered_map<std::string_view, RHI::VkPhysicalDevice> m_physical_devices;

    VkQueue m_graphics_queue {};
    VkQueue m_present_queue {};
    VkQueue m_transfer_queue {};
    VkCommandPool m_graphics_command_pool {};
    VkCommandPool m_transfer_command_pool {};
    RHI::VkCommandBuffer m_graphics_command_buffer {};
};

}
