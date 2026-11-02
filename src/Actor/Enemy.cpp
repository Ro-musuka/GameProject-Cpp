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
      mStateTimer(0.0f),
      gravityDown(-10.0f)
{
}

void Enemy::Initialize()
{
    mAnimMeshComp = FindComponent<AnimMeshComponent>();
    ChangeState(EnemyState::EIdle);

    // デバック用
    ChangeState(EnemyState::EChase);
}

void Enemy::UpdateActor(float deltaTime)
{
    mVelocity.x = mVelocity.z = 0.0f;
    mStateTimer += deltaTime;

    RotateTowardPlayer(deltaTime);
    switch (mEnemyState)
    {
    case EnemyState::EIdle:
        UpdateIdle(deltaTime);
        break;

    case EnemyState::EChase:
        UpdateChase(deltaTime);
        break;

    case EnemyState::EAttack:
        UpdateAttack(deltaTime);
        break;

    case EnemyState::ERecover:
        UpdateRecover(deltaTime);
        break;

    case EnemyState::EHit:
        // 被弾処理
        break;

    case EnemyState::EDead:
        // 死亡処理
        break;
    }

    // 移動
    mVelocity.y += gravityDown * deltaTime;
    if (mVelocity.y <= -15.0f)
    {
        mVelocity.y = -15.0f;
    }

    glm::vec3 pos = GetPosition();
    pos += mVelocity * deltaTime;
    SetPosition(pos);
}

void Enemy::UpdateIdle(float deltaTime)
{
}

void Enemy::UpdateChase(float deltaTime)
{
    // 敵とプレイヤーの距離を計算
    glm::vec3 diff = GetGame()->GetPlayer()->GetPosition() - GetPosition();
    diff.y = 0.0f;
    // diff <= 10.0f
    if (glm::length2(diff) <= (100.0f))
    {
        ChangeState(ERecover);
        return;
    }

    if (glm::length2(diff) >= (100.0f * 100.0f))
    {
        ChangeState(EIdle);
        return;
    }

    const float chaseSpeed = 10.0f;
    glm::vec3 forward = glm::normalize(diff);
    mVelocity += forward * chaseSpeed;
}

void Enemy::UpdateAttack(float deltaTime)
{
    if (!mAnimMeshComp->IsPlaying())
    {
        ChangeState(EnemyState::ERecover);
    }

    return;
}

void Enemy::UpdateRecover(float deltaTime)
{
    mStateTimer += deltaTime;
    if (mStateTimer <= 1.8f)
    {
        return;
    }

    glm::vec3 diff = GetGame()->GetPlayer()->GetPosition() - GetPosition();
    // 20.0f圏内なら、回転
    if (glm::length2(diff) <= (20.0f * 20.0f))
    {
        RotateTowardPlayer(deltaTime);
        // 相手方向を向いたら、攻撃
        if (!mIsRotating)
        {
            ChangeState(EnemyState::EAttack);
        }
    }
    // 離れていたて、かつ見失ってないならチェイス
    else if (glm::length2(diff) <= (100.0f * 100.0f))
    {
        ChangeState(EnemyState::EChase);
    }
    else
    {
        ChangeState(EIdle);
        return;
    }
}

void Enemy::RotateTowardPlayer(float deltaTime)
{
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

        return;
    }

    // 敵とプレイヤーの距離を計算
    glm::vec3 diff = GetGame()->GetPlayer()->GetPosition() - GetPosition();
    diff.y = 0.0f;
    if (Common::NearZero(diff))
    {
        return;
    }

    // 現在の向いている方向
    glm::vec3 currentForward = GetForward();
    currentForward.y = 0.0f;

    // 二つの方向が平行なら回転しない
    // 平行なベクトル同士の内積は, 1.0
    float dot = glm::dot(currentForward, diff);
    if (Common::NearZero(1 - dot))
    {
        return;
    }

    // 目標までのYaw角
    float yaw = glm::atan(diff.x, diff.z);

    mIsRotating = true;
    mRotateTimer = 0.0f;
    mTargetRotation = glm::angleAxis(yaw, Common::UnitY);
}

void Enemy::ChangeState(EnemyState newState)
{
    if (mEnemyState == newState)
    {
        return;
    }

    mEnemyState = newState;
    mStateTimer = 0.0f;

    switch (mEnemyState)
    {
    case EnemyState::EIdle:
        mAnimMeshComp->Play("Idle");
        mAnimMeshComp->SetLoop(true);
        break;

    case EnemyState::EChase:
        mAnimMeshComp->Play("ERunning");
        mAnimMeshComp->SetLoop(true);
        break;

    case EnemyState::EAttack:
        mAnimMeshComp->Play("Punching");
        mAnimMeshComp->SetLoop(false);
        break;

    case EnemyState::ERecover:
        mAnimMeshComp->Play("Idle");
        mAnimMeshComp->SetLoop(true);
        break;

    case EnemyState::EHit:
        mAnimMeshComp->Play("Hit");
        mAnimMeshComp->SetLoop(false);
        break;

    case EnemyState::EDead:
        mAnimMeshComp->Play("Death");
        mAnimMeshComp->SetLoop(false);
        break;
    }
}