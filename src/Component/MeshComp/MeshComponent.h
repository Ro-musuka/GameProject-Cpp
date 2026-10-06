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

private:
    Mesh *mMesh;
};