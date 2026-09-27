#include "Terrain.h"

#include <glm/glm.hpp>

#include "../Game.h"

#include "../Graphics/Mesh.h"
#include "../Graphics/Vertex.h"

Terrain::Terrain(Game *game)
    : Actor(game)
{
}

Terrain::~Terrain()
{
    delete mMesh;
}

void Terrain::Load(int width, int depth)
{
    Generate(width, depth);
    SetMeshData();
}

void Terrain::SetMeshData()
{
    // meshを登録
    std::vector<Vertex> vertices;

    for (int x = 0; x < mHeight.size(); x++)
    {
        for (int z = 0; z < mHeight[x].size(); z++)
        {
            glm::vec3 a, b;
            Vertex v;
            v.position.x = b.x = static_cast<float>(x);
            v.position.y = a.y = b.y = mHeight[x][z];
            v.position.z = a.z = static_cast<float>(z);

            if (x != 0)
            {
                a.x = x - 1;
            }
            else if (x == 0)
            {
                a.z = x + 1;
            }

            if (z != 0)
            {
                b.z = z - 1;
            }
            else if (z == 0)
            {
                b.z = z + 1;
            }

            a.y = mHeight[a.x][a.z];
            b.y = mHeight[b.x][b.z];

            auto n = glm::cross(a, b);
            v.normal.x = n.x;
            v.normal.y = n.y;
            v.normal.z = n.z;

            v.texCoord.x = v.texCoord.y = 0;

            vertices.emplace_back(v);
        }
    }

    std::vector<uint32_t> indices;

    for (int x = 0; x < mHeight.size() - 1; x++)
    {
        for (int z = 0; z < mHeight[x].size() - 1; z++)
        {
            uint32_t i0 = x * mHeight[x].size() + z;
            uint32_t i1 = i0 + 1;
            uint32_t i2 = (x + 1) * mHeight[x].size() + z;
            uint32_t i3 = i2 + 1;

            indices.emplace_back(i0);
            indices.emplace_back(i2);
            indices.emplace_back(i1);

            indices.emplace_back(i1);
            indices.emplace_back(i2);
            indices.emplace_back(i3);
        }
    }

    mMesh = new Mesh(vertices, indices);
}

std::vector<std::vector<float>> &Terrain::Generate(int width, int depth)
{
    mHeight.resize(width);
    for (int x = 0; x < width; x++)
    {
        mHeight[x].resize(depth);
        for (int z = 0; z < depth; z++)
        {
            mHeight[x][z] = glm::sin(x * 0.1f) * glm::cos(z * 0.1f) * 10.0f;
        }
    }

    return mHeight;
}