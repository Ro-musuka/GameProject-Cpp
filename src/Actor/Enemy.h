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

    AnimMeshComponent *mAnimMeshComp;
    EnemyState mEnemyState;

    bool mIsRotating;
    float mRotateTimer;
    glm::quat mTargetRotation;

    float mStateTimer;

    glm::vec3 mVelocity;
    const float gravityDown;
};