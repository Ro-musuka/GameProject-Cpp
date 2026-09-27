#pragma once

#include <vector>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

class Game;
class Actor;
class Shader;
class Texture;
class MeshComponent;
class AnimMeshComponent;
class FollowCameraComponent;

class Renderer
{
public:
    Renderer(Game *game);

    bool Initialize(float screenWidth, float screenHeight);
    void Shutdown();

    void Draw();

    Game *GetGame() const { return mGame; }
    void SetView(const glm::mat4 &view) { mView = view; }

    void AddMeshComponent(MeshComponent *mesh) { mMeshComps.emplace_back(mesh); }
    void RemoveMeshComponent(MeshComponent *mesh);
    void AddAnimMeshComponent(AnimMeshComponent *amesh) { mAnimMeshComps.emplace_back(amesh); }
    void RemoveAnimMeshComponent(AnimMeshComponent *amesh);

    void SetCameraComp(FollowCameraComponent *cameraComp) { mCameraComp = cameraComp; }

private:
    bool LoadShader();

    float mScreenWidth;
    float mScreenHeight;

    // ワールド座標への変換行列
    glm::mat4 mView;
    glm::mat4 mProjection;

    // 光源
    glm::vec3 mLightDir;
    glm::vec3 mObjectColor;
    glm::vec3 mLightColor;

    std::vector<MeshComponent *> mMeshComps;
    std::vector<AnimMeshComponent *> mAnimMeshComps;
    FollowCameraComponent *mCameraComp;

    Game *mGame;

    SDL_Window *mWindow;
    SDL_GLContext mContext;
};