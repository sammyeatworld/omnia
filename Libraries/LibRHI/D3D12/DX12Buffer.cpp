/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibRHI/D3D12/DX12Buffer.h>

namespace RHI {

auto DX12Buffer::create(Configuration const& config) -> std::expected<std::unique_ptr<DX12Buffer>, std::string>
{
    std::unique_ptr<DX12Buffer> buffer(new DX12Buffer);
    buffer->m_config = config;
    return buffer;
}

DX12Buffer::~DX12Buffer()
{
}

}
