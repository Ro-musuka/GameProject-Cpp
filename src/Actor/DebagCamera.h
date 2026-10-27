#pragma once

#include "Actor.h"

class DebagCamera : public Actor
{
public:
    DebagCamera(Game *actor);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const bool *keystate) override;

    // ゲッター/セッター
    const char *GetTypeName() const override { return "DebagCamera"; }

private:
    glm::vec3 mVelocity;

    float mRotateRightSpeed;
    float mRotateUpSpeed;
};