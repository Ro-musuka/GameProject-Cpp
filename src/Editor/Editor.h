#pragma once

#include <string>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

class Game;
class Renderer;
class BoxComponent;

class Editor
{
public:
    Editor(Game *game);
    ~Editor();

    bool Init(const std::string &openGL_version);

    void ProcessEvent(const SDL_Event &event);
    void Draw();

private:
    // ロール、ピッチ、ヨーのデータ
    glm::vec3 mAngleXYZ;

    Game *mGame;
    BoxComponent *mSelectedBox;
};