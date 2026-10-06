#include "PlayerBoxComponent.h"

#include <glm/glm.hpp>

#include "../Actor/Actor.h"

PlayerBoxComponent::PlayerBoxComponent(Actor *owner)
    : BoxComponent(owner)
{
}

void PlayerBoxComponent::OnCollision(BoxComponent *otherBC)
{
    switch (otherBC->GetCollisionType())
    {
    case BoxComponent::CollisionType::EStatic:
        ResolveStaticCollision(otherBC);
        break;

    default:
        break;
    }
}