// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "BoxComponent.h"

#include "Core/Game.h"
#include "Collision/PhysWorld.h"
#include "Actor/Actor.h"

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

void BoxComponent::OnCollision(BoxComponent *other)
{
}

void BoxComponent::ResolveStaticCollision(BoxComponent *other)
{
	const Collision::AABB &a = mWorldBox;
	const Collision::AABB &b = other->GetWorldBox();

	// めり込んだ距離を計算
	float overlapX = std::min(a.max.x, b.max.x) - std::max(a.min.x, b.min.x);
	if (overlapX <= 0.0f)
	{
		return;
	}
	float overlapY = std::min(a.max.y, b.max.y) - std::max(a.min.y, b.min.y);
	if (overlapY <= 0.0f)
	{
		return;
	}
	float overlapZ = std::min(a.max.z, b.max.z) - std::max(a.min.z, b.min.z);
	if (overlapZ <= 0.0f)
	{
		return;
	}

	glm::vec3 pos = mOwner->GetPosition();
	glm::vec3 otherPos = other->GetOwner()->GetPosition();

	// X方向のめり込みが最小なら、
	if (overlapX < overlapY && overlapX < overlapZ)
	{
		// pos.xの方が大きいなら、めり込んだ距離分、右にずらす
		//
		// 	wall |-------|			→	      wall|-------|
		// 		player|----|		→				player|----|

		// pos.xの方が小さいなら、めり込んだ距離分、左にずらす
		//
		// 		wall |-------|	   →	         wall|-------|
		// 	player|----|		   →	  player|----|

		if (pos.x < otherPos.x)
		{
			pos.x -= overlapX;
		}
		else
		{
			pos.x += overlapX;
		}
	}
	// Y方向
	else if (overlapY < overlapZ)
	{
		if (pos.y < otherPos.y)
		{
			pos.y -= overlapY;
		}
		else
		{
			pos.y += overlapY;
		}
	}
	// Z方向
	else
	{
		if (pos.z < otherPos.z)
		{
			pos.z -= overlapZ;
		}
		else
		{
			pos.z += overlapZ;
		}
	}

	mOwner->SetPosition(pos);
}