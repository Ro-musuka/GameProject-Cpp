#include "Player.h"

#include <glm/glm.hpp>

#include "Core/Game.h"
#include "Core/Common.h"

#include "Component/MeshComp/AnimMeshComponent.h"

Player::Player(Game *game)
    : Actor(game),
      mVelocity(Common::ZeroVec3),
      gravityUp(-13.0f),
      gravityDown(-10.0f),
      mHP(100)
{
    GetGame()->SetPlayer(this);
}

void Player::Initialize()
{
    mAnimMeshComp = FindComponent<AnimMeshComponent>();
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
    // glm::vec3 pos = GetPosition();
    // pos += mVelocity * deltaTime;
    // SetPosition(pos);
}

void Player::ActorInput(const bool *keystate)
{
    if (mPlayerState == PlayerState::EDead && !mAnimMeshComp->IsPlaying())
    {
        SetState(ActorState::EDead);
    }

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

    if (keystate[SDL_SCANCODE_SPACE] && mPlayerState != PlayerState::EJumping)
    {
        mVelocity.y = 15.0f;
        mAnimMeshComp->Play("Jumping");
        mAnimMeshComp->SetLoop(false);
        mPlayerState = PlayerState::EJumping;
    }

    if (keystate[SDL_SCANCODE_P] && mPlayerState != PlayerState::EPunching)
    {
        mAnimMeshComp->Play("Punching");
        mAnimMeshComp->SetLoop(false);
        mPlayerState = PlayerState::EPunching;
    }

    if (mAnimMeshComp->IsPlaying() && !mAnimMeshComp->IsLoop())
    {
        return;
    }

    if (Common::NearZero(mVelocity.x) && Common::NearZero(mVelocity.z))
    {
        if (mPlayerState != PlayerState::EIdle)
        {
            mAnimMeshComp->Play("Idle");
            mAnimMeshComp->SetLoop(true);
            mPlayerState = PlayerState::EIdle;
        }
    }
    else if (mPlayerState != PlayerState::EWalking)
    {
        mAnimMeshComp->Play("Walking");
        mAnimMeshComp->SetLoop(true);
        mPlayerState = PlayerState::EWalking;
    }
}

void Player::TakeDamage(float damage)
{
    if (mHP <= 0.0f)
    {
        return;
    }

    if (mHP <= damage)
    {
        mHP = 0;
    }
    else
    {
        mHP -= damage;
    }

    if (mHP <= 0.0f)
    {
        mPlayerState = PlayerState::EDead;
        mAnimMeshComp->Play("Death");
        mAnimMeshComp->SetLoop(false);
    }
}