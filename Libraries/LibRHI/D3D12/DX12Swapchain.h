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
#include <LibRHI/Swapchain.h>

namespace RHI {

class DX12Swapchain final : public Swapchain {
    OA_MAKE_NONCOPYABLE(DX12Swapchain);
    OA_MAKE_NONMOVABLE(DX12Swapchain);

public:
    static auto create(Configuration const& config) -> std::expected<std::unique_ptr<DX12Swapchain>, std::string>;

    ~DX12Swapchain() override;

    auto format() const -> Texture::Format override;
    auto textures() const -> std::vector<std::unique_ptr<Texture>> const& override;

    auto begin_frame() -> Frame override;
    void end_frame(Frame const& frame) override;
private:
    DX12Swapchain() = default;
private:
    Configuration m_config;
};

}
