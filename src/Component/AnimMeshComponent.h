#pragma onces

#include <string>

#include "Component.h"

#include "../Animation/AnimationPlayer.h"

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

private:
    AnimMesh *mAnimMesh;
    AnimationPlayer mAnimationPlayer;
};