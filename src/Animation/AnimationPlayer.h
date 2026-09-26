#pragma once

#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Skeleton.h"

class Skeleton;
struct Animation;
struct KeyPosition;
struct KeyRotation;
struct KeyScale;

class AnimationPlayer
{
public:
    AnimationPlayer(class Skeleton skeleton, const std::vector<Animation *> &animations);

    void Play(const std::string &animName);
    void Stop();

    void Update(float deltaTime);

    // ゲッター/セッター
    void SetLoop(bool loop) { mLoop = loop; }
    void SetPlaySpeed(float playSpeed) { mPlaySpeed = playSpeed; }

    bool IsPlaying() const { return mPlaying; }

    float GetCurrentTime() const { return mCurrentTime; }
    float GetDuration() const;

    const std::vector<glm::mat4> &GetBoneMatrices() const;

private:
    void UpdateBone(const std::string &boneName);

    glm::vec3 InterpolatePosition(const std::vector<KeyPosition> &positions) const;
    glm::quat InterpolateRotation(const std::vector<KeyRotation> &rotations) const;
    glm::vec3 InterpolateScale(const std::vector<KeyScale> &scales) const;

    size_t GetPositionIndex(const std::vector<KeyPosition> &positions) const;
    size_t GetRotationIndex(const std::vector<KeyRotation> &rotations) const;
    size_t GetScaleIndex(const std::vector<KeyScale> &scales) const;

    Skeleton mSkeleton;
    Animation *mCurrentAnim;
    std::unordered_map<std::string, Animation *> mAnimMap;

    float mCurrentTime;
    float mPlaySpeed;

    bool mPlaying;
    bool mLoop;
};