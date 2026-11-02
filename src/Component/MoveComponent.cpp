#include "MoveComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Core/Common.h"

#include "Actor/Actor.h"

MoveComponent::MoveComponent(class Actor *owner, int updateOrder)
    : Component(owner, updateOrder),
      mVelocity(Common::ZeroVec3),
      mSpeed(10.0f),
      mHasGravity(false),
      mGravitySpeed(-10.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
    if (mHasGravity)
    {
        mVelocity.y += mGravitySpeed * deltaTime;
    }

    if (mVelocity.y <= -15.0f)
    {
        mVelocity.y = -15.0f;
    }
}
