#include "AnimMeshComponent.h"

#include "Core/Game.h"

#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Mesh_Vertex/AnimMesh.h"

#include "Actor/Actor.h"

AnimMeshComponent::AnimMeshComponent(Actor *owner, AnimMesh *animMesh, AnimationPlayer animPlayer)
    : Component(owner),
      mAnimMesh(nullptr),
      mAnimationPlayer(animPlayer)
{
    mAnimMesh = animMesh;
    mOwner->GetGame()->GetRenderer()->AddAnimMeshComponent(this);
}

AnimMeshComponent::~AnimMeshComponent()
{
    delete mAnimMesh;
    mOwner->GetGame()->GetRenderer()->RemoveAnimMeshComponent(this);
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