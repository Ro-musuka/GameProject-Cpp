#pragma once

#include "Component/Component.h"

#include <glm/glm.hpp>

class MoveComponent : public Component
{
public:
    MoveComponent(class Actor *owner, int updateOrder = 10);
    void Update(float deltaTime) override;

    // ゲッター/セッター
    const char *GetTypeName() const override { return "MoveComponent"; }

    void AddVelocity(const glm::vec3 &vel) { mVelocity += vel; }
    void SetVelocity(const glm::vec3 &vel) { mVelocity = vel; }
    const glm::vec3 &GetVelocity() const { return mVelocity; }

    void SetSpeed(float speed) { mSpeed = speed; }
    float GetSpeed() const { return mSpeed; }

    void SetHasGravity(bool hasGravity) { mHasGravity = hasGravity; }
    bool GetHasGravity() const { return mHasGravity; }

private:
    glm::vec3 mVelocity;
    float mSpeed;

    // 重力
    bool mHasGravity;
    float mGravitySpeed;
};
