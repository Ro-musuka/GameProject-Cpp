#pragma once

#include <vector>
#include <glm/glm.hpp>

// グリッドの各セル
struct GridCell
{
    // 隣接セルへの移動可否
    bool canMoveNorth = true;
    bool canMoveSouth = true;
    bool canMoveEast = true;
    bool canMoveWest = true;
};

class NavigationGrid
{
public:
    NavigationGrid(int width, int height, float cellSize, const glm::vec3 &origin);

    // 隣接セルへ移動が可能かを調べる
    bool CanMove(const glm::ivec2 &from, const glm::ivec2 &to);

    // グリッドを座標に変換
    bool WorldToGrid(const glm::vec3 &worldPos, glm::ivec2 &outGridPos) const;
    // 座標をグリッドへ変換
    glm::vec3 GridToWorld(const glm::ivec2 &gridPos) const;

    // ゲッター/セッター
    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
    const std::vector<std::vector<GridCell>> &GetCells() const { return mCells; }

    GridCell *GetCell(int x, int z);
    void SetConnection(const glm::ivec2 &from, const glm::ivec2 &to, bool canMove);

private:
    bool IsValid(int x, int z) const;

    int mWidth;
    int mHeight;
    float mCellSize;

    glm::vec3 mOriginPos;

    std::vector<std::vector<GridCell>> mCells;
};