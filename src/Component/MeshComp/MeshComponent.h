#pragma onces

#include <string>

#include "Component/Component.h"

class Actor;
class Mesh;
class Shader;

class MeshComponent : public Component
{
public:
    MeshComponent(Actor *owner, Mesh *mesh);
    ~MeshComponent();

    void Draw(Shader &shader);

    // ゲッター/セッター
    const char *GetTypeName() const override { return "PlayerBoxComponent"; }
    const std::string &GetModelName() const { return mModelName; }
    void SetModelName(const std::string &modelName) { mModelName = modelName; }

private:
    Mesh *mMesh;
    std::string mModelName;
};