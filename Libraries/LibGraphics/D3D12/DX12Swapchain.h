/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "Swapchain.h"
#include <Common/Noncopyable.h>
#include <LibUI/Forward.h>

#include <expected>
#include <memory>
#include <string>

namespace Graphics {

class DX12Swapchain final : public Swapchain {
    OA_MAKE_NONCOPYABLE(DX12Swapchain);
    OA_MAKE_NONMOVABLE(DX12Swapchain);

public:
    static auto create(UI::Window* window, Configuration const& config) -> std::expected<std::unique_ptr<DX12Swapchain>, std::string>;

    ~DX12Swapchain() override;

    void present() override;
    auto config() const -> const Configuration& override;
private:
    DX12Swapchain() = default;
private:
    Configuration m_config;
};

}
