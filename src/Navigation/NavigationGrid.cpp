#include "NavigationGrid.h"

NavigationGrid::NavigationGrid(int width, int height, float cellSize, const glm::vec3 &origin)
    : mWidth(width),
      mHeight(height),
      mCellSize(cellSize),
      mOriginPos(origin)
{
    mCells.resize(mHeight);
    for (auto &cell : mCells)
    {
        cell.resize(mWidth);
    }
}

bool NavigationGrid::CanMove(const glm::ivec2 &from, const glm::ivec2 &to)
{
    GridCell *cell = GetCell(from.x, from.y);
    if (cell == nullptr)
    {
        return false;
    }

    if (to.x == from.x && to.y == from.y - 1)
    {
        return cell->canMoveNorth;
    }

    if (to.x == from.x && to.y == from.y + 1)
    {
        return cell->canMoveSouth;
    }

    if (to.x == from.x + 1 && to.y == from.y)
    {
        return cell->canMoveEast;
    }

    if (to.x == from.x - 1 && to.y == from.y)
    {
        return cell->canMoveWest;
    }

    // 隣接していないセルへの移動は不可
    return false;
}

bool NavigationGrid::WorldToGrid(const glm::vec3 &worldPos, glm::ivec2 &outGridPos) const
{
    int x = static_cast<int>(glm::floor((worldPos.x - mOriginPos.x) / mCellSize));
    int y = static_cast<int>(glm::floor((worldPos.y - mOriginPos.z) / mCellSize));
    if (!IsValid(x, y))
    {
        return false;
    }

    outGridPos.x = x;
    outGridPos.y = y;

    return true;
}

glm::vec3 NavigationGrid::GridToWorld(const glm::ivec2 &gridPos) const
{
    float x = mOriginPos.x + (gridPos.x + 0.5f) * mCellSize;
    float z = mOriginPos.y + (gridPos.y + 0.5f) * mCellSize;

    return glm::vec3(x, mOriginPos.y, z);
}

GridCell *NavigationGrid::GetCell(int x, int y)
{
    if (!IsValid(x, y))
    {
        return nullptr;
    }

    return &mCells[y][x];
}

void NavigationGrid::SetConnection(const glm::ivec2 &from, const glm::ivec2 &to, bool canMove)
{
    GridCell *fromCell = GetCell(from.x, from.y);
    GridCell *toCell = GetCell(to.x, to.y);
    if (fromCell == nullptr || toCell == nullptr)
    {
        return;
    }

    if (to.x == from.x && to.y == from.y - 1)
    {
        fromCell->canMoveNorth = canMove;
        toCell->canMoveSouth = canMove;
    }
    else if (to.x == from.x && to.y == from.y + 1)
    {
        fromCell->canMoveSouth = canMove;
        toCell->canMoveNorth = canMove;
    }
    else if (to.x == from.x + 1 && to.y == from.y)
    {
        fromCell->canMoveEast = canMove;
        toCell->canMoveWest = canMove;
    }
    else if (to.x == from.x - 1 && to.y == from.y)
    {
        fromCell->canMoveWest = canMove;
        toCell->canMoveEast = canMove;
    }
}

bool NavigationGrid::IsValid(int x, int y) const
{
    return (x >= 0 && x < mWidth &&
            y >= 0 && y < mHeight);
}