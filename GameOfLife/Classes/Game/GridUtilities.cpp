#include "GridUtilities.h"

using namespace game;
using namespace std;

vector<pair<int64_t, int64_t>> GridUtilities::getAdjacentGridCoordinates(pair<int64_t, int64_t> gridCoordinate)
{
    vector<pair<int64_t, int64_t>> adjacentGridCoordinates;
    auto adjacentGridCoordinate = gridCoordinate;
    
    // East
    ++adjacentGridCoordinate.first;
    adjacentGridCoordinates.push_back(adjacentGridCoordinate);
    // SouthEast
    --adjacentGridCoordinate.second;
    adjacentGridCoordinates.push_back(adjacentGridCoordinate);
    // South
    --adjacentGridCoordinate.first;
    adjacentGridCoordinates.push_back(adjacentGridCoordinate);
    // SouthWest
    --adjacentGridCoordinate.first;
    adjacentGridCoordinates.push_back(adjacentGridCoordinate);
    // West
    ++adjacentGridCoordinate.second;
    adjacentGridCoordinates.push_back(adjacentGridCoordinate);
    // NorthWest
    ++adjacentGridCoordinate.second;
    adjacentGridCoordinates.push_back(adjacentGridCoordinate);
    // North
    ++adjacentGridCoordinate.first;
    adjacentGridCoordinates.push_back(adjacentGridCoordinate);
    // NorthEast
    ++adjacentGridCoordinate.first;
    adjacentGridCoordinates.push_back(adjacentGridCoordinate);
    
    return adjacentGridCoordinates;
}
