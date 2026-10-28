#pragma once

#include "Actor.h"

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
    void SearchPlayer();

    void ChasePlayer();
    void ObstacleDetection();

    // ゲッター/セッター
    const char *GetTypeName() const override { return "Enemy"; }

    void SetAnimMeshComp(AnimMeshComponent *animMeshComp) { mAnimMeshComp = animMeshComp; }

private:
    AnimMeshComponent *mAnimMeshComp;
    EnemyState mEnemyState;

    glm::vec3 mVelocity;
    const float gravityUp;
    const float gravityDown;
};