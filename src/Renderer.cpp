#include "Renderer.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Game.h"
#include "Common.h"
#include "ModelLoader.h"

#include "Actor/Terrain.h"

#include "Component/FollowCameraComponent.h"
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
    // 3dモデルのロード
    if (!LoadModel("../assets/models/BaseMan.glb", "anim"))
    {
        return false;
    }
    // モデルの肉付け
    AttachModelComp(actor, "anim");
    // 再生するアニメの設定
    mAnimMeshComps.back()->Play("Walking");
    actor->SetRotation(glm::angleAxis(glm::radians(90.f), Common::UnitX));

    actor = new Actor(GetGame());
    if (!LoadModel("../assets/models/box.glb", "box"))
    {
        return false;
    }
    AttachModelComp(actor, "box");
    actor->SetScale(glm::vec3{10.f, .5f, 17.f});
    mCameraComp = new FollowCameraComponent(actor);

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
    mAssetManager.SetShader("base", shader);

    Shader animationShader;
    if (!animationShader.Load("../assets/shaders/Skinned.vert", "../assets/shaders/Basic.frag"))
    {
        SDL_Log("shaderの読み込みに失敗しました");
        return false;
    }
    mAssetManager.SetShader("anim", animationShader);

    // 視野角, アスペクト比, 近クリップ面, 遠クリップ面
    mProjection = glm::perspective(glm::radians(45.0f), mScreenWidth / mScreenHeight, 0.1f, 500.0f);
    mWorldTransform = glm::mat4(1.0f);

    // 光源色
    mObjectColor = glm::vec3(0.8f);
    mLightColor = glm::vec3(1.0f);

    return true;
}

void Renderer::Draw()
{
    // 前回の画面をクリア
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    mView = glm::lookAt(mCameraComp->GetCameraPos(), mTarget, Common::UnitY);
    mLightDir = -glm::normalize(mCameraComp->GetCameraPos());

    // 通常モデルの描画
    Shader &shader = mAssetManager.GetShader("base");
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

    // アニメーションモデルの描画
    Shader &animShader = mAssetManager.GetShader("anim");
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

bool Renderer::LoadModel(const std::string &fileName, const std::string &modelName)
{
    ModelLoader loader(fileName);
    if (!loader.IsValid())
    {
        SDL_Log("%sの読み込みに失敗しました", fileName.c_str());
        return false;
    }

    if (loader.IsAnimation())
    {
        mAssetManager.SetAnimMesh(modelName, loader.AnimMeshLoader());
        mAssetManager.SetSkeleton(modelName, loader.SkeletonLoader());
        mAssetManager.SetAnimations(modelName, loader.AnimationsLoader());
    }
    else
    {
        mAssetManager.SetMesh(modelName, loader.MeshLoader());
    }

    return true;
}

void Renderer::AttachModelComp(Actor *actor, const std::string &modelName)
{
    Mesh *mesh = mAssetManager.GetMesh(modelName);
    if (mesh != nullptr)
    {
        MeshComponent *mc = new MeshComponent(actor, mesh);
        mMeshComps.emplace_back(mc);
        return;
    }

    AnimMesh *amesh = mAssetManager.GetAnimMesh(modelName);
    if (amesh != nullptr)
    {
        AnimationPlayer animPlayer(mAssetManager.GetSkeleton(modelName), *mAssetManager.GetAnimations(modelName));
        AnimMeshComponent *amc = new AnimMeshComponent(actor, amesh, animPlayer);
        mAnimMeshComps.emplace_back(amc);
        return;
    }

    SDL_Log("%sがAssetManagerに登録されていません。", modelName.c_str());
}

void Renderer::UnloadData()
{
    mAssetManager.Unload();
}

void Renderer::Shutdown()
{
    SDL_GL_DestroyContext(mContext);
    SDL_DestroyWindow(mWindow);
}