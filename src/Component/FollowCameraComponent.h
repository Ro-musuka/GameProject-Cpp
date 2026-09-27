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

private:
    glm::vec3 mCameraPos;

    float mFrontSpeed;
    float mLeftSpeed;
};