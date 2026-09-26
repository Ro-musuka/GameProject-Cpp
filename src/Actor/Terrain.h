#pragma once
#include "Actor.h"
#include <vector>

class Terrain : public Actor
{
public:
    Terrain(class Game *game);
    ~Terrain();

    void Load(int width, int depth);

    class Mesh *GetMesh() const { return mMesh; }

private:
    std::vector<std::vector<float>> &Generate(int width, int depth);
    void SetMeshData();

    std::vector<std::vector<float>> mHeight;
    class Mesh *mMesh;
};