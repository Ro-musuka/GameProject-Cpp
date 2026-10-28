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
        EChasing,
        EFighting,
        EIdle
    };

    Enemy(Game *game);

    void Initialize() override;

    void UpdateActor(float deltaTime) override;

    void ChasePlayer(float deltaTime);
    void RotateTowardPlayer();
    void ObstacleDetection();

    // ゲッター/セッター
    const char *GetTypeName() const override { return "Enemy"; }

    void SetAnimMeshComp(AnimMeshComponent *animMeshComp) { mAnimMeshComp = animMeshComp; }

private:
    AnimMeshComponent *mAnimMeshComp;
    EnemyState mEnemyState;

    bool mIsRotating;
    float mRotateTimer;
    glm::quat mTargetRotation;

    glm::vec3 mVelocity;
    const float gravityUp;
    const float gravityDown;
};