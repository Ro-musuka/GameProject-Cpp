#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class AnimMesh
{
public:
    AnimMesh(const std::vector<struct AnimVertex> &vertices, const std::vector<uint32_t> &indices);
    ~AnimMesh();

    void Draw();

private:
    GLuint mVAO;
    GLuint mVBO;
    GLuint mEBO;

    uint32_t mIndexCount;
};