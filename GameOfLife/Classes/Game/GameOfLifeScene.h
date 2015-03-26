#ifndef _GAME_GAME_OF_LIFE_SCENE_H
#define _GAME_GAME_OF_LIFE_SCENE_H

#include "cocos2d.h"

namespace game
{
    
class GameOfLifeSimulationNode;

class GameOfLifeScene : public cocos2d::Layer
{
    
public:
    
    static cocos2d::Scene* createScene();

    virtual bool init() override;
    
private:
    
    CREATE_FUNC(GameOfLifeScene);
    
    void menuCloseCallback(cocos2d::Ref* sender);
    void spawnRandomCells(GameOfLifeSimulationNode* gameOfLifeSimulationNode, long long gridRange, long long numCells);
    
    GameOfLifeSimulationNode* mGameOfLifeSimulationNode;
    
};
    
}

#endif // _GAME_GAME_OF_LIFE_SCENE_H
