#include "ModelLoader.h"

#include "Graphics/Vertex.h"
#include "Graphics/AnimVertex.h"
#include "Graphics/Mesh.h"
#include "Graphics/AnimMesh.h"

#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"

ModelLoader::ModelLoader(const std::string &fileName)
{
    mScene = mImporter.ReadFile(fileName,
                                aiProcess_Triangulate |
                                    aiProcess_JoinIdenticalVertices |
                                    aiProcess_GenSmoothNormals);

    if (mScene == nullptr || mScene->mNumMeshes == 0)
    {
        SDL_Log("[Assimp] %s", mImporter.GetErrorString());
    }
}

bool ModelLoader::IsValid() const
{
    return (!(mScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE));
}

bool ModelLoader::IsAnimation() const
{
    return (IsValid() && mScene->mNumAnimations > 0);
}

Mesh *ModelLoader::MeshLoader()
{
    const aiMesh *mesh = mScene->mMeshes[0];

    std::vector<Vertex> vertices(mesh->mNumVertices);
    std::vector<uint32_t> indices;

    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        vertices[i].position.x = mesh->mVertices[i].x;
        vertices[i].position.y = mesh->mVertices[i].y;
        vertices[i].position.z = mesh->mVertices[i].z;
        if (mesh->HasNormals())
        {
            vertices[i].normal.x = mesh->mNormals[i].x;
            vertices[i].normal.y = mesh->mNormals[i].y;
            vertices[i].normal.z = mesh->mNormals[i].z;
        }
        if (mesh->HasTextureCoords(0))
        {
            vertices[i].texCoord.x = mesh->mTextureCoords[0][i].x;
            vertices[i].texCoord.y = mesh->mTextureCoords[0][i].y;
        }
    }

    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace &face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh *m = new Mesh(vertices, indices);

    return m;
}

