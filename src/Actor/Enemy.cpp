#include "Enemy.h"

#include <glm/glm.hpp>

#include "Core/Game.h"

#include "Component/MeshComp/AnimMeshComponent.h"
#include "Actor/Player.h"

Enemy::Enemy(Game *game)
    : Actor(game),
      mAnimMeshComp(nullptr),
      mEnemyState(EIdle),
      gravityUp(-13.0f),
      gravityDown(-10.0f)
{
}

void Enemy::Initialize()
{
    mAnimMeshComp = FindComponent<AnimMeshComponent>();
    mAnimMeshComp->Play("Idle");
    mAnimMeshComp->SetLoop(true);

    mEnemyState = EFighting;
}

void Enemy::UpdateActor(float deltaTime)
{
    if (mEnemyState == EChasing)
    {
        SearchPlayer();
    }
    else if (mEnemyState == EFighting)
    {
        ChasePlayer();
    }
    else if (mEnemyState == EIdle)
    {
    }

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

void Enemy::SearchPlayer()
{
}

void Enemy::ChasePlayer()
{
    Player *player = GetGame()->GetPlayer();

    glm::vec3 diff = player->GetPosition() - GetPosition();
    diff.y = 0.0f;

    if (Common::NearZero(diff))
    {
        return;
    }
    glm::vec3 forward = glm::normalize(diff);

    glm::vec3 currentForward = GetForward();
    currentForward.y = 0.0f;

    // 向きがほぼ一致しているなら回転しない
    float dot = glm::dot(currentForward, forward);
    if (Common::NearZero(1 - dot))
    {
        return;
    }

    // 目標のYaw角
    float yaw = glm::atan(forward.x, forward.z);

    // 回転を補間
    SetRotation(glm::angleAxis(yaw, Common::UnitY));
}

void Enemy::ObstacleDetection()
{
}