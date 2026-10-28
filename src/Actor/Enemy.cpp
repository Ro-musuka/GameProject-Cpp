#include "Enemy.h"

#include <glm/gtx/quaternion.hpp>

#include "Core/Game.h"

#include "Component/MeshComp/AnimMeshComponent.h"
#include "Actor/Player.h"

Enemy::Enemy(Game *game)
    : Actor(game),
      mAnimMeshComp(nullptr),
      mEnemyState(EIdle),
      mIsRotating(false),
      mRotateTimer(0.0f),
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
    mVelocity = Common::ZeroVec3;

    if (mEnemyState == EChasing)
    {
        ChasePlayer(deltaTime);
    }
    else if (mEnemyState == EFighting)
    {
        RotateTowardPlayer();
    }
    else if (mEnemyState == EIdle)
    {
    }

    // 回転
    if (mIsRotating)
    {
        mRotateTimer += deltaTime;
        // 回転に2秒かかる場合
        float t = glm::clamp(mRotateTimer / 2.0f, 0.0f, 1.0f);

        // 回転を補間
        glm::quat rotate = glm::slerp(GetRotation(), mTargetRotation, t);
        SetRotation(rotate);

        if (t == 1.0f)
        {
            mIsRotating = false;
            mRotateTimer = 0.0f;
        }
    }

    // 移動
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

void Enemy::ChasePlayer(float deltaTime)
{
    // 敵とプレイヤーの距離を計算
    glm::vec3 diff = GetGame()->GetPlayer()->GetPosition() - GetPosition();
    diff.y = 0.0f;
    glm::vec3 forward = glm::normalize(diff);

    mVelocity += forward * 10.0f;
}

void Enemy::RotateTowardPlayer()
{
    // 敵とプレイヤーの距離を計算
    glm::vec3 diff = GetGame()->GetPlayer()->GetPosition() - GetPosition();
    diff.y = 0.0f;
    if (Common::NearZero(diff))
    {
        return;
    }

    // プレイヤーへの方向
    glm::vec3 forward = glm::normalize(diff);
    // 現在の向いている方向
    glm::vec3 currentForward = GetForward();
    currentForward.y = 0.0f;

    // 二つの方向が平行なら回転しない
    // 平行なベクトル同士の内積は, 1.0
    float dot = glm::dot(currentForward, forward);
    if (Common::NearZero(1 - dot))
    {
        return;
    }

    // 目標までのYaw角
    float yaw = glm::atan(forward.x, forward.z);

    mIsRotating = true;
    mRotateTimer = 0.0f;
    mTargetRotation = glm::angleAxis(yaw, Common::UnitY);
}

void Enemy::ObstacleDetection()
{
}