AnimMesh *ModelLoader::AnimMeshLoader()
{
    const aiMesh *mesh = mScene->mMeshes[0];

    std::vector<AnimVertex> vertices(mesh->mNumVertices);
    std::vector<uint32_t> indices;

    for (int i = 0; i < mesh->mNumVertices; ++i)
    {
        vertices[i].position.x = mesh->mVertices[i].x;
        vertices[i].position.y = mesh->mVertices[i].y;
        vertices[i].position.z = mesh->mVertices[i].z;
        if (mesh->HasNormals())
        {
            vertices[i].normal.x = mesh->mNormals[i].x;
            vertices[i].normal.y = mesh->mNormals[i].y;
            vertices[i].normal.z = mesh->mNormals[i].z;
        }
        if (mesh->HasTextureCoords(0))
        {
            vertices[i].texCoord.x = mesh->mTextureCoords[0][i].x;
            vertices[i].texCoord.y = mesh->mTextureCoords[0][i].y;
        }
    }

    for (int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
    {
        aiBone *bone = mesh->mBones[boneIndex];
        for (int weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++)
        {
            aiVertexWeight &weight = bone->mWeights[weightIndex];
            int vertexId = weight.mVertexId;
            for (int i = 0; i < 4; ++i)
            {
                if (vertices[vertexId].weights[i] == 0.0f)
                {
                    vertices[vertexId].bones[i] = boneIndex;
                    vertices[vertexId].weights[i] = weight.mWeight;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace &face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    AnimMesh *am = new AnimMesh(vertices, indices);

    return am;
}

Skeleton ModelLoader::SkeletonLoader()
{
    Skeleton skeleton;

    for (int i = 0; i < mScene->mNumMeshes; ++i)
    {
        aiMesh *mesh = mScene->mMeshes[i];
        for (int j = 0; j < mesh->mNumBones; ++j)
        {
            aiBone *aiBone = mesh->mBones[j];

            Bone bone;
            bone.name = aiBone->mName.C_Str();
            bone.offset = ToGlm(aiBone->mOffsetMatrix);

            skeleton.AddBone(bone);
        }
    }

    BuildBoneHierarchy(&skeleton, mScene->mRootNode);

    skeleton.ResizeBoneMatrices();

    return skeleton;
}

void ModelLoader::BuildBoneHierarchy(Skeleton *skeleton, aiNode *node, int parentID)
{
    // 子ボーン
    Bone *bone = skeleton->FindBone(node->mName.C_Str());
    // bone != nullptr
    if (bone)
    {
        bone->bindLocalTransform = ToGlm(node->mTransformation);
        bone->localTransform = bone->bindLocalTransform;
        // 親登録
        bone->parentID = parentID;
        if (parentID != -1)
        {
            // 子ボーン登録
            // スケルトンから親呼んで、子IDを登録
            Bone *parent = skeleton->FindBone(parentID);
            if (parent)
            {
                parent->childrenIDs.push_back(bone->ID);
            }
            else
            {
                SDL_Log("boneが見つからない");
            }
        }
        else
        {
            // ルートボーン登録
            skeleton->SetRootID(bone->ID);
        }
        parentID = bone->ID;
    }

    for (int i = 0; i < node->mNumChildren; ++i)
    {
        BuildBoneHierarchy(skeleton, node->mChildren[i], parentID);
    }
}

std::vector<Animation *> ModelLoader::AnimationsLoader()
{
    std::vector<Animation *> animations;

    for (unsigned int i = 0; i < mScene->mNumAnimations; ++i)
    {
        aiAnimation *aiAnim = mScene->mAnimations[i];

        // 各アニメを登録
        Animation *anim = new Animation();
        anim->name = aiAnim->mName.C_Str();
        SDL_Log("%s", anim->name.c_str());
        anim->duration = static_cast<float>(aiAnim->mDuration);
        anim->ticksPerSecond = static_cast<float>(aiAnim->mTicksPerSecond);
        for (unsigned int i = 0; i < aiAnim->mNumChannels; ++i)
        {
            aiNodeAnim *channel = aiAnim->mChannels[i];
            std::string boneName = channel->mNodeName.C_Str();
            anim->channels[boneName] = BoneAnimLoader(channel);
        }

        animations.emplace_back(anim);
    }

    return animations;
}

BoneAnimation ModelLoader::BoneAnimLoader(const aiNodeAnim *channel)
{
    BoneAnimation animation;

    // 位置
    for (unsigned int i = 0; i < channel->mNumPositionKeys; ++i)
    {
        aiVectorKey &key = channel->mPositionKeys[i];

        KeyPosition position;
        position.time = static_cast<float>(key.mTime);
        position.value = ToGlm(key.mValue);
        animation.positions.push_back(position);
    }
    // 回転
    for (unsigned int i = 0; i < channel->mNumRotationKeys; ++i)
    {
        aiQuatKey &key = channel->mRotationKeys[i];

        KeyRotation rotation;
        rotation.time = static_cast<float>(key.mTime);
        rotation.value = ToGlm(key.mValue);
        animation.rotations.push_back(rotation);
    }
    // 拡大
    for (unsigned int i = 0; i < channel->mNumScalingKeys; ++i)
    {
        aiVectorKey &key = channel->mScalingKeys[i];

        KeyScale scale;
        scale.time = static_cast<float>(key.mTime);
        scale.value = ToGlm(key.mValue);
        animation.scales.push_back(scale);
    }

    return animation;
}

glm::mat4 ModelLoader::ToGlm(const aiMatrix4x4 &m)
{
    return glm::mat4(
        m.a1, m.b1, m.c1, m.d1,
        m.a2, m.b2, m.c2, m.d2,
        m.a3, m.b3, m.c3, m.d3,
        m.a4, m.b4, m.c4, m.d4);
}

glm::vec3 ModelLoader::ToGlm(const aiVector3D &v)
{
    return glm::vec3(v.x, v.y, v.z);
}

glm::quat ModelLoader::ToGlm(const aiQuaternion &q)
{
    return glm::quat(q.w, q.x, q.y, q.z);
}