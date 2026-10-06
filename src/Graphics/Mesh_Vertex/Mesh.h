#pragma once

#include <vector>

#include <GL/glew.h>

class Mesh
{
public:
    Mesh(const std::vector<struct Vertex> &vertices, const std::vector<uint32_t> &indices);
    ~Mesh();

    void Draw();

private:
    GLuint mVAO;
    GLuint mVBO;
    GLuint mEBO;

    uint32_t mIndexCount;
};