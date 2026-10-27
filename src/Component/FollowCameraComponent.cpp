#include "FollowCameraComponent.h"

#include <SDL3/SDL.h>
#include <glm/gtx/quaternion.hpp>

#include "Core/Common.h"
#include "Core/Game.h"
#include "Actor/Actor.h"
#include "Graphics/Renderer.h"

FollowCameraComponent::FollowCameraComponent(Actor *owner)
    : Component(owner),
      mCameraPos(mOwner->GetPosition()),
      mRotation(Common::UnitQuat),
      mRotationSpeed(2.0f),
      mHorzDist(30.0f),
      mVertDist(0.0f),
      mTargetDist(100.0f),
      mKeyPressed{}
{
    mOwner->GetGame()->GetRenderer()->SetCameraComp(this);
    mTarget = mCameraPos + mOwner->GetForward() * mTargetDist;
}

void FollowCameraComponent::Update(float deltaTime)
{
    mCameraPos = mOwner->GetPosition();
    mCameraPos -= mOwner->GetForward() * mHorzDist;
    mCameraPos += Common::UnitY * mVertDist;

    /*
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
    // Iが押されたら、上へ回転
    if (mKeyPressed[2])
    {
        mRotation *= glm::angleAxis(glm::radians(30.0f) * deltaTime, Common::UnitX);
    }
    // Kが押されたら、下へ回転
    if (mKeyPressed[3])
    {
        mRotation *= glm::angleAxis(glm::radians(-30.0f) * deltaTime, Common::UnitX);
    }
    // キーを離したら、中央へ戻す
    if (mKeyPressed[4])
    {
        mRotation = glm::slerp(mRotation, Common::UnitQuat, mRotationSpeed * deltaTime);
    }
    */

    mTarget = (mOwner->GetPosition() + mOwner->GetForward() * mTargetDist) * mRotation;
}

void FollowCameraComponent::ProcessInput(const bool *keystate)
{
    /*
    for (size_t i = 0; i < mKeyPressed.size(); i++)
    {
        mKeyPressed[i] = false;
    }

    if (keystate[SDL_SCANCODE_J])
    {
        mKeyPressed[0] = true;
    }
    if (keystate[SDL_SCANCODE_L])
    {
        mKeyPressed[1] = true;
    }
    if (keystate[SDL_SCANCODE_I])
    {
        mKeyPressed[2] = true;
    }
    if (keystate[SDL_SCANCODE_K])
    {
        mKeyPressed[3] = true;
    }
    if (!keystate[SDL_SCANCODE_J] && !keystate[SDL_SCANCODE_L] &&
        !keystate[SDL_SCANCODE_I] && !keystate[SDL_SCANCODE_K])
    {
        mKeyPressed[4] = true;
    }
    */

    // マウスに合わせた回転を計算
    const float sensitivity = 0.002f;
    float mouseX = 0.0f;
    float mouseY = 0.0f;
    SDL_GetRelativeMouseState(&mouseX, &mouseY);

    // 上下の回転を計算
    mRotation = glm::angleAxis(-mouseY * sensitivity, Common::UnitX) * mRotation;
    // 左右の回転を計算
    // 回転軸は上下方向に回転させておく
    mRotation = glm::angleAxis(mouseX * sensitivity, mRotation * Common::UnitY) * mRotation;
}