/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "VkSwapchain.h"

namespace Graphics {

auto VkSwapchain::create(UI::Window* window, Configuration const& config) -> std::expected<std::unique_ptr<VkSwapchain>, std::string>
{
    (void)window;
    (void)config;
    std::unique_ptr<VkSwapchain> swapchain(new VkSwapchain);
    return swapchain;
}

VkSwapchain::~VkSwapchain()
{
}

void VkSwapchain::present()
{
}

auto VkSwapchain::config() const -> Configuration const&
{
    return m_config;
}

}
