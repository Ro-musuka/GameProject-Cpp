#include "Player.h"

#include <glm/glm.hpp>

#include "Core/Game.h"
#include "Core/Common.h"

#include "Component/MeshComp/AnimMeshComponent.h"
#include "Component/MoveComponent.h"

Player::Player(Game *game)
    : Actor(game),
      mHP(100),
      Up(5.0f)
{
    GetGame()->SetPlayer(this);
}

void Player::Initialize()
{
    mAnimMeshComp = FindComponent<AnimMeshComponent>();
    mMoveComponent = FindComponent<MoveComponent>();
    mMoveComponent->SetSpeed(10.0f);
}

void Player::UpdateActor(float deltaTime)
{
    // ジャンプ中は、落下を遅くする
    glm::vec3 vel = mMoveComponent->GetVelocity();
    if (vel.y >= 0.0f)
    {
        vel.y += Up * deltaTime;
    }

    glm::vec3 pos = GetPosition();
    pos += vel * deltaTime;
    SetPosition(pos);
}

void Player::ActorInput(const bool *keystate)
{
    if (mPlayerState == PlayerState::EDead && !mAnimMeshComp->IsPlaying())
    {
        SetState(ActorState::EDead);
    }

    glm::vec3 vel = mMoveComponent->GetVelocity();
    vel.x = vel.y = 0.0f;
    float speed = mMoveComponent->GetSpeed();

    if (keystate[SDL_SCANCODE_W])
    {
        vel += GetForward() * speed;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        vel -= GetForward() * speed;
    }

    if (keystate[SDL_SCANCODE_A])
    {
        vel -= GetRight() * speed;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        vel += GetRight() * speed;
    }

    if (keystate[SDL_SCANCODE_SPACE] && mPlayerState != PlayerState::EJumping)
    {
        vel.y = 15.0f;
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

    // ループのないアニメを再生しているなら、終了
    if (mAnimMeshComp->IsPlaying() && !mAnimMeshComp->IsLoop())
    {
        return;
    }

    if (Common::NearZero(vel.x) && Common::NearZero(vel.z))
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