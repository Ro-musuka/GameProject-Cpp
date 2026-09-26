#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Animation/Skeleton.h"

class Mesh;
class AnimMesh;
class Texture;
struct Animation;
class Skeleton;
class Shader;
class AssetManager
{
public:
    void Unload();

    Mesh *GetMesh(const std::string &name) const;
    void SetMesh(const std::string &name, Mesh *mesh);

    AnimMesh *GetAnimMesh(const std::string &name) const;
    void SetAnimMesh(const std::string &name, AnimMesh *animMesh);

    Texture *GetTexture(const std::string &name);
    void SetTexture(const std::string &name, Texture *tex);

    std::vector<Animation *> *GetAnimations(const std::string &name);
    void SetAnimations(const std::string &name, const std::vector<Animation *> &anims);

    Skeleton GetSkeleton(const std::string &name) const;
    void SetSkeleton(const std::string &name, Skeleton skeleton);

    Shader &GetShader(const std::string &name);
    void SetShader(const std::string &name, Shader &shader);

private:
    std::unordered_map<std::string, Mesh *> meshes;
    std::unordered_map<std::string, AnimMesh *> animMeshes;
    std::unordered_map<std::string, Texture *> textures;

    std::unordered_map<std::string, std::vector<Animation *>> animations;
    std::unordered_map<std::string, Skeleton> skeletons;

    std::unordered_map<std::string, Shader> shaders;
};
