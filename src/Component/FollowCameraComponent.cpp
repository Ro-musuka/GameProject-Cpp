#include "FollowCameraComponent.h"

#include <SDL3/SDL.h>
#include <glm/gtx/quaternion.hpp>

#include "../Common.h"

#include "../Actor/Actor.h"

FollowCameraComponent::FollowCameraComponent(Actor *owner)
    : Component(owner),
      mCameraPos(mOwner->GetPosition()),
      mRotation(Common::UnitQuat),
      mRotationSpeed(10.0f),
      mHorzDist(30.0f),
      mVertDist(10.0f),
      mTargetDist(100.0f),
      mKeyPressed{}
{
    mTarget = mCameraPos + mOwner->GetForward() * mTargetDist;
}

void FollowCameraComponent::Update(float deltaTime)
{
    mCameraPos = mOwner->GetPosition();
    mCameraPos -= mOwner->GetForward() * mHorzDist;
    mCameraPos += Common::UnitY * mVertDist;

    // Kキーが押されたら、右回転
    if (mKeyPressed[0])
    {
        mRotation *= glm::angleAxis(glm::radians(-30.0f) * deltaTime, Common::UnitY);
    }
    // Lキーが押されたら、左回転
    if (mKeyPressed[1])
    {
        mRotation *= glm::angleAxis(glm::radians(30.0f) * deltaTime, Common::UnitY);
    }
    // キーを離したら、中央へ戻す
    if (mKeyPressed[2])
    {
        mRotation = glm::slerp(mRotation, Common::UnitQuat, mRotationSpeed * deltaTime);
    }

    mTarget = (mOwner->GetPosition() + mOwner->GetForward() * mTargetDist) * mRotation;
}

void FollowCameraComponent::ProcessInput(const bool *keystate)
{
    for (size_t i = 0; i < mKeyPressed.size(); i++)
    {
        mKeyPressed[i] = false;
    }

    if (keystate[SDL_SCANCODE_K])
    {
        mKeyPressed[0] = true;
    }
    if (keystate[SDL_SCANCODE_L])
    {
        mKeyPressed[1] = true;
    }
    if (!keystate[SDL_SCANCODE_K] && !keystate[SDL_SCANCODE_L])
    {
        mKeyPressed[2] = true;
    }
}