#ifndef _GAME_GAME_OF_LIFE_SIMULATION_NODE_H
#define _GAME_GAME_OF_LIFE_SIMULATION_NODE_H

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
    GameOfLifeLivingCell* getLivingCellAtGridCoordinate(std::pair<int64_t, int64_t> gridCoordinate) const;
    bool isGridCoordinateReservedForCreation(std::pair<int64_t, int64_t> gridCoordinate) const;
    void reserveGridCoordinateForCreation(std::pair<int64_t, int64_t> gridCoordinate);
    void createCell(std::pair<int64_t, int64_t> gridCoordinate);
    void killCell(std::pair<int64_t, int64_t> gridCoordinate);
    
    static const std::string SIMULATION_TICK_BEGIN_NOTIFICATION;
    static const std::string SIMULATION_TICK_END_NOTIFICATION;
    
private:
    
    // Use create instead of constructor
    GameOfLifeSimulationNode();
    // Nodes are reference counted, not explicitly deleted
    virtual ~GameOfLifeSimulationNode();
    
    void tickSimulation() const;
    void onSimulationTickEnd();
    
    std::set<std::pair<int64_t, int64_t>> mReservedGridCoordinateForCellCreationSet;
    std::map<std::pair<int64_t, int64_t>, GameOfLifeLivingCell*> mGridCoordinateToLivingCellMap;
    cocos2d::Action* mTickAction;
    std::shared_ptr<std::function<void()>> mSimulationTickEndCallback;
    
};
    
}

#endif // _GAME_GAME_OF_LIFE_SIMULATION_NODE_H
