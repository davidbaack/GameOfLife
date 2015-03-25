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
        if (gridCoordinate.first < INT_64_MAX_VALUE)
        {
            ++adjacentGridCoordinate.first;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == SOUTHEAST)
    {
        if (gridCoordinate.first < INT_64_MAX_VALUE && gridCoordinate.second > INT_64_MIN_VALUE)
        {
            ++adjacentGridCoordinate.first;
            --adjacentGridCoordinate.second;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == SOUTH)
    {
        if (gridCoordinate.second > INT_64_MIN_VALUE)
        {
            --adjacentGridCoordinate.second;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == SOUTHWEST)
    {
        if (gridCoordinate.first > INT_64_MIN_VALUE && gridCoordinate.second > INT_64_MIN_VALUE)
        {
            --adjacentGridCoordinate.first;
            --adjacentGridCoordinate.second;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == WEST)
    {
        if (gridCoordinate.first > INT_64_MIN_VALUE)
        {
            --adjacentGridCoordinate.first;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == NORTHWEST)
    {
        if (gridCoordinate.first > INT_64_MIN_VALUE && gridCoordinate.second < INT_64_MAX_VALUE)
        {
            --adjacentGridCoordinate.first;
            ++adjacentGridCoordinate.second;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == NORTH)
    {
        if (gridCoordinate.second < INT_64_MAX_VALUE)
        {
            ++adjacentGridCoordinate.second;
            isOutsideOfGrid = false;
        }
    }
    else if (direction == NORTHEAST)
    {
        if (gridCoordinate.first < INT_64_MAX_VALUE && gridCoordinate.second < INT_64_MAX_VALUE)
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
