#include "FollowCameraComponent.h"

#include <SDL3/SDL.h>

#include "../Actor/Actor.h"

FollowCameraComponent::FollowCameraComponent(Actor *owner)
    : Component(owner),
      mCameraPos(mOwner->GetPosition()),
      mTarget{0.0f, 10.0f, 0.0f},
      mAngle(.0f),
      mRadius(30.f),
      mRotationSpeed(.0f)
{
}

void FollowCameraComponent::Update(float deltaTime)
{
    mAngle += mRotationSpeed * deltaTime;
    mCameraPos = mOwner->GetPosition();
    mCameraPos += glm::vec3{mRadius * glm::cos(mAngle), 30.0f, mRadius * glm::sin(mAngle)};
}

void FollowCameraComponent::ProcessInput(const bool *keystate)
{
    mRotationSpeed = 0.0f;
    if (keystate[SDL_SCANCODE_K])
    {
        mRotationSpeed += 5.f;
    }
    if (keystate[SDL_SCANCODE_L])
    {
        mRotationSpeed -= 5.f;
    }
}