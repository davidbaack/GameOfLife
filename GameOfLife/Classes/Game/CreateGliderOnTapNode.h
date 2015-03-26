#ifndef _GAME_CREATE_GLIDER_ON_TAP_NODE_H
#define _GAME_CREATE_GLIDER_ON_TAP_NODE_H

namespace game
{

class GameOfLifeSimulationNode;

class CreateGliderOnTapNode final : public cocos2d::Node
{
    
public:
    
    CREATE_FUNC(CreateGliderOnTapNode);
    
    void setGameOfLifeSimulationNode(GameOfLifeSimulationNode* simulationNode);
    
    static const std::string CAMERA_MOVEMENT_BEGIN;
    static const std::string CAMERA_MOVEMENT_END;
    
private:
    
    // Use create instead of constructor
    CreateGliderOnTapNode();
    // Nodes are reference counted, not explicitly deleted
    virtual ~CreateGliderOnTapNode();
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* unused_event);
    void createGliderIfTapGesture(cocos2d::Touch* touch) const;
    
    GameOfLifeSimulationNode* mGameOfLifeSimulationNode;
    
};
    
}

#endif // _GAME_CREATE_GLIDER_ON_TAP_NODE_H
