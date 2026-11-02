#pragma once

#include "Actor/Actor.h"

class AnimMeshComponent;
class Game;

class Player : public Actor
{
public:
    enum PlayerState
    {
        EWalking,
        ERunning,
        EPunching,
        EJumping,
        EDead,
        EIdle
    };

    Player(Game *game);

    void Initialize() override;

    void UpdateActor(float deltaTime) override;
    void ActorInput(const bool *keystate) override;

    void TakeDamage(float damage);

    // ゲッター/セッター
    const char *GetTypeName() const override { return "Player"; }

    PlayerState GetPlayerState() const { return mPlayerState; }
    void SetPlayerState(PlayerState state) { mPlayerState = state; }

    void SetAnimMeshComp(AnimMeshComponent *animMeshComp) { mAnimMeshComp = animMeshComp; }

private:
    AnimMeshComponent *mAnimMeshComp;
    PlayerState mPlayerState;

    float mHP;

    glm::vec3 mVelocity;
    const float gravityUp;
    const float gravityDown;
};