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

    // ボーンのグローバル変換行列を計算し、ボーン行列を作成
    void Update();
    // 再帰処理で、親の位置から、子ボーンのグローバル変換行列を計算していく
    void UpdateBone(Bone &bone, const glm::mat4 &parentTransform = glm::mat4(1.0f));

    void AddBone(Bone &bone);

    // 更新ごとにリセット
    void ResetlocalTransform();
    // ボーン設定したのち、設定
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
    // ルートボーンID
    int mRootID = -1;

    std::vector<Bone> mBones;
    std::unordered_map<std::string, uint32_t> mBoneMap;
    std::vector<glm::mat4> mBoneMatrices;
};