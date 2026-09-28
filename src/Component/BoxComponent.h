// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

#include "Component.h"
#include "../Collision.h"

class BoxComponent : public Component
{
public:
    BoxComponent(class Actor *owner, int updateOrder = 100);
    ~BoxComponent();

    void OnUpdateWorldTransform() override;

    void SetObjectBox(const Collision::AABB &model) { mObjectBox = model; }
    const Collision::AABB &GetWorldBox() const { return mWorldBox; }

    void SetShouldRotate(bool value) { mShouldRotate = value; }

private:
    Collision::AABB mObjectBox;
    Collision::AABB mWorldBox;
    bool mShouldRotate;
};
