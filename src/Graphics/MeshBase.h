#pragma once

#include <GL/glew.h>
#include <vector>

class MeshBase
{
public:
    MeshBase()
    {
        // メッシュデータをopenGLに設定
        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);
        glGenBuffers(1, &mEBO);

        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    }

    virtual ~MeshBase()
    {
        glDeleteBuffers(1, &mVBO);
        glDeleteBuffers(1, &mEBO);
        glDeleteVertexArrays(1, &mVAO);
    }

protected:
    GLuint mVAO;
    GLuint mVBO;
    GLuint mEBO;

    uint32_t mIndexCount;
};