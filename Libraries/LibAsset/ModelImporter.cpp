/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <unordered_map>

#include <Common/File.h>
#include <LibAsset/ModelImporter.h>

namespace Asset {

auto ModelImporter::import(std::filesystem::path const& path) -> std::expected<std::any, std::string>
{
    if (!std::filesystem::exists(path)) {
        return std::unexpected(std::format("Model file '{}' does not exist", path.string()));
    }

    auto extension = path.extension().string();
    auto supported_extensions = this->supported_extensions();
    if (std::ranges::find(supported_extensions.begin(), supported_extensions.end(), extension) == supported_extensions.end()) {
        return std::unexpected(std::format("Unsupported model file extension '{}'", extension));
    }

    if (extension == ".obj") {
        return import_obj(path);
    }

    return {};
}

auto ModelImporter::supported_extensions() const -> std::vector<std::string>
{
    return { ".obj" };
}

auto ModelImporter::import_obj(std::filesystem::path const& path) -> std::expected<std::any, std::string>
{
    auto lines = File::read_lines(path);
    if (!lines) {
        return std::unexpected(std::move(lines.error()));
    }

    Graphics::ModelConfiguration model_data;

    std::unordered_map<std::string, u32> vertex_cache;
    std::vector<Math::Vec3f> positions;
    std::vector<Math::Vec2f> tex_coords;
    std::vector<Math::Vec3f> normals;

    Graphics::SubMesh* current_submesh = nullptr;

    for (auto const& line : lines.value()) {
        std::istringstream line_stream(line);
        std::string token;
        line_stream >> token;

        if (token == "mtllib") {

        } else if (token == "usemtl") {

        } else if (token == "o" || token == "g") {
            model_data.sub_meshes.emplace_back();
            current_submesh = &model_data.sub_meshes.back();
        } else if (token == "v") {
            auto& position = positions.emplace_back();
            line_stream >> position.x >> position.y >> position.z;
        } else if (token == "vt") {
            auto& tex_coord = tex_coords.emplace_back();
            line_stream >> tex_coord.x >> tex_coord.y;
        } else if (token == "vn") {
            auto& normal = normals.emplace_back();
            line_stream >> normal.x >> normal.y >> normal.z;
        } else if (token == "f") {
            if (current_submesh == nullptr) {
                model_data.sub_meshes.emplace_back();
                current_submesh = &model_data.sub_meshes.back();
            }

            std::vector<std::string> face_tokens;
            std::string face_token;
            while (line_stream >> face_token) {
                face_tokens.push_back(std::move(face_token));
            }

            for (std::size_t i = 1; i + 1 < face_tokens.size(); ++i) {
                for (std::size_t const j : {0ULL, i, i + 1}) {
                    auto& vertex_token = face_tokens[j];

                    if (auto it = vertex_cache.find(vertex_token); it != vertex_cache.end()) {
                        current_submesh->indices.push_back(it->second);
                        continue;
                    }

                    std::istringstream vertex_stream(vertex_token);

                    Graphics::Vertex vertex {};
                    std::string vertex_index;
                    u32 index_count = 0;

                    while (std::getline(vertex_stream, vertex_index, '/')) {
                        if (!vertex_index.empty()) {
                            auto index_value = std::stoul(vertex_index) - 1;

                            if (index_count == 0 && index_value < positions.size()) {
                                vertex.position = positions[index_value];
                            } else if (index_count == 1 && index_value < tex_coords.size()) {
                                vertex.tex_coord = tex_coords[index_value];
                            } else if (index_count == 2 && index_value < normals.size()) {
                                vertex.normal = normals[index_value];
                            }
                        }
                        ++index_count;
                    }

                    auto index = static_cast<Graphics::Index>(current_submesh->vertices.size());
                    current_submesh->vertices.push_back(vertex);
                    current_submesh->indices.push_back(index);
                    vertex_cache[vertex_token] = index;
                }
            }
        }
    }

    return model_data;
}

}
