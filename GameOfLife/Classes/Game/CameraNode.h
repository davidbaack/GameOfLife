#ifndef _GAME_CAMERA_NODE_H
#define _GAME_CAMERA_NODE_H

namespace game
{

class CameraNode final : public cocos2d::Node
{
    
public:
    
    CREATE_FUNC(CameraNode);
    
    static const std::string CAMERA_MOVEMENT_BEGIN;
    static const std::string CAMERA_MOVEMENT_END;
    
private:
    
    // Use create instead of constructor
    CameraNode();
    // Nodes are reference counted, not explicitly deleted
    virtual ~CameraNode();
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* unused_event);
    
};
    
}

#endif // _GAME_CAMERA_NODE_H
