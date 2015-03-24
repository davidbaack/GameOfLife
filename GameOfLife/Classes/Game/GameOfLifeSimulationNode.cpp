#include "GameOfLifeSimulationNode.h"
#include "GameOfLifeLivingCell.h"
#include "NotificationCenter.h"

using namespace game;
using namespace cocos2d;
using namespace std;

const string GameOfLifeSimulationNode::SIMULATION_TICK_NOTIFICATION = "Tick";

GameOfLifeSimulationNode::GameOfLifeSimulationNode()
    : mTickAction(nullptr)
{}

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

void GameOfLifeSimulationNode::addLivingCells(const std::vector<pair<int64_t, int64_t>>& gridCoordinates)
{
    for (const auto& gridCoordinate : gridCoordinates)
    {
        auto gameOfLifeLivingCell = GameOfLifeLivingCell::create();
        addChild(gameOfLifeLivingCell);
        mGridCoordinateToLivingCellMap.insert(make_pair(gridCoordinate, gameOfLifeLivingCell));
    }
}

void GameOfLifeSimulationNode::tickSimulation() const
{
    engine::NotificationCenter::getInstance().notify(SIMULATION_TICK_NOTIFICATION);
}
