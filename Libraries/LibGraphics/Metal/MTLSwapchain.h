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

class MTLSwapchain final : public Swapchain {
    OA_MAKE_NONCOPYABLE(MTLSwapchain);
    OA_MAKE_NONMOVABLE(MTLSwapchain);

public:
    static auto create(UI::Window* window, Configuration const& config) -> std::expected<std::unique_ptr<MTLSwapchain>, std::string>;

    ~MTLSwapchain() override;

    void present() override;
    auto config() const -> const Configuration& override;
private:
    MTLSwapchain() = default;
private:
    Configuration m_config;
};

}
