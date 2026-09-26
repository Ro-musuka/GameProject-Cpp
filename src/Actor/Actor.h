#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

#include "../Common.h"

class Actor
{
public:
    Actor(class Game *game);
    virtual ~Actor();

    void Update(float deltaTime);
    virtual void UpdateActor(float deltaTime);
    void UpdateComponent(float deltaTime);

    void ProcessInput(const bool *keystate);
    virtual void ActorInput(const bool *keystate);

    void AddComponent(class Component *component);
    void RemoveComponent(class Component *component);

    const glm::vec3 &GetPosition() const { return mPosition; }
    void SetPosition(const glm::vec3 &pos)
    {
        mPosition = pos;
        mRecomputeWorldTransform = true;
    }
    const glm::vec3 &GetScale() const { return mScale; }
    void SetScale(const glm::vec3 &scale)
    {
        mScale = scale;
        mRecomputeWorldTransform = true;
    }
    const glm::quat &GetRotation() const { return mRotation; }
    void SetRotation(const glm::quat &rotation)
    {
        mRotation = rotation;
        mRecomputeWorldTransform = true;
    }

    void ComputeWorldTransform();
    const glm::mat4 &GetWorldTransform() const { return mWorldTransform; }

    glm::vec3 GetForward() const { return (Common::UnitX * mRotation); }
    glm::vec3 GetRight() const { return (Common::UnitY * mRotation); }

    class Game *GetGame() const { return mGame; }

private:
    std::vector<class Component *> mComponents;

    glm::mat4 mWorldTransform;
    glm::vec3 mPosition;
    glm::quat mRotation;
    glm::vec3 mScale;

    bool mRecomputeWorldTransform;

    class Game *mGame;
};