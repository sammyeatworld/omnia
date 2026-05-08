/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Common/File.h>
#include <LibAsset/ShaderCompiler.h>
#include <LibAsset/ShaderImporter.h>

namespace Asset {

auto ShaderImporter::import(std::filesystem::path const& path) -> std::expected<ShaderData, std::string>
{
    if (!std::filesystem::exists(path)) {
        return std::unexpected(std::format("Shader file '{}' does not exist", path.string()));
    }

    auto extension = path.extension().string();
    auto supported_extensions = ShaderImporter::supported_extensions();
    if (std::ranges::find(supported_extensions.begin(), supported_extensions.end(), extension) == supported_extensions.end()) {
        return std::unexpected(std::format("Unsupported shader file extension '{}'", extension));
    }

    auto file_name = path.stem().string();
    auto shader_stage_string = std::string_view(file_name).substr(file_name.find_last_of('.') + 1);
    Graphics::ShaderStage shader_stage {};
    if (shader_stage_string == "vs") {
        shader_stage = Graphics::ShaderStage::Vertex;
    } else if (shader_stage_string == "fs") {
        shader_stage = Graphics::ShaderStage::Fragment;
    } else {
        return std::unexpected(std::format("Unsupported shader stage '{}'", shader_stage_string));
    }

    ShaderData shader_data;
    shader_data.stage = shader_stage;

    auto file_content = File::read_all(path);
    if (!file_content) {
        return std::unexpected(file_content.error());
    }

    {
        Graphics::ShaderVariant spirv_variant;
        spirv_variant.format = Graphics::ShaderFormat::SPIRV;

        auto compiled_spirv = ShaderCompiler::compile_spirv(path, file_content.value(), shader_stage);
        if (!compiled_spirv) {
            return std::unexpected(compiled_spirv.error());
        }

        spirv_variant.bytecode = std::move(compiled_spirv.value());
        shader_data.variants.push_back(spirv_variant);
    }

    return shader_data;
}

auto ShaderImporter::supported_extensions() -> std::vector<std::string>
{
    return { ".glsl" };
}

}
