#include "CreateGliderOnTapNode.h"
#include "GameOfLifeSimulationNode.h"

using namespace game;
using namespace cocos2d;
using namespace std;

static const float TAP_MOVEMENT_THRESHOLD = 50.0f;

CreateGliderOnTapNode::CreateGliderOnTapNode()
    : mGameOfLifeSimulationNode(nullptr)
{
    // listen for touch events
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = bind(&CreateGliderOnTapNode::onTouchBegan, this, placeholders::_1, placeholders::_2);
    touchListener->onTouchEnded = bind(&CreateGliderOnTapNode::onTouchEnded, this, placeholders::_1, placeholders::_2);
    touchListener->onTouchCancelled = bind(&CreateGliderOnTapNode::onTouchCancelled, this, placeholders::_1, placeholders::_2);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

CreateGliderOnTapNode::~CreateGliderOnTapNode()
{}

void CreateGliderOnTapNode::setGameOfLifeSimulationNode(GameOfLifeSimulationNode* simulationNode)
{
    mGameOfLifeSimulationNode = simulationNode;
}

bool CreateGliderOnTapNode::onTouchBegan(Touch* touch, Event* unused_event)
{
    return true;
}

void CreateGliderOnTapNode::onTouchEnded(Touch* touch, Event* unused_event)
{
    createGliderIfTapGesture(touch);
}

void CreateGliderOnTapNode::onTouchCancelled(Touch* touch, Event* unused_event)
{
    createGliderIfTapGesture(touch);
}

void CreateGliderOnTapNode::createGliderIfTapGesture(Touch* touch) const
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
