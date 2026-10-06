#pragma once

#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Component/Component.h"

class Actor;

class FollowCameraComponent : public Component
{
public:
    FollowCameraComponent(Actor *owner);

    void Update(float deltaTime) override;
    void ProcessInput(const bool *keystate) override;

    const glm::vec3 &GetCameraPos() const { return mCameraPos; }
    const glm::vec3 &GetTarget() const { return mTarget; }

private:
    glm::vec3 mCameraPos;
    glm::vec3 mTarget;

    glm::quat mRotation;
    float mRotationSpeed;

    // ターゲットとの距離
    const float mTargetDist;
    // 水平距離
    const float mHorzDist;
    // 垂直距離
    const float mVertDist;

    std::array<bool, 5> mKeyPressed;
};