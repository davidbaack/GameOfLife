#ifndef _GAME_GAME_OF_LIFE_LIVING_CELL_H
#define _GAME_GAME_OF_LIFE_LIVING_CELL_H

#include "cocos2d.h"

namespace game
{
    
class GameOfLifeSimulationNode;

class GameOfLifeLivingCell final : public cocos2d::Node
{
    
public:
    
    static GameOfLifeLivingCell* create(GameOfLifeSimulationNode& gameOfLifeSimulationNode, std::pair<int64_t, int64_t> gridCoordinate);
    static bool shouldDie(const GameOfLifeSimulationNode& gameOfLifeSimulationNode, std::pair<int64_t, int64_t> gridCoordinate);
    static bool shouldComeToLife(const GameOfLifeSimulationNode& gameOfLifeSimulationNode, std::pair<int64_t, int64_t> gridCoordinate);
    
    std::vector<std::pair<int64_t, int64_t>> getGridCoordinatesShouldCreateCellsAt() const;
    
private:
    
    // Use create instead of constructor
    GameOfLifeLivingCell(GameOfLifeSimulationNode& gameOfLifeSimulationNode, std::pair<int64_t, int64_t> gridCoordinate);
    // Nodes are reference counted, not explicitly deleted
    virtual ~GameOfLifeLivingCell();
    
    void onSimulationTickBegin();
    void onSimulationTickEnd();
    
    GameOfLifeSimulationNode& mGameOfLifeSimulationNode;
    std::pair<int64_t, int64_t> mGridCoordinate;
    cocos2d::Sprite* mSprite;
    std::vector<std::pair<int64_t, int64_t>> mGridCoordinatesToCreateCellsAt;
    bool mWillDie;
    std::shared_ptr<std::function<void()>> mSimulationTickBeginCallback;
    std::shared_ptr<std::function<void()>> mSimulationTickEndCallback;
    
};
    
}

#endif // _GAME_GAME_OF_LIFE_LIVING_CELL_H
