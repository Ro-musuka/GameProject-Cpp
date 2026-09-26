#include "AnimMeshComponent.h"

#include "../Actor/Actor.h"

#include "../Graphics/Shader.h"
#include "../Graphics/AnimMesh.h"

AnimMeshComponent::AnimMeshComponent(Actor *owner, AnimMesh *animMesh, AnimationPlayer animPlayer)
    : Component(owner), mAnimMesh(nullptr), mAnimationPlayer(animPlayer)
{
    mAnimMesh = animMesh;
}

AnimMeshComponent::~AnimMeshComponent()
{
    delete mAnimMesh;
}

void AnimMeshComponent::Draw(Shader *shader)
{
    mOwner->ComputeWorldTransform();
    shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
    shader->SetMatrixArrayUniform("uMatrixPalette", mAnimationPlayer.GetBoneMatrices());
    mAnimMesh->Draw();
}

void AnimMeshComponent::Update(float deltaTime)
{
    Component::Update(deltaTime);
    mAnimationPlayer.Update(deltaTime);
}

void AnimMeshComponent::Play(const std::string &animName)
{
    mAnimationPlayer.Play(animName);
}

void AnimMeshComponent::Stop()
{
    mAnimationPlayer.Stop();
}