#include <SDL3/SDL.h>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Mesh;
class AnimMesh;
class Skeleton;
struct Animation;
struct BoneAnimation;

class ModelLoader
{
public:
    ModelLoader(const std::string &fileName);

    // 読み込み判定
    bool IsValid() const;
    // アニメーション判定
    bool IsAnimation() const;

    // 読み込んだデータの取得関数
    Mesh *MeshLoader();
    AnimMesh *AnimMeshLoader();
    Skeleton SkeletonLoader();
    std::vector<Animation *> AnimationsLoader();

private:
    // 子ボーンの登録
    void BuildBoneHierarchy(Skeleton *skeleton, aiNode *node, int parentID = -1);
    // アニメ中のボーンの動きを取得
    BoneAnimation BoneAnimLoader(const aiNodeAnim *channel);

    // Assimp座標系をglm座標系に修正
    glm::mat4 ToGlm(const aiMatrix4x4 &mat4);
    glm::vec3 ToGlm(const aiVector3D &vec3);
    glm::quat ToGlm(const aiQuaternion &quat);

    Assimp::Importer mImporter;
    const aiScene *mScene;
};