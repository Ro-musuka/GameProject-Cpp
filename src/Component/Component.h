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

    Actor *GetOwner() { return mOwner; }
    int GetUpdateOrder() const { return mUpdateOrder; }

protected:
    int mUpdateOrder;
    Actor *mOwner;
};