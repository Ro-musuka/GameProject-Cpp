// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "BoxComponent.h"

#include "../Game.h"
#include "../PhysWorld.h"

#include "../Actor/Actor.h"

BoxComponent::BoxComponent(Actor *owner, int updateOrder)
	: Component(owner, updateOrder),
	  mObjectBox(glm::vec3(.0f), glm::vec3(.0f)),
	  mWorldBox(glm::vec3(.0f), glm::vec3(.0f)),
	  mShouldRotate(true)
{
	mOwner->GetGame()->GetPhysWorld()->AddBox(this);
}

BoxComponent::~BoxComponent()
{
	mOwner->GetGame()->GetPhysWorld()->RemoveBox(this);
}

void BoxComponent::OnUpdateWorldTransform()
{
	mWorldBox = mObjectBox;
	mWorldBox.min *= mOwner->GetScale();
	mWorldBox.max *= mOwner->GetScale();
	if (mShouldRotate)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	mWorldBox.min += mOwner->GetPosition();
	mWorldBox.max += mOwner->GetPosition();
}
