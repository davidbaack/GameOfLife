#ifndef _GAME_GRID_UTILITIES_H
#define _GAME_GRID_UTILITIES_H

namespace game
{

struct GridUtilities
{
    static constexpr int GRID_SPACE_SIZE = 10;
    
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
    static std::pair<int64_t, int64_t> getAdjacentGridCoordinateInDirection(const std::pair<int64_t, int64_t>& gridCoordinate, Direction direction, bool& isOutsideOfGrid);
    static std::vector<std::pair<int64_t, int64_t>> getAdjacentGridCoordinates(const std::pair<int64_t, int64_t>& gridCoordinate);
};
    
}

#endif // _GAME_GRID_UTILITIES_H
