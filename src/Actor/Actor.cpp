#include "Actor.h"

#include <algorithm>

#include <glm/gtx/quaternion.hpp>

#include "Core/Game.h"
#include "Component/Component.h"

Actor::Actor(Game *game)
    : mGame(game),
      mPosition(Common::ZeroVec3),
      mRotation(Common::UnitQuat),
      mScale(glm::vec3(1.0f)),
      mWorldTransform(1.0f),
      mRecomputeWorldTransform(true)
{
    mGame->AddActor(this);
}

Actor::~Actor()
{
    mGame->RemoveActor(this);

    while (!mComponents.empty())
    {
        delete mComponents.back();
    }
}

void Actor::Update(float deltaTime)
{
    UpdateComponent(deltaTime);
    UpdateActor(deltaTime);
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::UpdateComponent(float deltaTime)
{
    for (auto comp : mComponents)
    {
        comp->Update(deltaTime);
    }
}

void Actor::ProcessInput(const bool *keystate)
{
    for (auto comp : mComponents)
    {
        comp->ProcessInput(keystate);
    }

    ActorInput(keystate);
}

void Actor::ActorInput(const bool *keystate)
{
}

void Actor::ComputeWorldTransform()
{
    if (mRecomputeWorldTransform)
    {
        mRecomputeWorldTransform = false;
        glm::mat4 model(1.0f);
        // 位置、回転、拡大
        model = glm::translate(model, mPosition);
        model *= glm::toMat4(mRotation);
        model = glm::scale(model, mScale);

        mWorldTransform = model;

        for (auto comp : mComponents)
        {
            comp->OnUpdateWorldTransform();
        }
    }
}

void Actor::AddComponent(Component *comp)
{
    // 更新順序にソートしておく
    int myOrder = comp->GetUpdateOrder();
    auto iter = mComponents.begin();
    for (; iter != mComponents.end(); iter++)
    {
        if (myOrder < (*iter)->GetUpdateOrder())
        {
            break;
        }
    }
    mComponents.insert(iter, comp);
}

void Actor::RemoveComponent(Component *comp)
{
    auto iter = std::find(mComponents.begin(), mComponents.end(), comp);
    if (iter != mComponents.end())
    {
        // 更新順序を守るため, erase()
        mComponents.erase(iter);
    }
}