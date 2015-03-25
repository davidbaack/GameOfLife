#ifndef _GAME_GRID_UTILITIES_H
#define _GAME_GRID_UTILITIES_H

namespace game
{

struct GridUtilities
{
    static constexpr int64_t INT_64_MAX_VALUE = 9223372036854775807;
    static constexpr int64_t INT_64_MIN_VALUE = -9223372036854775807;
    static constexpr int GRID_SPACE_SIZE = 10;
    
    using GridCoordinate = std::pair<int64_t, int64_t>;
    
    enum Direction
    {
        EAST = 0,
        SOUTHEAST = 1,
        SOUTH = 2,
        SOUTHWEST = 3,
        WEST = 4,
        NORTHWEST = 5,
        NORTH = 6,
        NORTHEAST = 7,
        COUNT = 8
    };
    
    // If we try to get an adjacent grid coordinate that is outside our maximum range, this will return the original gridCoordinate instead and set isOutsideOfGrid to true;
    static GridCoordinate getAdjacentGridCoordinateInDirection(const GridCoordinate& gridCoordinate, Direction direction, bool& isOutsideOfGrid);
    static std::vector<GridCoordinate> getAdjacentGridCoordinates(const GridCoordinate& gridCoordinate);
};
    
}

#endif // _GAME_GRID_UTILITIES_H
