#pragma once

#include <string>

#include <SDL3/SDL.h>

#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_opengl3.h"
#include "external/imgui/imgui_impl_sdl3.h"

class Renderer;

class Editor
{
public:
    Editor(Renderer *renderer);
    ~Editor();

    // AudioPlayer初期化後
    bool Init(const std::string &openGL_version);

    void ProcessEvent(const SDL_Event &event);
    void Draw();

private:
    float mScreenWidth;
    float mScreenHeight;

    Renderer *mRenderer;
};