#include "FollowCameraComponent.h"

#include <SDL3/SDL.h>

#include "../Actor/Actor.h"

FollowCameraComponent::FollowCameraComponent(Actor *owner)
    : Component(owner)
{
}

void FollowCameraComponent::Update(float deltaTime)
{
    mCameraPos = mOwner->GetPosition() + glm::vec3{.0f, 30.f, 30.f};
    mCameraPos += glm::vec3{mFrontSpeed * deltaTime, .0f, mLeftSpeed * deltaTime};
}

void FollowCameraComponent::ProcessInput(const bool *keystate)
{
    mFrontSpeed = 0.0f;
    mLeftSpeed = 0.0f;

    if (keystate[SDL_SCANCODE_W])
    {
        mFrontSpeed += 50.0f;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        mFrontSpeed -= 50.0f;
    }

    if (keystate[SDL_SCANCODE_A])
    {
        mLeftSpeed += 50.0f;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        mLeftSpeed -= 50.0f;
    }
}