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
#include <LibRHI/Texture.h>

namespace RHI {

class DX12Texture final : public Texture {
    OA_MAKE_NONCOPYABLE(DX12Texture);
    OA_MAKE_NONMOVABLE(DX12Texture);

public:
    static auto create(Configuration const& config) -> std::expected<std::unique_ptr<DX12Texture>, std::string>;

    ~DX12Texture() override;

    auto width() const -> u32 override;
    auto height() const -> u32 override;
    auto format() const -> TextureFormat override;
private:
    DX12Texture() = default;
};

}
