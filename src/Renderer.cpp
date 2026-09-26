#include "Renderer.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Game.h"
#include "Common.h"
#include "ModelLoader.h"

#include "Actor/Camera.h"
#include "Actor/Terrain.h"

#include "Component/MeshComponent.h"
#include "Component/AnimMeshComponent.h"

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Mesh.h"
#include "Graphics/AnimMesh.h"

#include "Animation/Skeleton.h"
#include "Animation/Animation.h"
#include "Animation/AnimationPlayer.h"

Renderer::Renderer(Game *game)
    : mGame(game),
      mWindow(nullptr),
      mContext(nullptr),
      mTarget{0.0f, 0.0f, 0.0f}
//   mTerrain(nullptr)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    // ver4.1のopenGLを使用
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    // openGLのコアプロファイルを使用
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // RGBA・深度バッファのサイズを指定
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // ダブルブッファリングの有効化
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //  openGLがハードウェアアクセラレーションを使うように強制する
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // ウィンドウの作成
    mWindow = SDL_CreateWindow("ゲーム プロジェクト in C++", static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
    if (!mWindow)
    {
        SDL_Log("ウィンドウの作成に失敗しました: %s", SDL_GetError());
        return false;
    }
    // openGL コンテキストを作成
    mContext = SDL_GL_CreateContext(mWindow);
    if (!mContext)
    {
        SDL_Log("OpenGLコンテキストの作成に失敗しました: %s", SDL_GetError());
        return false;
    }

    // GLEWによる設定初期化
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        SDL_Log("GLEWの初期化に失敗しました");
        return false;
    }
    glGetError();

    // GLEWによる初期化時に無害なエラーがでるため、
    // エラーをクリアしておく
    glEnable(GL_DEPTH_TEST);

    // モデルのロード
    Actor *actor = new Actor(GetGame());

    actor = new Actor(GetGame());
    actor->SetPosition(glm::vec3(.0f, .0f, .0f));

    ModelLoader loader("../assets/models/BaseMan.glb");
    if (!loader.IsValid())
    {
        SDL_Log("BaseMan.glbの読み込みに失敗しました");
        return false;
    }
    AnimationPlayer animPlayer(loader.SkeletonLoader(), loader.AnimationsLoader());
    AnimMeshComponent *comp = new AnimMeshComponent(actor, loader.AnimMeshLoader(), animPlayer);
    comp->Play("Walking");
    mAnimMeshComps.emplace_back(comp);

    // mTerrain = new Terrain(GetGame());
    // mTerrain->Load(256, 256);
    // mTerrain->SetScale(glm::vec3(0.1));
    // mTerrain->SetPosition({-10.f, 0, -10.f});

    // シェーダーのロード
    Shader shader;
    if (!shader.Load("../assets/shaders/Basic.vert", "../assets/shaders/Basic.frag"))
    {
        SDL_Log("shaderの読み込みに失敗しました");
        return false;
    }
    mShaderMap["base"] = shader;

    Shader animationShader;
    if (!animationShader.Load("../assets/shaders/Skinned.vert", "../assets/shaders/Basic.frag"))
    {
        SDL_Log("shaderの読み込みに失敗しました");
        return false;
    }
    mShaderMap["anim"] = animationShader;

    // 視野角, アスペクト比, 近クリップ面, 遠クリップ面
    mProjection = glm::perspective(glm::radians(45.0f), mScreenWidth / mScreenHeight, 0.1f, 100.0f);
    mWorldTransform = glm::mat4(1.0f);

    mObjectColor = glm::vec3(0.8f);
    mLightColor = glm::vec3(1.0f);

    return true;
}

void Renderer::AttachModel(const std::string &name, Actor *actor)
{
    Mesh *mesh = mAssetManager.GetMesh(name);
    if (mesh)
    {
        Component *comp = new MeshComponent(actor, mesh);
        mMeshComps.emplace_back(comp);
        return;
    }

    AnimMesh *animMesh = mAssetManager.GetAnimMesh(name);
    AnimationPlayer animPlayer(mAssetManager.GetSkeleton(name), mAssetManager.GetAnimations(name));
    Component *comp = new AnimMeshComponent(actor, animMesh, animPlayer);
    mMeshComps.emplace_back(comp);
}

void Renderer::Draw()
{
    // 前回の画面をクリア
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    mView = glm::lookAt(mGame->GetCamera()->GetPosition(), mTarget, Common::UnitY);
    mLightDir = -glm::normalize(mGame->GetCamera()->GetPosition());

    Shader &shader = mShaderMap["base"];
    shader.SetActive();
    shader.SetMatrixUniform("uView", mView);
    shader.SetMatrixUniform("uProjection", mProjection);
    shader.SetVectorUniform("lightDir", mLightDir);
    shader.SetVectorUniform("objectColor", mObjectColor);
    shader.SetVectorUniform("lightColor", mLightColor);
    for (auto &model : mMeshComps)
    {
        model->Draw(shader);
    }
    // mTerrain->ComputeWorldTransform();
    // shader.SetMatrixUniform("uWorldTransform", mTerrain->GetWorldTransform());
    // mTerrain->GetMesh()->Draw();

    Shader &animShader = mShaderMap["anim"];
    animShader.SetActive();
    animShader.SetMatrixUniform("uView", mView);
    animShader.SetMatrixUniform("uProjection", mProjection);
    animShader.SetVectorUniform("lightDir", mLightDir);
    animShader.SetVectorUniform("objectColor", mObjectColor);
    animShader.SetVectorUniform("lightColor", mLightColor);
    for (auto &model : mAnimMeshComps)
    {
        model->Draw(&animShader);
    }

    // 画面バッファの交換
    SDL_GL_SwapWindow(mWindow);
}

void Renderer::UnloadData()
{
    for (auto &[name, shader] : mShaderMap)
    {
        shader.Unload();
    }
    mShaderMap.clear();

    for (auto &model : mMeshComps)
    {
        delete model;
    }

    for (auto &model : mAnimMeshComps)
    {
        delete model;
    }
}

void Renderer::Shutdown()
{
    SDL_GL_DestroyContext(mContext);
    SDL_DestroyWindow(mWindow);
}