#ifndef _GAME_GAME_OF_LIFE_SIMULATION_NODE_H
#define _GAME_GAME_OF_LIFE_SIMULATION_NODE_H

#include "cocos2d.h"
#include <map>

namespace game
{
    
class GameOfLifeLivingCell;

class GameOfLifeSimulationNode final : public cocos2d::Node
{
    
public:
    
    CREATE_FUNC(GameOfLifeSimulationNode);
    
    void runSimulation(float tickInterval);
    void addLivingCells(const std::vector<std::pair<int64_t, int64_t>>& gridCoordinates);
    
    static const std::string SIMULATION_TICK_NOTIFICATION;
    
private:
    
    // Use create instead of constructor
    GameOfLifeSimulationNode();
    // Nodes are reference counted, not explicitly deleted
    virtual ~GameOfLifeSimulationNode();
    
    void tickSimulation() const;
    
    std::map<std::pair<int64_t, int64_t>, GameOfLifeLivingCell*> mGridCoordinateToLivingCellMap;
    cocos2d::Action* mTickAction;
    
};
    
}

#endif // _GAME_GAME_OF_LIFE_SIMULATION_NODE_H
