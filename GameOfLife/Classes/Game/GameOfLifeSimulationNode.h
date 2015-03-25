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
    
    void runSimulation(float tickInterval);
    GameOfLifeLivingCell* getLivingCellAtGridCoordinate(const GridUtilities::GridCoordinate& gridCoordinate) const;
    bool hasBeenCheckedForCellCreation(const GridUtilities::GridCoordinate& gridCoordinate) const;
    void markHasBeenCheckedForCellCreation(const GridUtilities::GridCoordinate& gridCoordinate);
    void createCell(const GridUtilities::GridCoordinate& gridCoordinate);
    void killCell(const GridUtilities::GridCoordinate& gridCoordinate);
    
    static const std::string SIMULATION_TICK_BEGIN_NOTIFICATION;
    static const std::string SIMULATION_TICK_END_NOTIFICATION;
    
private:
    
    // Use create instead of constructor
    GameOfLifeSimulationNode();
    // Nodes are reference counted, not explicitly deleted
    virtual ~GameOfLifeSimulationNode();
    
    void tickSimulation();
    
    void onCameraMovementBegin();
    void onCameraMovementEnd();
    
    std::set<GridUtilities::GridCoordinate> mGridCoordinateCheckedForCellCreationSet;
    std::map<GridUtilities::GridCoordinate, GameOfLifeLivingCell*> mGridCoordinateToLivingCellMap;
    cocos2d::Action* mTickAction;
    std::shared_ptr<std::function<void()>> mCameraMovementBeginCallback;
    std::shared_ptr<std::function<void()>> mCameraMovementEndCallback;
    engine::ObjectPool<GameOfLifeLivingCell> mLivingCellPool;
    
};
    
}

#endif // _GAME_GAME_OF_LIFE_SIMULATION_NODE_H
