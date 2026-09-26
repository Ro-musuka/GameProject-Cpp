#pragma once

#include <vector>
#include <unordered_map>
#include <filesystem>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include "AssetManager.h"

class Game;
class Actor;
class Shader;
class Texture;
class MeshComponent;
class AnimMeshComponent;

class Renderer
{
public:
    Renderer(Game *game);

    bool Initialize(float screenWidth, float screenHeight);
    void Shutdown();
    void UnloadData();

    void Draw();

    Game *GetGame() const { return mGame; }
    void SetView(const glm::mat4 &view) { mView = view; }

private:
    bool LoadModel(const std::string &filName, const std::string &modelName);
    void AttachModelComp(Actor *actor, const std::string &modelName);

    float mScreenWidth;
    float mScreenHeight;

    // ワールド座標への変換行列
    glm::mat4 mView;
    glm::mat4 mProjection;
    glm::mat4 mWorldTransform;

    glm::vec3 mTarget;

    // 光源
    glm::vec3 mLightDir;
    glm::vec3 mObjectColor;
    glm::vec3 mLightColor;

    std::vector<MeshComponent *> mMeshComps;
    std::vector<AnimMeshComponent *> mAnimMeshComps;
    AssetManager mAssetManager;
    // class Terrain *mTerrain;

    Game *mGame;

    SDL_Window *mWindow;
    SDL_GLContext mContext;
};