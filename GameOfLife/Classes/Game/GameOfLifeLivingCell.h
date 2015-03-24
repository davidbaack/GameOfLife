#ifndef _GAME_GAME_OF_LIFE_LIVING_CELL_H
#define _GAME_GAME_OF_LIFE_LIVING_CELL_H

#include "cocos2d.h"

namespace game
{

class GameOfLifeLivingCell final : public cocos2d::Node
{
    
public:
    
    CREATE_FUNC(GameOfLifeLivingCell);
    
private:
    
    // Use create instead of constructor
    GameOfLifeLivingCell();
    // Nodes are reference counted, not explicitly deleted
    virtual ~GameOfLifeLivingCell();
    
    void onSimulationTick();
    
    cocos2d::Sprite* mSprite;
    std::shared_ptr<std::function<void()>> mSimulationTickCallback;
    
};
    
}

#endif // _GAME_GAME_OF_LIFE_LIVING_CELL_H
