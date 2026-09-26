#include "MeshComponent.h"

#include "../Actor/Actor.h"

#include "../Graphics/Shader.h"
#include "../Graphics/Mesh.h"
#include "../ModelLoader.h"

MeshComponent::MeshComponent(Actor *owner, Mesh *mesh)
    : Component(owner),
      mMesh(nullptr)
{
    mMesh = mesh;
}

MeshComponent::~MeshComponent()
{
    delete mMesh;
}

void MeshComponent::Draw(Shader &shader)
{
    // ワールド変換行列計算
    mOwner->ComputeWorldTransform();
    shader.SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
    mMesh->Draw();
}