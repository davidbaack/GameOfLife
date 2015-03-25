#include "GameOfLifeSimulationNode.h"
#include "GameOfLifeLivingCell.h"
#include "NotificationCenter.h"
#include "CameraNode.h"

using namespace game;
using namespace cocos2d;
using namespace std;

const string GameOfLifeSimulationNode::SIMULATION_TICK_BEGIN_NOTIFICATION = "TickBegin";
const string GameOfLifeSimulationNode::SIMULATION_TICK_END_NOTIFICATION = "TickEnd";

GameOfLifeSimulationNode::GameOfLifeSimulationNode()
    : mTickAction(nullptr)
{
    mSimulationTickEndCallback = make_shared<function<void()>>(bind(&GameOfLifeSimulationNode::onSimulationTickEnd, this));
    engine::NotificationCenter::getInstance().subscribe(GameOfLifeSimulationNode::SIMULATION_TICK_END_NOTIFICATION, mSimulationTickEndCallback);
    mCameraMovementBeginCallback = make_shared<function<void()>>(bind(&GameOfLifeSimulationNode::onCameraMovementBegin, this));
    engine::NotificationCenter::getInstance().subscribe(CameraNode::CAMERA_MOVEMENT_BEGIN, mCameraMovementBeginCallback);
    mCameraMovementEndCallback = make_shared<function<void()>>(bind(&GameOfLifeSimulationNode::onCameraMovementEnd, this));
    engine::NotificationCenter::getInstance().subscribe(CameraNode::CAMERA_MOVEMENT_END, mCameraMovementEndCallback);
}

GameOfLifeSimulationNode::~GameOfLifeSimulationNode()
{}

void GameOfLifeSimulationNode::runSimulation(float tickInterval)
{
    if (mTickAction)
    {
        stopAction(mTickAction);
    }
    mTickAction = RepeatForever::create(Sequence::createWithTwoActions(DelayTime::create(tickInterval), CallFunc::create(bind(&GameOfLifeSimulationNode::tickSimulation, this))));
    runAction(mTickAction);
}

GameOfLifeLivingCell* GameOfLifeSimulationNode::getLivingCellAtGridCoordinate(const pair<int64_t, int64_t>& gridCoordinate) const
{
    if (mGridCoordinateToLivingCellMap.count(gridCoordinate) == 0)
    {
        return nullptr;
    }
    return mGridCoordinateToLivingCellMap.at(gridCoordinate);
}

bool GameOfLifeSimulationNode::hasBeenCheckedForCellCreation(const pair<int64_t, int64_t>& gridCoordinate) const
{
    return mGridCoordinateCheckedForCellCreationSet.count(gridCoordinate) > 0;
}

void GameOfLifeSimulationNode::markHasBeenCheckedForCellCreation(const pair<int64_t, int64_t>& gridCoordinate)
{
    mGridCoordinateCheckedForCellCreationSet.insert(gridCoordinate);
}

void GameOfLifeSimulationNode::createCell(const pair<int64_t, int64_t>& gridCoordinate)
{
    if (mGridCoordinateToLivingCellMap.count(gridCoordinate) > 0)
    {
        return;
    }
    auto gameOfLifeLivingCell = GameOfLifeLivingCell::create(*this, gridCoordinate);
    mGridCoordinateToLivingCellMap.insert(make_pair(gridCoordinate, gameOfLifeLivingCell));
    addChild(gameOfLifeLivingCell);
}

void GameOfLifeSimulationNode::killCell(const pair<int64_t, int64_t>& gridCoordinate)
{
    if (mGridCoordinateToLivingCellMap.count(gridCoordinate) == 0)
    {
        return;
    }
    auto gameOfLifeLivingCell = mGridCoordinateToLivingCellMap.at(gridCoordinate);
    mGridCoordinateToLivingCellMap.erase(gridCoordinate);
    gameOfLifeLivingCell->removeFromParent();
}

void GameOfLifeSimulationNode::tickSimulation() const
{
    engine::NotificationCenter::getInstance().notify(SIMULATION_TICK_BEGIN_NOTIFICATION);
    engine::NotificationCenter::getInstance().notify(SIMULATION_TICK_END_NOTIFICATION);
}

void GameOfLifeSimulationNode::onSimulationTickEnd()
{
    mGridCoordinateCheckedForCellCreationSet.clear();
}

void GameOfLifeSimulationNode::onCameraMovementBegin()
{
    //pause();
}

void GameOfLifeSimulationNode::onCameraMovementEnd()
{
    //resume();
}
