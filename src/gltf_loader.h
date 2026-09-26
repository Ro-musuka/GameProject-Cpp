/*
  以前使用していたtinygltfを用いたローダーです、
  アニメーション読み込みの処理の手順が多く、assimpに乗り換えました。
*/

#pragma once

#include <string>

#include <SDL3/SDL.h>

#define TINYGLTF_IMPLEMENTATION
#include "external/tiny_gltf_v3.h"
#include "external/tinygltf_json_c.h"

#include "Graphics/Mesh.h"
#include "Graphics/Vertex.h"

#include "Animation/Skeleton.h"

namespace gltf_loader
{
    int32_t FindAttribute(const tg3_primitive &primitive, const char *name);
    bool ReadFloatData(const tg3_model &model, int32_t accessorIndex, std::vector<float> &outData);
    bool ReadIndices(const tg3_model &model, int32_t accessorIndex, std::vector<uint32_t> &outIndices);

    inline Mesh *Load(const std::string &fileName)
    {
        tg3_model model;
        tg3_parse_options options;
        tg3_error_stack errors;

        tg3_parse_options_init(&options);
        tg3_error_stack_init(&errors);

        const tg3_error_code result = tg3_parse_file(&model, &errors, fileName.c_str(), static_cast<uint32_t>(fileName.size()), &options);

        if (result != TG3_OK)
        {
            for (uint32_t i = 0; i < errors.count; ++i)
            {
                const tg3_error_entry &error = errors.entries[i];
                SDL_Log("[glTF] %s", error.message);
            }

            SDL_Log("%s 3Dモデルの読み込みに失敗しました", fileName.c_str());

            tg3_model_free(&model);
            tg3_error_stack_free(&errors);

            return nullptr;
        }

        const tg3_mesh &mesh = model.meshes[0];

        const tg3_primitive &primitive = mesh.primitives[0];

        const int32_t positionAccessor = FindAttribute(primitive, "POSITION");
        const int32_t normalAccessor = FindAttribute(primitive, "NORMAL");
        const int32_t uvAccessor = FindAttribute(primitive, "TEXCOORD_0");

        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> uvs;
        std::vector<uint32_t> indices;

        if (!ReadFloatData(model, positionAccessor, positions))
        {
            SDL_Log("POSITIONの読み込みに失敗しました");

            tg3_model_free(&model);
            tg3_error_stack_free(&errors);

            return nullptr;
        }

        if (!ReadFloatData(model, normalAccessor, normals))
        {
            SDL_Log("NORMALの読み込みに失敗しました");

            tg3_model_free(&model);
            tg3_error_stack_free(&errors);

            return nullptr;
        }

        if (!ReadFloatData(model, uvAccessor, uvs))
        {
            SDL_Log("TEXCOORD_0の読み込みに失敗しました");

            tg3_model_free(&model);
            tg3_error_stack_free(&errors);

            return nullptr;
        }

        if (!ReadIndices(model, primitive.indices, indices))
        {
            SDL_Log("Indexの読み込みに失敗しました");

            tg3_model_free(&model);
            tg3_error_stack_free(&errors);

            return nullptr;
        }

        const size_t vertexCount = positions.size() / 3;
        std::vector<Vertex> vertices(vertexCount);

        for (size_t i = 0; i < vertexCount; ++i)
        {
            vertices[i].position.x = positions[i * 3 + 0];
            vertices[i].position.y = positions[i * 3 + 1];
            vertices[i].position.z = positions[i * 3 + 2];

            if (normals.size() >= (i + 1) * 3)
            {
                vertices[i].normal.x = normals[i * 3 + 0];
                vertices[i].normal.y = normals[i * 3 + 1];
                vertices[i].normal.z = normals[i * 3 + 2];
            }

            if (uvs.size() >= (i + 1) * 2)
            {
                vertices[i].texCoord.x = uvs[i * 2 + 0];
                vertices[i].texCoord.y = uvs[i * 2 + 1];
            }
        }

        Mesh *m = new Mesh(vertices, indices);

        tg3_model_free(&model);
        tg3_error_stack_free(&errors);

        return m;
    }

    inline int32_t FindAttribute(const tg3_primitive &primitive, const char *name)
    {
        for (uint32_t i = 0; i < primitive.attributes_count; ++i)
        {
            const tg3_str_int_pair &attribute = primitive.attributes[i];
            if (attribute.key.len == std::strlen(name) && std::memcmp(attribute.key.data, name, attribute.key.len) == 0)
            {
                return attribute.value;
            }
        }

        return 0;
    }

    inline bool ReadFloatData(const tg3_model &model, int32_t accessorIndex, std::vector<float> &outData)
    {
        const tg3_accessor &accessor = model.accessors[accessorIndex];
        const tg3_buffer_view &view = model.buffer_views[accessor.buffer_view];
        const tg3_buffer &buffer = model.buffers[view.buffer];

        int32_t components = tg3_num_components(accessor.type);
        int32_t stride = tg3_accessor_byte_stride(&accessor, &view);

        size_t elementSize = static_cast<size_t>(components) * sizeof(float);

        size_t startOffset = static_cast<size_t>(view.byte_offset + accessor.byte_offset);
        size_t lastOffset = startOffset + static_cast<size_t>(accessor.count - 1) * stride;

        outData.resize(static_cast<size_t>(accessor.count) * components);

        for (uint64_t i = 0; i < accessor.count; ++i)
        {
            size_t offset = startOffset + i * static_cast<size_t>(stride);
            const uint8_t *src = buffer.data.data + offset;

            float *dst = outData.data() + static_cast<size_t>(i) * components;

            std::memcpy(dst, src, elementSize);
        }

        return true;
    }

    inline bool ReadIndices(const tg3_model &model, int32_t accessorIndex, std::vector<uint32_t> &outIndices)
    {
        const tg3_accessor &accessor = model.accessors[accessorIndex];
        const tg3_buffer_view &view = model.buffer_views[accessor.buffer_view];
        const tg3_buffer &buffer = model.buffers[view.buffer];

        size_t componentSize = tg3_component_size(accessor.component_type);

        int32_t stride = tg3_accessor_byte_stride(&accessor, &view);

        size_t startOffset = static_cast<size_t>(view.byte_offset + accessor.byte_offset);
        size_t lastOffset = startOffset + (accessor.count - 1) * static_cast<size_t>(stride);

        outIndices.resize(accessor.count);

        const size_t elementSize = tg3_component_size(accessor.component_type);

        for (uint64_t i = 0; i < accessor.count; ++i)
        {
            const size_t offset = startOffset + i * static_cast<size_t>(stride);
            const uint8_t *src = buffer.data.data + offset;

            uint32_t value = 0;
            std::memcpy(&value, src, elementSize);
            outIndices[i] = value;
        }

        return true;
    }
}