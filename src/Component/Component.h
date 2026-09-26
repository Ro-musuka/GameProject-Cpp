#pragma once

class Actor;

class Component
{
public:
    Component(Actor *owner, int updateOrder = 100);
    ~Component();

    virtual void Update(float deltaTime);
    virtual void OnUpdateWorldTransform();

    virtual void ProcessInput(const bool *keystate);

    int GetUpdateOrder() const { return mUpdateOrder; }

protected:
    int mUpdateOrder;
    Actor *mOwner;
};