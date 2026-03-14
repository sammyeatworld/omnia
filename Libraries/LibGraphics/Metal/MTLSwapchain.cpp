/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "MTLSwapchain.h"

namespace Graphics {

auto MTLSwapchain::create(UI::Window* window, Configuration const& config) -> std::expected<std::unique_ptr<MTLSwapchain>, std::string>
{
    (void)window;
    (void)config;
    std::unique_ptr<MTLSwapchain> swapchain(new MTLSwapchain);
    return swapchain;
}

MTLSwapchain::~MTLSwapchain()
{
}

void MTLSwapchain::present()
{
}

auto MTLSwapchain::config() const -> Configuration const&
{
    return m_config;
}

}
