#ifndef _GAME_GRID_UTILITIES_H
#define _GAME_GRID_UTILITIES_H

namespace game
{

struct GridUtilities
{
    static std::vector<std::pair<int64_t, int64_t>> getAdjacentGridCoordinates(std::pair<int64_t, int64_t> gridCoordinate);
};
    
}

#endif // _GAME_GRID_UTILITIES_H
