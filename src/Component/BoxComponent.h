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
    // 衝突タイプ
    enum CollisionType
    {
        EStatic,
        EDynamic,
        ETrigger
    };

    BoxComponent(class Actor *owner, int updateOrder = 100);
    ~BoxComponent();

    void OnUpdateWorldTransform() override;

    virtual void OnCollision(BoxComponent *other);
    void ResolveStaticCollision(BoxComponent *other);

    // ゲッター/セッター
    void SetObjectBox(const Collision::AABB &model)
    {
        mObjectBox = model;
    }
    const Collision::AABB &GetWorldBox() const { return mWorldBox; }

    void SetShouldRotate(bool value) { mShouldRotate = value; }

    CollisionType GetCollisionType() const { return mCollisionType; }
    void SetCollisionType(CollisionType type) { mCollisionType = type; }

private:
    Collision::AABB mObjectBox;
    Collision::AABB mWorldBox;
    bool mShouldRotate;

    // 衝突タイプ
    CollisionType mCollisionType;
};
