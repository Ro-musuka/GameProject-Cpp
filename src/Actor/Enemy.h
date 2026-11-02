#pragma once

#include "Actor.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Game;
class AnimMeshComponent;

class Enemy : public Actor
{
public:
    enum EnemyState
    {
        EIdle,
        EChase,
        EAttack,
        ERecover,
        EHit,
        EDead
    };

    Enemy(Game *game);

    void Initialize() override;

    void UpdateActor(float deltaTime) override;

    // ゲッター/セッター
    const char *GetTypeName() const override { return "Enemy"; }

    void SetAnimMeshComp(AnimMeshComponent *animMeshComp) { mAnimMeshComp = animMeshComp; }

private:
    void UpdateIdle(float deltaTime);
    void UpdateChase(float deltaTime);
    void UpdateAttack(float deltaTime);
    void UpdateRecover(float deltaTime);
    void RotateTowardPlayer(float deltaTime);
    void ChangeState(EnemyState newState);
    void CheckAttackHit();

    // アニメーション
    AnimMeshComponent *mAnimMeshComp;
    // 敵の状態
    EnemyState mEnemyState;
    float mStateTimer;

    // 回転
    bool mIsRotating;
    float mRotateTimer;
    glm::quat mStartRotation;
    glm::quat mTargetRotation;

    // 速度と重力
    glm::vec3 mVelocity;
    const float gravityDown;
};