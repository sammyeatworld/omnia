/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#include <unordered_map>

#include <Common/File.h>
#include <LibAsset/ModelImporter.h>

namespace Asset {

auto ModelImporter::import(std::filesystem::path const& path, AssetRegistry const& asset_registry) -> std::expected<ModelData, std::string>
{
    if (!std::filesystem::exists(path)) {
        return std::unexpected(std::format("Model file '{}' does not exist", path.string()));
    }

    auto extension = path.extension().string();
    auto supported_extensions = ModelImporter::supported_extensions();
    if (std::ranges::find(supported_extensions.begin(), supported_extensions.end(), extension) == supported_extensions.end()) {
        return std::unexpected(std::format("Unsupported model file extension '{}'", extension));
    }

    if (extension == ".gltf") {
        return import_gltf(path, asset_registry);
    }

    return {};
}

auto ModelImporter::supported_extensions() -> std::vector<std::string>
{
    return { ".gltf" };
}

auto ModelImporter::import_gltf(std::filesystem::path const& path, AssetRegistry const& asset_registry) -> std::expected<ModelData, std::string>
{
    auto file_data = File::read_all(path);
    if (!file_data) {
        return std::unexpected(std::move(file_data).error());
    }

    cgltf_options const options {};
    cgltf_data* data = nullptr;
    if (cgltf_parse(&options, file_data->data(), file_data->size(), &data) != cgltf_result_success) {
        return std::unexpected(std::format("Failed to parse glTF file '{}'", path.string()));
    }
    if (cgltf_load_buffers(&options, data, path.string().c_str()) != cgltf_result_success) {
        cgltf_free(data);
        return std::unexpected(std::format("Failed to load buffers for glTF file '{}'", path.string()));
    }
    if (cgltf_validate(data) != cgltf_result_success) {
        cgltf_free(data);
        return std::unexpected(std::format("Invalid glTF file '{}'", path.string()));
    }

    ModelData model_data;

    for (cgltf_size i = 0; i < data->materials_count; ++i) {
        auto const& gltf_material = data->materials[i];
        auto& material = model_data.materials.emplace_back();
        material.name = gltf_material.name ? gltf_material.name : std::format("Material_{}", i);

        if (gltf_material.has_pbr_metallic_roughness) {
            auto const& pbr = gltf_material.pbr_metallic_roughness;

            if (pbr.base_color_texture.texture != nullptr) {
                auto const* img = pbr.base_color_texture.texture->image;
                auto const texture_path = path.parent_path() / img->uri;
                auto key = asset_registry.resolve_key(texture_path);
                material.albedo_texture_id = asset_registry.key_to_id(key);
                material.base_color = Math::Vec4f(pbr.base_color_factor[0], pbr.base_color_factor[1], pbr.base_color_factor[2], pbr.base_color_factor[3]);
            }
        }
    }

    auto get_material_index = [&](cgltf_material const* gltf_mat) -> u64 {
        if (gltf_mat == nullptr) {
            return 0;
        }

        for (cgltf_size i = 0; i < data->materials_count; ++i) {
            if (&data->materials[i] == gltf_mat) {
                return i;
            }
        }
        return 0;
    };

    for (cgltf_size i = 0; i < data->meshes_count; ++i) {
        auto const& gltf_mesh = data->meshes[i];

        for (cgltf_size j = 0; j < gltf_mesh.primitives_count; ++j) {
            auto const& primitive = gltf_mesh.primitives[j];
            if (primitive.type != cgltf_primitive_type_triangles) {
                continue;
            }

            auto& sub_mesh = model_data.sub_meshes.emplace_back();
            sub_mesh.material_index = get_material_index(primitive.material);

            cgltf_accessor const* position_accessor = nullptr;
            cgltf_accessor const* normal_accessor = nullptr;
            cgltf_accessor const* tex_coord_accessor = nullptr;

            for (cgltf_size k = 0; k < primitive.attributes_count; ++k) {
                auto const& attribute = primitive.attributes[k];
                switch (attribute.type) {
                case cgltf_attribute_type_position:
                    position_accessor = attribute.data;
                    break;
                case cgltf_attribute_type_normal:
                    normal_accessor = attribute.data;
                    break;
                case cgltf_attribute_type_texcoord:
                    tex_coord_accessor = attribute.data;
                    break;
                case cgltf_attribute_type_joints:
                    break;
                case cgltf_attribute_type_weights:
                    break;
                default:
                    break;
                }
            }

            if (position_accessor == nullptr) {
                continue;
            }

            auto vertex_count = position_accessor->count;
            sub_mesh.vertices.reserve(vertex_count);

            for (cgltf_size v = 0; v < vertex_count; ++v) {
                Graphics::Vertex vertex {};

                if (position_accessor) {
                    cgltf_accessor_read_float(position_accessor, v, &vertex.position.x, 3);
                }

                if (normal_accessor) {
                    cgltf_accessor_read_float(normal_accessor, v, &vertex.normal.x, 3);
                }

                if (tex_coord_accessor) {
                    cgltf_accessor_read_float(tex_coord_accessor, v, &vertex.tex_coord.x, 2);
                }

                sub_mesh.vertices.push_back(vertex);
            }

            if (primitive.indices != nullptr) {
                auto index_count = primitive.indices->count;
                sub_mesh.indices.reserve(index_count);

                for (cgltf_size idx = 0; idx < index_count; ++idx) {
                    u32 index_value = 0;
                    cgltf_accessor_read_uint(primitive.indices, idx, &index_value, 1);
                    sub_mesh.indices.push_back(index_value);
                }
            } else {
                sub_mesh.indices.reserve(vertex_count);
                for (u32 idx = 0; idx < vertex_count; ++idx) {
                    sub_mesh.indices.push_back(idx);
                }
            }
        }
    }

    std::erase_if(model_data.sub_meshes, [](auto const& submesh) {
        return submesh.indices.empty();
    });

    cgltf_free(data);
    return model_data;
}

}
