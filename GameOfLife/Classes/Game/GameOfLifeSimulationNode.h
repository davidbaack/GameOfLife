#ifndef _GAME_GAME_OF_LIFE_SIMULATION_NODE_H
#define _GAME_GAME_OF_LIFE_SIMULATION_NODE_H

#include "GridUtilities.h"
#include "ObjectPool.h"
#include "cocos2d.h"
#include <map>
#include <set>

namespace game
{
    
class GameOfLifeLivingCell;

class GameOfLifeSimulationNode final : public cocos2d::Node
{
    
public:
    
    CREATE_FUNC(GameOfLifeSimulationNode);
    
    void createCellsFromFile(const std::string& filePath);
    void createRandomCells(long long gridRange, long long numCells);
    void runSimulation(float tickInterval);
    bool doesGridCoordinateContainLivingCell(const GridUtilities::GridCoordinate& gridCoordinate);
    void createCell(const GridUtilities::GridCoordinate& gridCoordinate);
    void killCell(const GridUtilities::GridCoordinate& gridCoordinate);
    
private:
    
    // Use create instead of constructor
    GameOfLifeSimulationNode();
    // Nodes are reference counted, not explicitly deleted
    virtual ~GameOfLifeSimulationNode();
    
    void tickSimulation();
    
    void onPause();
    void onPlay();
    
    std::map<GridUtilities::GridCoordinate, GameOfLifeLivingCell*> mGridCoordinateToLivingCellMap;
    cocos2d::Action* mTickAction;
    std::shared_ptr<std::function<void()>> mPauseCallback;
    std::shared_ptr<std::function<void()>> mPlayCallback;
    engine::ObjectPool<GameOfLifeLivingCell> mLivingCellPool;
    
};
    
}

#endif // _GAME_GAME_OF_LIFE_SIMULATION_NODE_H
