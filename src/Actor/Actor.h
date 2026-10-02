#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../Common.h"

class Game;
class Component;

class Actor
{
public:
    Actor(Game *game);
    virtual ~Actor();

    void Update(float deltaTime);
    virtual void UpdateActor(float deltaTime);
    void UpdateComponent(float deltaTime);

    void ProcessInput(const bool *keystate);
    virtual void ActorInput(const bool *keystate);

    void AddComponent(Component *component);
    void RemoveComponent(Component *component);

    void ComputeWorldTransform();

    // ゲッター/セッター
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

    const glm::mat4 &GetWorldTransform() const { return mWorldTransform; }

    glm::vec3 GetForward() const { return (Common::UnitX * mRotation); }
    glm::vec3 GetRight() const { return (Common::UnitY * mRotation); }

    Game *GetGame() const { return mGame; }

    template <typename T>
    T *GetComponent() const
    {
        T *result = nullptr;
        for (Component *comp : mComponents)
        {
            result = dynamic_cast<T *>(comp);
            if (result != nullptr)
            {
                break;
            }
        }

        return result;
    }

private:
    std::vector<Component *> mComponents;

    glm::mat4 mWorldTransform;
    glm::vec3 mPosition;
    glm::quat mRotation;
    glm::vec3 mScale;

    bool mRecomputeWorldTransform;

    Game *mGame;
};