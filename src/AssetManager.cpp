#include "AssetManager.h"

#include "Graphics/Mesh.h"
#include "Graphics/AnimMesh.h"

#include "Animation/Animation.h"
#include "Animation/Skeleton.h"

Mesh *AssetManager::GetMesh(const std::string &name) const
{
    auto iter = meshes.find(name);
    if (iter == meshes.end())
    {
        return nullptr;
    }

    return iter->second;
}

void AssetManager::SetMesh(const std::string &name, Mesh *mesh)
{
    meshes[name] = mesh;
}

AnimMesh *AssetManager::GetAnimMesh(const std::string &name) const
{
    auto iter = animMeshes.find(name);
    if (iter == animMeshes.end())
    {
        return nullptr;
    }

    return iter->second;
}

void AssetManager::SetAnimMesh(const std::string &name, AnimMesh *animeMesh)
{
    animMeshes[name] = animeMesh;
}

Texture *AssetManager::GetTexture(const std::string &name)
{
    // 既に読み込まれているか確認
    auto iter = textures.find(name);
    if (iter == textures.end())
    {
        return nullptr;
    }
    return iter->second;

    // // 新しく読み込む
    // Texture *tex = new Texture();

    // if (!tex->Load(path))
    // {
    //     delete tex;
    //     return nullptr;
    // }

    // // キャッシュへ登録
    // textures.emplace(key, tex);

    // return tex;
}

std::vector<Animation *> *AssetManager::GetAnimations(const std::string &name)
{
    auto iter = animations.find(name);
    if (iter == animations.end())
    {
        return nullptr;
    }

    return &(iter->second);
}

void AssetManager::SetAnimations(const std::string &name, const std::vector<Animation *> &anims)
{
    animations[name] = anims;
}

Skeleton AssetManager::GetSkeleton(const std::string &name) const
{
    auto iter = skeletons.find(name);
    // エラー出ても知るか。
    return iter->second;
}

void AssetManager::SetSkeleton(const std::string &name, Skeleton skeleton)
{
    skeletons[name] = skeleton;
}