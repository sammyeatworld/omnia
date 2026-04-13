/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <expected>
#include <filesystem>
#include <string>

#include <LibCore/Export.h>

namespace Core::Utilities {

auto CORE_API read_file_to_string(std::filesystem::path const& path) -> std::expected<std::string, std::string>;
auto CORE_API read_file_lines(std::filesystem::path const& path) -> std::expected<std::vector<std::string>, std::string>;

}
