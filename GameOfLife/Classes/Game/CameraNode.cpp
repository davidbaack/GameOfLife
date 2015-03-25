#include "CameraNode.h"
#include "NotificationCenter.h"

using namespace game;
using namespace cocos2d;
using namespace std;

const string CameraNode::CAMERA_MOVEMENT_BEGIN = "CameraMovementBegin";
const string CameraNode::CAMERA_MOVEMENT_END = "CameraMovementEnd";

CameraNode::CameraNode()
{
    // listen for touch events
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = bind(&CameraNode::onTouchBegan, this, placeholders::_1, placeholders::_2);
    touchListener->onTouchMoved = bind(&CameraNode::onTouchMoved, this, placeholders::_1, placeholders::_2);
    touchListener->onTouchEnded = bind(&CameraNode::onTouchEnded, this, placeholders::_1, placeholders::_2);
    touchListener->onTouchCancelled = bind(&CameraNode::onTouchCancelled, this, placeholders::_1, placeholders::_2);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

CameraNode::~CameraNode()
{}

bool CameraNode::onTouchBegan(Touch* touch, Event* unused_event)
{
    engine::NotificationCenter::getInstance().notify(CAMERA_MOVEMENT_BEGIN);
    return true;
}

void CameraNode::onTouchMoved(Touch* touch, Event* unused_event)
{
    Vec2 offset = touch->getLocation() - touch->getPreviousLocation();
    setPosition(getPosition() + offset);
}

void CameraNode::onTouchEnded(Touch* touch, Event* unused_event)
{
    engine::NotificationCenter::getInstance().notify(CAMERA_MOVEMENT_END);
}

void CameraNode::onTouchCancelled(Touch* touch, Event* unused_event)
{
    engine::NotificationCenter::getInstance().notify(CAMERA_MOVEMENT_END);
}
