#include "Skeleton.h"

#include <SDL3/SDL.h>

Skeleton::~Skeleton()
{
    mBones.clear();
}

void Skeleton::Update()
{
    // アニメーションによる移動をなくす
    mBones.begin()[mRootID].localTransform = glm::mat4(1.0f);

    UpdateBone(mBones[mRootID]);

    for (const Bone &bone : mBones)
    {
        mBoneMatrices[bone.ID] = bone.globalTransform * bone.offset;
    }
}

void Skeleton::UpdateBone(Bone &bone, const glm::mat4 &parentTransform)
{
    bone.globalTransform = parentTransform * bone.localTransform;

    for (uint32_t childID : bone.childrenIDs)
    {
        UpdateBone(mBones[childID], bone.globalTransform);
    }
}

void Skeleton::ResetlocalTransform()
{
    for (Bone &bone : mBones)
    {
        bone.localTransform = bone.bindLocalTransform;
    }
}

void Skeleton::ResizeBoneMatrices()
{
    mBoneMatrices.resize(mBones.size(), glm::mat4(1.0f));
}

void Skeleton::AddBone(Bone &bone)
{
    // 登録済みのボーンは追加しない。
    auto it = mBoneMap.find(bone.name);
    if (it != mBoneMap.end())
    {
        return;
    }

    // 要素番号がidとなる。
    uint32_t id = static_cast<uint32_t>(mBones.size());
    bone.ID = id;

    mBones.push_back(bone);
    mBoneMap[bone.name] = id;
}

Bone *Skeleton::FindBone(const std::string &name)
{
    auto it = mBoneMap.find(name);
    if (it == mBoneMap.end())
    {
        return nullptr;
    }

    return &mBones[it->second];
}

Bone *Skeleton::FindBone(uint32_t boneID)
{
    if (boneID >= mBones.size())
    {
        return nullptr;
    }
    return &mBones[boneID];
}