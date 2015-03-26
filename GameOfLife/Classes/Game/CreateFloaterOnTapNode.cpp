#include "CreateFloaterOnTapNode.h"
#include "GameOfLifeSimulationNode.h"

using namespace game;
using namespace cocos2d;
using namespace std;

static const float TAP_MOVEMENT_THRESHOLD = 50.0f;

CreateFloaterOnTapNode::CreateFloaterOnTapNode()
    : mGameOfLifeSimulationNode(nullptr)
{
    // listen for touch events
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = bind(&CreateFloaterOnTapNode::onTouchBegan, this, placeholders::_1, placeholders::_2);
    touchListener->onTouchEnded = bind(&CreateFloaterOnTapNode::onTouchEnded, this, placeholders::_1, placeholders::_2);
    touchListener->onTouchCancelled = bind(&CreateFloaterOnTapNode::onTouchCancelled, this, placeholders::_1, placeholders::_2);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

CreateFloaterOnTapNode::~CreateFloaterOnTapNode()
{}

void CreateFloaterOnTapNode::setGameOfLifeSimulationNode(GameOfLifeSimulationNode* simulationNode)
{
    mGameOfLifeSimulationNode = simulationNode;
}

bool CreateFloaterOnTapNode::onTouchBegan(Touch* touch, Event* unused_event)
{
    return true;
}

void CreateFloaterOnTapNode::onTouchEnded(Touch* touch, Event* unused_event)
{
    createFloaterIfTapGesture(touch);
}

void CreateFloaterOnTapNode::onTouchCancelled(Touch* touch, Event* unused_event)
{
    createFloaterIfTapGesture(touch);
}

void CreateFloaterOnTapNode::createFloaterIfTapGesture(Touch* touch) const
{
    if (mGameOfLifeSimulationNode && fabs(touch->getLocation().getDistance(touch->getStartLocation())) < TAP_MOVEMENT_THRESHOLD)
    {
        auto tappedSimulationLocation = mGameOfLifeSimulationNode->getParent()->convertToNodeSpace(touch->getLocation());
        GridUtilities::GridCoordinate tappedGridCoordinate = GridUtilities::getGridCoordinateForLocation(tappedSimulationLocation);
        mGameOfLifeSimulationNode->createCell(GridUtilities::GridCoordinate(tappedGridCoordinate.first, tappedGridCoordinate.second));
        mGameOfLifeSimulationNode->createCell(GridUtilities::GridCoordinate(tappedGridCoordinate.first + 1, tappedGridCoordinate.second));
        mGameOfLifeSimulationNode->createCell(GridUtilities::GridCoordinate(tappedGridCoordinate.first + 2, tappedGridCoordinate.second));
        mGameOfLifeSimulationNode->createCell(GridUtilities::GridCoordinate(tappedGridCoordinate.first + 2, tappedGridCoordinate.second + 1));
        mGameOfLifeSimulationNode->createCell(GridUtilities::GridCoordinate(tappedGridCoordinate.first + 1, tappedGridCoordinate.second + 2));
        
    }
}
