#pragma once

#include "Actor.h"

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
        EIdle
    };

    Player(Game *game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const bool *keystate) override;

    // ゲッター/セッター
    PlayerState GetPlayerState() const { return mPlayerState; }
    void SetPlayerState(PlayerState state) { mPlayerState = state; }

    void SetAnimMeshComp(AnimMeshComponent *animMeshComp) { mAnimMeshComp = animMeshComp; }

private:
    AnimMeshComponent *mAnimMeshComp;
    PlayerState mPlayerState;

    float mFrontSpeed;
    float mLeftSpeed;
    float mUpSpeed;
    const float gravityUp;
    const float gravityDown;
};