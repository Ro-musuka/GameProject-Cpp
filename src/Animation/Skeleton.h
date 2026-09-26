#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Bone.h"

struct Bone;

class Skeleton
{
public:
    ~Skeleton();

    void Update();
    void UpdateBone(Bone &bone, const glm::mat4 &parentTransform = glm::mat4(1.0f));

    void AddBone(Bone &bone);

    void ResetlocalTransform();
    void ResizeBoneMatrices();

    // ゲッター/セッター
    Bone *FindBone(const std::string &name);
    Bone *FindBone(uint32_t boneID);

    int GetRootID() const { return mRootID; }
    void SetRootID(int rootID) { mRootID = rootID; }

    const std::vector<Bone> &GetBones() const { return mBones; }
    const std::unordered_map<std::string, uint32_t> &GetBoneMap() const { return mBoneMap; }
    const std::vector<glm::mat4> &GetBoneMatrices() const { return mBoneMatrices; }

private:
    int mRootID = -1;

    std::vector<Bone> mBones;
    std::unordered_map<std::string, uint32_t> mBoneMap;
    std::vector<glm::mat4> mBoneMatrices;
};