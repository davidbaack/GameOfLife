#include "GameOfLifeLivingCell.h"
#include "GameOfLifeSimulationNode.h"
#include "GridUtilities.h"
#include "NotificationCenter.h"

using namespace game;
using namespace cocos2d;
using namespace std;

static const string SPRITE_NAME = "WhitePixel.png";

GameOfLifeLivingCell* GameOfLifeLivingCell::create(GameOfLifeSimulationNode& gameOfLifeSimulationNode, pair<int64_t, int64_t> gridCoordinate)
{
    GameOfLifeLivingCell* pRet = new(nothrow) GameOfLifeLivingCell(gameOfLifeSimulationNode, gridCoordinate);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        return nullptr;
    }
}

GameOfLifeLivingCell::GameOfLifeLivingCell(GameOfLifeSimulationNode& gameOfLifeSimulationNode, pair<int64_t, int64_t> gridCoordinate)
    : mGameOfLifeSimulationNode(gameOfLifeSimulationNode)
    , mGridCoordinate(gridCoordinate)
    , mWillDie(false)
{
    setPosition(gridCoordinate.first * 10, gridCoordinate.second * 10);
    
    mSprite = Sprite::create(SPRITE_NAME);
    mSprite->setScale(10, 10);
    addChild(mSprite);
    
    mSimulationTickBeginCallback = make_shared<function<void()>>(bind(&GameOfLifeLivingCell::onSimulationTickBegin, this));
    engine::NotificationCenter::getInstance().subscribe(GameOfLifeSimulationNode::SIMULATION_TICK_BEGIN_NOTIFICATION, mSimulationTickBeginCallback);
    mSimulationTickEndCallback = make_shared<function<void()>>(bind(&GameOfLifeLivingCell::onSimulationTickEnd, this));
    engine::NotificationCenter::getInstance().subscribe(GameOfLifeSimulationNode::SIMULATION_TICK_END_NOTIFICATION, mSimulationTickEndCallback);
}

GameOfLifeLivingCell::~GameOfLifeLivingCell()
{}

bool GameOfLifeLivingCell::shouldDie(const GameOfLifeSimulationNode& gameOfLifeSimulationNode, pair<int64_t, int64_t> gridCoordinate)
{
    if (!gameOfLifeSimulationNode.getLivingCellAtGridCoordinate(gridCoordinate))
    {
        return false;
    }
    
    auto numAdjacentLivingCells = 0;
    const auto& adjacentGridCoordinates = GridUtilities::getAdjacentGridCoordinates(gridCoordinate);
    for (auto adjacentGridCoordinate : adjacentGridCoordinates)
    {
        if (gameOfLifeSimulationNode.getLivingCellAtGridCoordinate(adjacentGridCoordinate))
        {
            ++numAdjacentLivingCells;
        }
    }
    return numAdjacentLivingCells < 2 || numAdjacentLivingCells > 3;
}

bool GameOfLifeLivingCell::shouldComeToLife(const GameOfLifeSimulationNode& gameOfLifeSimulationNode, pair<int64_t, int64_t> gridCoordinate)
{
    if (gameOfLifeSimulationNode.getLivingCellAtGridCoordinate(gridCoordinate))
    {
        return false;
    }
    
    auto numAdjacentLivingCells = 0;
    const auto& adjacentGridCoordinates = GridUtilities::getAdjacentGridCoordinates(gridCoordinate);
    for (auto adjacentGridCoordinate : adjacentGridCoordinates)
    {
        if (gameOfLifeSimulationNode.getLivingCellAtGridCoordinate(adjacentGridCoordinate))
        {
            ++numAdjacentLivingCells;
        }
    }
    return numAdjacentLivingCells == 3;
}

vector<pair<int64_t, int64_t>> GameOfLifeLivingCell::getGridCoordinatesShouldCreateCellsAt() const
{
    vector<pair<int64_t, int64_t>> gridCoordinatesShouldCreateCellsAt;
    const auto& adjacentGridCoordinates = GridUtilities::getAdjacentGridCoordinates(mGridCoordinate);
    for (auto adjacentGridCoordinate : adjacentGridCoordinates)
    {
        if (!mGameOfLifeSimulationNode.isGridCoordinateReservedForCreation(adjacentGridCoordinate) && shouldComeToLife(mGameOfLifeSimulationNode, adjacentGridCoordinate))
        {
            gridCoordinatesShouldCreateCellsAt.push_back(adjacentGridCoordinate);
        }
    }
    return gridCoordinatesShouldCreateCellsAt;
}

void GameOfLifeLivingCell::onSimulationTickBegin()
{
    mWillDie = shouldDie(mGameOfLifeSimulationNode, mGridCoordinate);
    mGridCoordinatesToCreateCellsAt = getGridCoordinatesShouldCreateCellsAt();
    for (auto gridCoordinate : mGridCoordinatesToCreateCellsAt)
    {
        mGameOfLifeSimulationNode.reserveGridCoordinateForCreation(gridCoordinate);
    }
}

void GameOfLifeLivingCell::onSimulationTickEnd()
{
    for (auto gridCoordinate : mGridCoordinatesToCreateCellsAt)
    {
        mGameOfLifeSimulationNode.createCell(gridCoordinate);
    }
    mGridCoordinatesToCreateCellsAt.clear();
    if (mWillDie)
    {
        mGameOfLifeSimulationNode.killCell(mGridCoordinate);
    }
}
