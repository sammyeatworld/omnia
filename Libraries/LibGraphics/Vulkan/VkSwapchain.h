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

class VkSwapchain final : public Swapchain {
    OA_MAKE_NONCOPYABLE(VkSwapchain);
    OA_MAKE_NONMOVABLE(VkSwapchain);

public:
    static auto create(UI::Window* window, Configuration const& config) -> std::expected<std::unique_ptr<VkSwapchain>, std::string>;

    ~VkSwapchain() override;

    void present() override;
    auto config() const -> const Configuration& override;
private:
    VkSwapchain() = default;
private:
    Configuration m_config;
};

}
