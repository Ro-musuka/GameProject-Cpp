#pragma once

class Actor;

class Component
{
public:
    Component(Actor *owner, int updateOrder = 100);
    virtual ~Component();

    virtual void Update(float deltaTime);
    virtual void OnUpdateWorldTransform();

    virtual void ProcessInput(const bool *keystate);

    // ゲッター/セッター
    virtual const char *GetTypeName() const { return "Component"; }

    int GetUpdateOrder() const { return mUpdateOrder; }

    Actor *GetOwner() { return mOwner; }

protected:
    int mUpdateOrder;
    Actor *mOwner;
};