#include "MeshComponent.h"

#include "Core/Game.h"

#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Mesh_Vertex/Mesh.h"

#include "Actor/Actor.h"

MeshComponent::MeshComponent(Actor *owner, Mesh *mesh)
    : Component(owner),
      mMesh(nullptr)
{
    mMesh = mesh;
    mOwner->GetGame()->GetRenderer()->AddMeshComponent(this);
}

MeshComponent::~MeshComponent()
{
    delete mMesh;
    mOwner->GetGame()->GetRenderer()->RemoveMeshComponent(this);
}

void MeshComponent::Draw(Shader &shader)
{
    // ワールド変換行列計算
    mOwner->ComputeWorldTransform();
    shader.SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
    mMesh->Draw();
}