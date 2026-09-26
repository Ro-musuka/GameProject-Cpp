#include "AnimMeshComponent.h"

#include "../Actor/Actor.h"

#include "../Graphics/Shader.h"
#include "../Graphics/AnimMesh.h"

AnimMeshComponent::AnimMeshComponent(Actor *owner, AnimMesh *animMesh, AnimationPlayer animPlayer)
    : Component(owner),
      mAnimMesh(nullptr),
      mAnimationPlayer(animPlayer)
{
    mAnimMesh = animMesh;
}

AnimMeshComponent::~AnimMeshComponent()
{
    delete mAnimMesh;
}

void AnimMeshComponent::Draw(Shader *shader)
{
    // ワールド変換行列計算
    mOwner->ComputeWorldTransform();
    shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
    // ボーン行列取得
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