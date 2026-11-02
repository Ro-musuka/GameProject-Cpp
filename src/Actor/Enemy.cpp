#include "Enemy.h"

#include <glm/gtx/quaternion.hpp>

#include "Core/Game.h"

#include "Component/MeshComp/AnimMeshComponent.h"
#include "Actor/Player.h"

Enemy::Enemy(Game *game)
    : Actor(game),
      mAnimMeshComp(nullptr),
      mEnemyState(EIdle),
      mStateTimer(0.0f),
      mIsRotating(false),
      mRotateTimer(0.0f),
      mAttackDamage(10.0f),
      mAttackRange(5.0f),
      mAttackRadius(10.0f),
      mIsAttackActive(false),
      gravityDown(-10.0f)
{
}

void Enemy::Initialize()
{
    mAnimMeshComp = FindComponent<AnimMeshComponent>();
    // 発見処理を作ったらEIdelに変更
    ChangeState(EnemyState::ERecover);
}

void Enemy::UpdateActor(float deltaTime)
{
    mVelocity.x = mVelocity.z = 0.0f;
    mStateTimer += deltaTime;

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
    RotateTowardPlayer(deltaTime);

    // 攻撃判定の有効時間
    const float attackStart = 0.3f;
    const float attackEnd = 0.6f;

    if (mStateTimer >= attackStart &&
        mStateTimer <= attackEnd)
    {
        mIsAttackActive = true;
        CheckAttackHit();
    }
    else
    {
        mIsAttackActive = false;
        mHasDamagedPlayer = false;
    }

    // アニメーション終了
    if (!mAnimMeshComp->IsPlaying())
    {
        mIsAttackActive = false;
        ChangeState(EnemyState::ERecover);
    }
}

void Enemy::UpdateRecover(float deltaTime)
{
    // 待機中はプレイヤー方向に回転しつつ、1.8秒待機
    RotateTowardPlayer(deltaTime);
    if (mStateTimer <= 1.8f)
    {
        return;
    }

    glm::vec3 diff = GetGame()->GetPlayer()->GetPosition() - GetPosition();
    // 20.0f圏内なら、攻撃
    if (glm::length2(diff) <= (20.0f * 20.0f))
    {
        ChangeState(EnemyState::EAttack);
    }
    // 離れていて、かつ見失ってないならチェイス
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
        // 回転に1秒かかる場合
        float t = glm::clamp(mRotateTimer / 1.0f, 0.0f, 1.0f);

        // 回転を補間
        glm::quat rotate = glm::slerp(mStartRotation, mTargetRotation, t);
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
    glm::vec3 forward = glm::normalize(diff);

    // 現在の向いている方向
    glm::vec3 currentForward = GetForward();
    currentForward.y = 0.0f;
    currentForward = glm::normalize(currentForward);

    // 二つの方向が平行なら回転しない
    // 平行なベクトル同士の内積は, 1.0
    float dot = glm::dot(currentForward, forward);
    if (dot >= 0.9f)
    {
        return;
    }

    // 目標までのYaw角
    float yaw = glm::atan(diff.x, diff.z);

    mIsRotating = true;
    mRotateTimer = 0.0f;
    mStartRotation = GetRotation();
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
        mAnimMeshComp->Play("Running");
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

void Enemy::CheckAttackHit()
{
    if (!mIsAttackActive || mHasDamagedPlayer)
    {
        return;
    }
    Player *player = GetGame()->GetPlayer();

    // 攻撃判定の中心
    glm::vec3 attackCenter = GetPosition() + GetForward() * mAttackRange;

    // プレイヤーとの距離
    glm::vec3 diff = player->GetPosition() - attackCenter;
    diff.y = 0.0f;

    // 球形の攻撃判定
    const float hitRadius = mAttackRadius + 0.5f;

    if (glm::length2(diff) <= (hitRadius * hitRadius))
    {
        player->TakeDamage(mAttackDamage);
        mHasDamagedPlayer = true;
    }
}