/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <fstream>

#include <LibCore/Utilities.h>

namespace Core::Utilities {

auto read_file_to_string(std::filesystem::path const& path) -> std::expected<std::string, std::string>
{
    if (!std::filesystem::exists(path)) {
        return std::unexpected(std::format("File does not exist: {}", path.string()));
    }

    if (!std::filesystem::is_regular_file(path)) {
        return std::unexpected(std::format("Path is not a regular file: {}", path.string()));
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        return std::unexpected(std::format("Failed to open file: {}", path.string()));
    }

    return std::string(std::istreambuf_iterator<char>(file), {});
}

auto read_file_lines(std::filesystem::path const& path) -> std::expected<std::vector<std::string>, std::string>
{
    if (!std::filesystem::exists(path)) {
        return std::unexpected(std::format("File does not exist: {}", path.string()));
    }

    if (!std::filesystem::is_regular_file(path)) {
        return std::unexpected(std::format("Path is not a regular file: {}", path.string()));
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        return std::unexpected(std::format("Failed to open file: {}", path.string()));
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

}
