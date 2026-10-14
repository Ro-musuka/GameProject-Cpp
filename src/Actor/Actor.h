#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Core/Common.h"

class Game;
class Component;

class Actor
{
    // GetComponentを触りたいので、
    friend class SceneSerializer;

public:
    enum State
    {
        EActive,
        EPaused,
        EDead
    };

    Actor(Game *game);
    virtual ~Actor();

    // 更新
    void Update(float deltaTime);
    virtual void UpdateActor(float deltaTime);
    void UpdateComponent(float deltaTime);

    // 入力
    void ProcessInput(const bool *keystate);
    virtual void ActorInput(const bool *keystate);

    // ワールド変換の計算
    void ComputeWorldTransform();

    // compの追加
    // comp側が自動で追加・削除を行う
    void AddComponent(Component *component);
    void RemoveComponent(Component *component);

    // ゲッター/セッター
    // 型の名前
    virtual const char *GetTypeName() const { return "Actor"; }

    const glm::mat4 &GetWorldTransform() const { return mWorldTransform; }

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

    // 見いている方向
    glm::vec3 GetForward() const { return (Common::UnitZ * mRotation); }
    glm::vec3 GetRight() const { return (Common::UnitX * mRotation); }

    State GetState() const { return mState; }
    void SetState(State state) { mState = state; }

    Game *GetGame() const { return mGame; }

    // 指定のComponentを探索
    template <typename T>
    T *FindComponent() const
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
    const std::vector<Component *> &GetComponents() { return mComponents; }

    // Actorのデータ
    glm::mat4 mWorldTransform;
    glm::vec3 mPosition;
    glm::quat mRotation;
    glm::vec3 mScale;

    State mState;

    // 座標変化があった場合にtrue
    // ワールド変換の再計算をすべきか？
    bool mRecomputeWorldTransform;

    Game *mGame;
};