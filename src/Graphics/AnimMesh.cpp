#include "AnimMesh.h"

#include "AnimVertex.h"
#include "../Animation/Skeleton.h"

AnimMesh::AnimMesh(const std::vector<AnimVertex> &vertices, const std::vector<uint32_t> &indices)
{
    // メッシュデータをopenGLに設定
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

    // 頂点バッファ
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(AnimVertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (void *)offsetof(AnimVertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (void *)offsetof(AnimVertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (void *)offsetof(AnimVertex, texCoord));
    glEnableVertexAttribArray(2);

    glVertexAttribIPointer(3, 4, GL_INT, sizeof(AnimVertex), (void *)offsetof(AnimVertex, bones));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (void *)offsetof(AnimVertex, weights));
    glEnableVertexAttribArray(4);

    // インデックスバッファ
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    mIndexCount = indices.size();
}

AnimMesh::~AnimMesh()
{
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    glDeleteVertexArrays(1, &mVAO);
}

void AnimMesh::Draw()
{
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, nullptr);
}