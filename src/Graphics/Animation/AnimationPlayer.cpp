#include "AnimationPlayer.h"

#include <algorithm>

#include <SDL3/SDL.h>

#include "Graphics/Animation/Animation.h"
#include "Graphics/Animation/Bone.h"

AnimationPlayer::AnimationPlayer(Skeleton skeleton, const std::vector<Animation *> &animations)
    : mSkeleton(skeleton),
      mCurrentTime(0.0f),
      mPlaySpeed(1.0f),
      mPlaying(false),
      mLoop(false)
{
    for (auto &anim : animations)
    {
        mAnimMap[anim->name] = anim;
    }
    mCurrentAnim = animations[0];
}

void AnimationPlayer::Play(const std::string &animName)
{
    auto iter = mAnimMap.find(animName);
    if (iter == mAnimMap.end())
    {
        SDL_Log("Animation not found: %s", animName.c_str());
        return;
    }
    mCurrentAnim = iter->second;
    mCurrentTime = 0.0f;
    mPlaying = true;
}

void AnimationPlayer::Resume()
{
    mPlaying = true;
}

void AnimationPlayer::Stop()
{
    mPlaying = false;
}

void AnimationPlayer::Update(float deltaTime)
{
    if (!mPlaying)
    {
        return;
    }

    mCurrentTime += deltaTime * mPlaySpeed * mCurrentAnim->ticksPerSecond;
    if (mCurrentTime >= mCurrentAnim->duration)
    {
        if (mLoop)
        {
            mCurrentTime = std::fmod(mCurrentTime, mCurrentAnim->duration);
        }
        else
        {
            mCurrentTime = mCurrentAnim->duration;
            mPlaying = false;
        }
    }

    // ローカル変換座標を前ポーンリセット
    mSkeleton.ResetlocalTransform();
    for (auto &[name, bone] : mSkeleton.GetBoneMap())
    {
        UpdateBone(name);
    }
    // オフセットや親ボーンによる変換などで更新
    mSkeleton.Update();
}

void AnimationPlayer::UpdateBone(const std::string &boneName)
{
    auto iter = mCurrentAnim->channels.find(boneName);
    if (iter == mCurrentAnim->channels.end())
    {
        return;
    }

    const BoneAnimation &boneAnim = iter->second;
    glm::vec3 position = InterpolatePosition(boneAnim.positions);
    glm::quat rotation = InterpolateRotation(boneAnim.rotations);
    glm::vec3 scale = InterpolateScale(boneAnim.scales);

    mSkeleton.FindBone(boneName)->localTransform = glm::translate(glm::mat4(1.0f), position) *
                                                   glm::mat4_cast(rotation) *
                                                   glm::scale(glm::mat4(1.0f), scale);
}

glm::vec3 AnimationPlayer::InterpolatePosition(const std::vector<struct KeyPosition> &positions) const
{
    size_t index = GetPositionIndex(positions);

    const KeyPosition &current = positions[index];
    const KeyPosition &next = positions[index + 1];

    float factor = (mCurrentTime - current.time) / (next.time - current.time);

    return glm::mix(current.value, next.value, factor);
}

glm::quat AnimationPlayer::InterpolateRotation(const std::vector<struct KeyRotation> &rotations) const
{
    size_t index = GetRotationIndex(rotations);

    const KeyRotation &current = rotations[index];
    const KeyRotation &next = rotations[index + 1];

    float factor = (mCurrentTime - current.time) / (next.time - current.time);

    return glm::normalize(glm::slerp(current.value, next.value, factor));
}

glm::vec3 AnimationPlayer::InterpolateScale(const std::vector<struct KeyScale> &scales) const
{
    size_t index = GetScaleIndex(scales);

    const KeyScale &current = scales[index];
    const KeyScale &next = scales[index + 1];

    float factor = (mCurrentTime - current.time) / (next.time - current.time);

    return glm::mix(current.value, next.value, factor);
}

size_t AnimationPlayer::GetPositionIndex(const std::vector<KeyPosition> &positions) const
{
    for (size_t i = 0; i < positions.size() - 1; i++)
    {
        if (mCurrentTime < positions[i + 1].time)
        {
            return i;
        }
    }

    return positions.size() - 2;
}

size_t AnimationPlayer::GetRotationIndex(const std::vector<KeyRotation> &rotations) const
{
    for (size_t i = 0; i < rotations.size() - 1; i++)
    {
        if (mCurrentTime < rotations[i + 1].time)
        {
            return i;
        }
    }

    return rotations.size() - 2;
}

size_t AnimationPlayer::GetScaleIndex(const std::vector<KeyScale> &scales) const
{
    for (size_t i = 0; i < scales.size() - 1; i++)
    {
        if (mCurrentTime < scales[i + 1].time)
        {
            return i;
        }
    }

    return scales.size() - 2;
}

float AnimationPlayer::GetDuration() const
{
    return mCurrentAnim->duration;
}

const std::vector<glm::mat4> &AnimationPlayer::GetBoneMatrices() const
{
    return mSkeleton.GetBoneMatrices();
}