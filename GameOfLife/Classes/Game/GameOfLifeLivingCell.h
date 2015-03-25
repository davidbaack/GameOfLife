#ifndef _GAME_GAME_OF_LIFE_LIVING_CELL_H
#define _GAME_GAME_OF_LIFE_LIVING_CELL_H

#include "PoolableNode.h"
#include "GridUtilities.h"

namespace game
{
    
class GameOfLifeSimulationNode;

class GameOfLifeLivingCell final : public engine::PoolableNode
{
    
public:
    
    virtual void onPutInPool() override;
    virtual void onRetrieveFromPool() override;
    
    static GameOfLifeLivingCell* create(GameOfLifeSimulationNode& gameOfLifeSimulationNode, const GridUtilities::GridCoordinate& gridCoordinate);
    static bool shouldDie(const GameOfLifeSimulationNode& gameOfLifeSimulationNode, const GridUtilities::GridCoordinate& gridCoordinate);
    static bool shouldComeToLife(const GameOfLifeSimulationNode& gameOfLifeSimulationNode, const GridUtilities::GridCoordinate& gridCoordinate);
    
    void setGridCoordinate(const GridUtilities::GridCoordinate& gridCoordinate);
    std::vector<GridUtilities::GridCoordinate> attemptLivingCellCreationInAdjacentCells() const;
    
private:
    
    // Use create instead of constructor
    GameOfLifeLivingCell(GameOfLifeSimulationNode& gameOfLifeSimulationNode, const GridUtilities::GridCoordinate& gridCoordinate);
    // Nodes are reference counted, not explicitly deleted
    virtual ~GameOfLifeLivingCell();
    
    void onSimulationTickBegin();
    void onSimulationTickEnd();
    
    GameOfLifeSimulationNode& mGameOfLifeSimulationNode;
    GridUtilities::GridCoordinate mGridCoordinate;
    cocos2d::Sprite* mSprite;
    std::vector<GridUtilities::GridCoordinate> mGridCoordinatesToCreateCellsAt;
    bool mWillDie;
    std::shared_ptr<std::function<void()>> mSimulationTickBeginCallback;
    std::shared_ptr<std::function<void()>> mSimulationTickEndCallback;
    
};
    
}

#endif // _GAME_GAME_OF_LIFE_LIVING_CELL_H
