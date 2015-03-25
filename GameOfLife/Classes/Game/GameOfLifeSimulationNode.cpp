#include "GameOfLifeSimulationNode.h"
#include "GameOfLifeLivingCell.h"
#include "NotificationCenter.h"
#include "CameraNode.h"
#include <chrono>

using namespace game;
using namespace cocos2d;
using namespace std;

const string GameOfLifeSimulationNode::SIMULATION_TICK_BEGIN_NOTIFICATION = "TickBegin";
const string GameOfLifeSimulationNode::SIMULATION_TICK_END_NOTIFICATION = "TickEnd";

GameOfLifeSimulationNode::GameOfLifeSimulationNode()
    : mTickAction(nullptr)
{
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

GameOfLifeLivingCell* GameOfLifeSimulationNode::getLivingCellAtGridCoordinate(const GridUtilities::GridCoordinate& gridCoordinate) const
{
    if (mGridCoordinateToLivingCellMap.count(gridCoordinate) == 0)
    {
        return nullptr;
    }
    return mGridCoordinateToLivingCellMap.at(gridCoordinate);
}

bool GameOfLifeSimulationNode::hasBeenCheckedForCellCreation(const GridUtilities::GridCoordinate& gridCoordinate) const
{
    return mGridCoordinateCheckedForCellCreationSet.count(gridCoordinate) > 0;
}

void GameOfLifeSimulationNode::markHasBeenCheckedForCellCreation(const GridUtilities::GridCoordinate& gridCoordinate)
{
    mGridCoordinateCheckedForCellCreationSet.insert(gridCoordinate);
}

void GameOfLifeSimulationNode::createCell(const GridUtilities::GridCoordinate& gridCoordinate)
{
    // Check if this cell is already alive
    if (mGridCoordinateToLivingCellMap.count(gridCoordinate) > 0)
    {
        printf("Warning: trying to create a cell in a grid space that's already occupied\n");
        return;
    }
    auto gameOfLifeLivingCell = mLivingCellPool.retrieveFromPool();
    if (gameOfLifeLivingCell)
    {
        gameOfLifeLivingCell->setGridCoordinate(gridCoordinate);
    }
    else
    {
        gameOfLifeLivingCell = GameOfLifeLivingCell::create(*this, gridCoordinate);
        addChild(gameOfLifeLivingCell);
    }
    mGridCoordinateToLivingCellMap[gridCoordinate] = gameOfLifeLivingCell;
}

void GameOfLifeSimulationNode::killCell(const GridUtilities::GridCoordinate& gridCoordinate)
{
    auto iter = mGridCoordinateToLivingCellMap.find(gridCoordinate);
    // Check if this cell is already dead
    if (iter == mGridCoordinateToLivingCellMap.end())
    {
        printf("Warning: trying to kill a cell that has already been killed\n");
        return;
    }
    auto gameOfLifeLivingCell = iter->second;
    mLivingCellPool.putInPool(gameOfLifeLivingCell);
    mGridCoordinateToLivingCellMap.erase(iter);
}

void GameOfLifeSimulationNode::tickSimulation()
{
    chrono::high_resolution_clock::time_point timeSimulationStart = chrono::high_resolution_clock::now();
    
    engine::NotificationCenter::getInstance().notify(SIMULATION_TICK_BEGIN_NOTIFICATION);
    engine::NotificationCenter::getInstance().notify(SIMULATION_TICK_END_NOTIFICATION);
    mGridCoordinateCheckedForCellCreationSet.clear();
    
    chrono::high_resolution_clock::time_point timeSimulationEnd = chrono::high_resolution_clock::now();
    
    chrono::milliseconds timeTaken = std::chrono::duration_cast<chrono::milliseconds>(timeSimulationEnd - timeSimulationStart);
    
    printf("Simulation tick time taken: %lli milliseconds\n", timeTaken.count());
}

void GameOfLifeSimulationNode::onCameraMovementBegin()
{
    //pause();
}

void GameOfLifeSimulationNode::onCameraMovementEnd()
{
    //resume();
}
