#include "GridUtilities.h"
#include "limits.h"

using namespace game;
using namespace std;

GridUtilities::GridCoordinate GridUtilities::getAdjacentGridCoordinateInDirection(const GridCoordinate& gridCoordinate, Direction direction, bool& isOutsideOfGrid)
{
    isOutsideOfGrid = true;
    
    GridCoordinate adjacentGridCoordinate = gridCoordinate;
    if (direction == EAST)
    {
        if (gridCoordinate.first < MAX_GRID_INDEX)
        {
            ++adjacentGridCoordinate.first;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == SOUTHEAST)
    {
        if (gridCoordinate.first < MAX_GRID_INDEX && gridCoordinate.second > MIN_GRID_INDEX)
        {
            ++adjacentGridCoordinate.first;
            --adjacentGridCoordinate.second;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == SOUTH)
    {
        if (gridCoordinate.second > MIN_GRID_INDEX)
        {
            --adjacentGridCoordinate.second;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == SOUTHWEST)
    {
        if (gridCoordinate.first > MIN_GRID_INDEX && gridCoordinate.second > MIN_GRID_INDEX)
        {
            --adjacentGridCoordinate.first;
            --adjacentGridCoordinate.second;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == WEST)
    {
        if (gridCoordinate.first > MIN_GRID_INDEX)
        {
            --adjacentGridCoordinate.first;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == NORTHWEST)
    {
        if (gridCoordinate.first > MIN_GRID_INDEX && gridCoordinate.second < MAX_GRID_INDEX)
        {
            --adjacentGridCoordinate.first;
            ++adjacentGridCoordinate.second;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == NORTH)
    {
        if (gridCoordinate.second < MAX_GRID_INDEX)
        {
            ++adjacentGridCoordinate.second;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == NORTHEAST)
    {
        if (gridCoordinate.first < MAX_GRID_INDEX && gridCoordinate.second < MAX_GRID_INDEX)
        {
            ++adjacentGridCoordinate.first;
            ++adjacentGridCoordinate.second;
            isOutsideOfGrid = false;
        }
    }
    
    return adjacentGridCoordinate;
}

vector<GridUtilities::GridCoordinate> GridUtilities::getAdjacentGridCoordinates(const GridCoordinate& gridCoordinate)
{
    vector<GridCoordinate> adjacentGridCoordinates;
    bool isOutsideOfGrid;
    for (auto direction = 0; direction < GridUtilities::Direction::COUNT; ++direction)
    {
        const auto& adjacentGridCoordinate = GridUtilities::getAdjacentGridCoordinateInDirection(gridCoordinate, static_cast<GridUtilities::Direction>(direction), isOutsideOfGrid);
        if (!isOutsideOfGrid)
        {
            adjacentGridCoordinates.push_back(adjacentGridCoordinate);
        }
    }
    return adjacentGridCoordinates;
}

GridUtilities::GridCoordinate GridUtilities::getGridCoordinateForLocation(const cocos2d::Vec2& location)
{
    return GridUtilities::GridCoordinate(location.x / GRID_SPACE_SIZE, location.y / GRID_SPACE_SIZE);
}
