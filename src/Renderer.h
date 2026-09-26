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
    ~Renderer();

    bool Initialize(float screenWidth, float screenHeight);
    void Shutdown();
    void UnloadData();

    void Draw();

    Game *GetGame() const { return mGame; }
    void SetView(const glm::mat4 &view) { mView = view; }

private:
    void AttachModel(const std::string &name, Actor *actor);

    float mScreenWidth;
    float mScreenHeight;

    glm::mat4 mView;
    glm::mat4 mProjection;
    glm::mat4 mWorldTransform;

    glm::vec3 mTarget;

    glm::vec3 mLightDir;
    glm::vec3 mObjectColor;
    glm::vec3 mLightColor;

    std::unordered_map<std::string, Shader> mShaderMap;

    std::vector<MeshComponent *> mMeshComps;
    std::vector<AnimMeshComponent *> mAnimMeshComps;
    AssetManager mAssetManager;
    // class Terrain *mTerrain;

    Game *mGame;

    SDL_Window *mWindow;
    SDL_GLContext mContext;
};