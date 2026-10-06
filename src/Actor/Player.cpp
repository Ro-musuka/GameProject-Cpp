#include "Player.h"

#include <glm/glm.hpp>

#include "Core/Game.h"
#include "Core/Common.h"

#include "Component/MeshComp/AnimMeshComponent.h"

Player::Player(Game *game)
    : Actor(game),
      mVelocity(Common::ZeroVec3),
      gravityUp(-13.0f),
      gravityDown(-10.0f)
{
}

void Player::UpdateActor(float deltaTime)
{
    if (mVelocity.y >= 0.0f)
    {
        mVelocity.y += gravityUp * deltaTime;
    }
    else
    {
        mVelocity.y += gravityDown * deltaTime;
    }

    if (mVelocity.y <= -15.0f)
    {
        mVelocity.y = -15.0f;
    }
    glm::vec3 pos = GetPosition();
    pos += mVelocity * deltaTime;
    SetPosition(pos);
}

void Player::ActorInput(const bool *keystate)
{
    mVelocity.x = mVelocity.z = 0.0f;

    if (keystate[SDL_SCANCODE_W])
    {
        mVelocity += GetForward() * 10.0f;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        mVelocity -= GetForward() * 10.0f;
    }

    if (keystate[SDL_SCANCODE_A])
    {
        mVelocity -= GetRight() * 10.0f;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        mVelocity += GetRight() * 10.0f;
    }

    if (keystate[SDL_SCANCODE_SPACE])
    {
        mVelocity.y = 15.0f;
    }

    if (Common::NearZero(mVelocity.x) && Common::NearZero(mVelocity.z))
    {
        if (mPlayerState != EIdle)
        {
            mAnimMeshComp->Play("Idle");
            mPlayerState = EIdle;
        }
    }
    else if (mPlayerState != EWalking)
    {
        mAnimMeshComp->Play("Walking");
        mPlayerState = EWalking;
    }
}