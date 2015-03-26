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
    
    static GameOfLifeLivingCell* create();
    void setPositionForGridCoordinate(const GridUtilities::GridCoordinate& gridCoordinate);
    
private:
    
    // Use create instead of constructor
    GameOfLifeLivingCell();
    // Nodes are reference counted, not explicitly deleted
    virtual ~GameOfLifeLivingCell();
    
    cocos2d::Sprite* mSprite;
    
};
    
}

#endif // _GAME_GAME_OF_LIFE_LIVING_CELL_H
