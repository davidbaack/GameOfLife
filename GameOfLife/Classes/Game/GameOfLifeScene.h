#ifndef _GAME_GAME_OF_LIFE_SCENE_H
#define _GAME_GAME_OF_LIFE_SCENE_H

#include "cocos2d.h"

namespace game
{
    
class GameOfLifeSimulationNode;
class CameraNode;
class CreateGliderOnTapNode;

class GameOfLifeScene : public cocos2d::Layer
{
    
public:
    
    static cocos2d::Scene* createScene();

    virtual bool init() override;
    
    static const std::string PAUSE;
    static const std::string PLAY;
    
private:
    
    CREATE_FUNC(GameOfLifeScene);
    
    void resetSimulation(CameraNode* cameraNode, CreateGliderOnTapNode* createGliderOnTapNode);
    
    cocos2d::MenuItemImage* mPauseButton;
    cocos2d::MenuItemImage* mPlayButton;
    GameOfLifeSimulationNode* mGameOfLifeSimulationNode;
    bool mIsSimulationPaused;
    
};
    
}

#endif // _GAME_GAME_OF_LIFE_SCENE_H
