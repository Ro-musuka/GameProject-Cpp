#include "Component.h"

#include "Core/Game.h"
#include "Actor/Actor.h"

Component::Component(Actor *owner, int updateOrder)
    : mOwner(owner),
      mUpdateOrder(updateOrder)
{
    mOwner->AddComponent(this);
}

Component::~Component()
{
    mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{
}

void Component::OnUpdateWorldTransform()
{
}

void Component::ProcessInput(const bool *keystate)
{
}
