/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "DX12Swapchain.h"

namespace Graphics {

auto DX12Swapchain::create(UI::Window* window, Configuration const& config) -> std::expected<std::unique_ptr<DX12Swapchain>, std::string>
{
    (void)window;
    (void)config;
    std::unique_ptr<DX12Swapchain> swapchain(new DX12Swapchain);
    return swapchain;
}

DX12Swapchain::~DX12Swapchain()
{
}

void DX12Swapchain::present()
{
}

auto DX12Swapchain::config() const -> Configuration const&
{
    return m_config;
}

}
