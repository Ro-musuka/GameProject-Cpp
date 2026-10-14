#pragma onces

#include <string>

#include "Component/Component.h"
#include "Graphics/Animation/AnimationPlayer.h"

class Actor;
class AnimMesh;
class AnimationPlayer;
class Shader;

class AnimMeshComponent : public Component
{
public:
    AnimMeshComponent(Actor *owner, AnimMesh *mesh, AnimationPlayer animPlayer);
    ~AnimMeshComponent();

    void Draw(Shader *shader);

    void Update(float deltaTime) override;
    void Play(const std::string &animName);
    void Stop();

    // ゲッター/セッター
    const char *GetTypeName() const override { return "PlayerBoxComponent"; }
    const std::string &GetModelName() const { return mModelName; }
    void SetModelName(const std::string &modelName) { mModelName = modelName; }

private:
    AnimMesh *mAnimMesh;
    AnimationPlayer mAnimationPlayer;
    std::string mModelName;
};