#include "Renderer.h"

#include <algorithm>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Game.h"
#include "Common.h"
#include "AssetManager.h"

#include "Component/MeshComponent.h"
#include "Component/AnimMeshComponent.h"
#include "Component/FollowCameraComponent.h"

#include "Graphics/Shader.h"

Renderer::Renderer(Game *game)
    : mGame(game),
      mWindow(nullptr),
      mContext(nullptr),
      mTarget{0.0f, 0.0f, 0.0f}
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

    // シェーダー読み込み
    if (!LoadShader())
    {
        SDL_Log("シェーダーの読み込みに失敗しました。");
        return false;
    }

    // 視野角, アスペクト比, 近クリップ面, 遠クリップ面
    mProjection = glm::perspective(glm::radians(45.0f), mScreenWidth / mScreenHeight, 0.1f, 500.0f);

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

    glm::vec3 cameraPos = mGame->GetCameraComp()->GetCameraPos();
    mView = glm::lookAt(cameraPos, mTarget, Common::UnitY);
    mLightDir = -glm::normalize(cameraPos);

    // 通常モデルの描画
    Shader &shader = mGame->GetAssetManager()->GetShader("base");
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
    Shader &animShader = mGame->GetAssetManager()->GetShader("anim");
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

void Renderer::Shutdown()
{
    SDL_GL_DestroyContext(mContext);
    SDL_DestroyWindow(mWindow);
}

bool Renderer::LoadShader()
{
    AssetManager *assetManager = mGame->GetAssetManager();

    // シェーダーのロード
    Shader shader;
    if (!shader.Load("../assets/shaders/Basic.vert", "../assets/shaders/Basic.frag"))
    {
        SDL_Log("Basic.vertとBasic.fragのコンパイルに失敗しました");
        return false;
    }
    assetManager->SetShader("base", shader);

    Shader animationShader;
    if (!animationShader.Load("../assets/shaders/Skinned.vert", "../assets/shaders/Basic.frag"))
    {
        SDL_Log("Skinned.vertとBasic.fragのコンパイルに失敗しました");
        return false;
    }
    assetManager->SetShader("anim", animationShader);

    return true;
}

void Renderer::RemoveMeshComponent(MeshComponent *mesh)
{
    auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
    if (iter != mMeshComps.end())
    {
        std::iter_swap(iter, mMeshComps.end() - 1);
        mMeshComps.pop_back();
    }
}

void Renderer::RemoveAnimMeshComponent(AnimMeshComponent *amesh)
{
    auto iter = std::find(mAnimMeshComps.begin(), mAnimMeshComps.end(), amesh);
    if (iter != mAnimMeshComps.end())
    {
        std::iter_swap(iter, mAnimMeshComps.end() - 1);
        mAnimMeshComps.pop_back();
    }
}