#pragma once

#include <glm/glm.hpp>

#include "Component.h"

class Actor;

class FollowCameraComponent : public Component
{
public:
    FollowCameraComponent(Actor *owner);

    void Update(float deltaTime) override;
    void ProcessInput(const bool *keystate) override;

    const glm::vec3 &GetCameraPos() const { return mCameraPos; }
    const glm::vec3 &GetTargetPos() const { return mTarget; }

private:
    glm::vec3 mCameraPos;
    glm::vec3 mTarget;

    float mAngle;
    float mRadius;
    float mRotationSpeed;
};