#include "DebagCamera.h"

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Core/Game.h"
#include "Core/Common.h"

#include "Graphics/Renderer.h"

DebagCamera::DebagCamera(Game *game)
    : Actor(game),
      mRotateRightSpeed(0.0f),
      mRotateUpSpeed(0.0f),
      mVelocity(Common::ZeroVec3)
{
    GetGame()->GetRenderer()->SetDebagCamera(this);
}

void DebagCamera::UpdateActor(float deltaTime)
{
    glm::vec3 pos = GetPosition();
    pos += mVelocity * deltaTime;
    SetPosition(pos);

    glm::quat rotate = GetRotation();
    rotate = glm::angleAxis(mRotateRightSpeed * deltaTime, Common::UnitY) * rotate;
    rotate = glm::angleAxis(mRotateUpSpeed * deltaTime, Common::UnitX) * rotate;
    SetRotation(rotate);
}

void DebagCamera::ActorInput(const bool *keystate)
{
    mVelocity = Common::ZeroVec3;

    if (keystate[SDL_SCANCODE_W])
    {
        mVelocity += GetForward() * 30.0f;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        mVelocity -= GetForward() * 30.0f;
    }

    if (keystate[SDL_SCANCODE_A])
    {
        mVelocity += GetRight() * 30.0f;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        mVelocity -= GetRight() * 30.0f;
    }

    mRotateRightSpeed = 0.0f;
    mRotateUpSpeed = 0.0f;
    if (keystate[SDL_SCANCODE_L])
    {
        mRotateRightSpeed += glm::radians(45.0f);
    }
    if (keystate[SDL_SCANCODE_J])
    {
        mRotateRightSpeed -= glm::radians(45.0f);
    }
    if (keystate[SDL_SCANCODE_I])
    {
        mRotateUpSpeed += glm::radians(45.0f);
    }
    if (keystate[SDL_SCANCODE_K])
    {
        mRotateUpSpeed -= glm::radians(45.0f);
    }
}