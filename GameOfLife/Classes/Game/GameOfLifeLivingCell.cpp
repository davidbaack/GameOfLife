#include "GameOfLifeLivingCell.h"
#include "GameOfLifeSimulationNode.h"
#include "NotificationCenter.h"

using namespace game;
using namespace cocos2d;
using namespace std;

static const string SPRITE_NAME = "WhitePixel.png";

void GameOfLifeLivingCell::onPutInPool()
{
    PoolableNode::onPutInPool();
    setVisible(false);
}

void GameOfLifeLivingCell::onRetrieveFromPool()
{
    PoolableNode::onRetrieveFromPool();
    setVisible(true);
    mWillDie = false;
    mGridCoordinatesToCreateCellsAt.clear();
}

GameOfLifeLivingCell* GameOfLifeLivingCell::create(GameOfLifeSimulationNode& gameOfLifeSimulationNode, const GridUtilities::GridCoordinate& gridCoordinate)
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

GameOfLifeLivingCell::GameOfLifeLivingCell(GameOfLifeSimulationNode& gameOfLifeSimulationNode, const GridUtilities::GridCoordinate& gridCoordinate)
    : mGameOfLifeSimulationNode(gameOfLifeSimulationNode)
    , mWillDie(false)
{
    setGridCoordinate(gridCoordinate);
    
    mSprite = Sprite::create(SPRITE_NAME);
    mSprite->setScale(GridUtilities::GRID_SPACE_SIZE);
    addChild(mSprite);
    
    mSimulationTickBeginCallback = make_shared<function<void()>>(bind(&GameOfLifeLivingCell::onSimulationTickBegin, this));
    engine::NotificationCenter::getInstance().subscribe(GameOfLifeSimulationNode::SIMULATION_TICK_BEGIN_NOTIFICATION, mSimulationTickBeginCallback);
    mSimulationTickEndCallback = make_shared<function<void()>>(bind(&GameOfLifeLivingCell::onSimulationTickEnd, this));
    engine::NotificationCenter::getInstance().subscribe(GameOfLifeSimulationNode::SIMULATION_TICK_END_NOTIFICATION, mSimulationTickEndCallback);
}

GameOfLifeLivingCell::~GameOfLifeLivingCell()
{}

void GameOfLifeLivingCell::setGridCoordinate(const GridUtilities::GridCoordinate& gridCoordinate)
{
    mGridCoordinate = gridCoordinate;
    setPosition(gridCoordinate.first * GridUtilities::GRID_SPACE_SIZE, gridCoordinate.second * GridUtilities::GRID_SPACE_SIZE);
}

bool GameOfLifeLivingCell::shouldDie(const GameOfLifeSimulationNode& gameOfLifeSimulationNode, const GridUtilities::GridCoordinate& gridCoordinate)
{
    // Check if we're already dead
    if (!gameOfLifeSimulationNode.getLivingCellAtGridCoordinate(gridCoordinate))
    {
        printf("Warning: checking if a cell that is already dead should die\n");
        return false;
    }
    // Die if the number of adjacent living cells isn't 2 or 3
    auto numAdjacentLivingCells = 0;
    bool isOutsideOfGrid;
    for (auto direction = 0; direction < GridUtilities::Direction::COUNT; ++direction)
    {
        const auto& adjacentGridCoordinate = GridUtilities::getAdjacentGridCoordinateInDirection(gridCoordinate, static_cast<GridUtilities::Direction>(direction), isOutsideOfGrid);
        if (!isOutsideOfGrid && gameOfLifeSimulationNode.getLivingCellAtGridCoordinate(adjacentGridCoordinate))
        {
            ++numAdjacentLivingCells;
            if (numAdjacentLivingCells > 3)
            {
                return true;
            }
        }
    }
    
    return numAdjacentLivingCells < 2;
}

bool GameOfLifeLivingCell::shouldComeToLife(const GameOfLifeSimulationNode& gameOfLifeSimulationNode, const GridUtilities::GridCoordinate& gridCoordinate)
{
    // Check if we're already alive
    if (gameOfLifeSimulationNode.getLivingCellAtGridCoordinate(gridCoordinate))
    {
        return false;
    }
    // Come to life if the number of adjacent living cells is 3
    const auto& adjacentGridCoordinates = GridUtilities::getAdjacentGridCoordinates(gridCoordinate);
    auto numAdjacentLivingCells = 0;
    auto numCoordinatesLeftToCheck = adjacentGridCoordinates.size();
    for (const auto& adjacentGridCoordinate : adjacentGridCoordinates)
    {
        if (gameOfLifeSimulationNode.getLivingCellAtGridCoordinate(adjacentGridCoordinate))
        {
            ++numAdjacentLivingCells;
        }
        // Return early if there are already too many adjacent living cells
        if (numAdjacentLivingCells > 3)
        {
            return false;
        }
        --numCoordinatesLeftToCheck;
        // Return early if there are not enough adjacent cells left to check to get the count up to 3
        if (numAdjacentLivingCells + numCoordinatesLeftToCheck < 3)
        {
            return false;
        }
    }
    return true;
}

vector<GridUtilities::GridCoordinate> GameOfLifeLivingCell::attemptLivingCellCreationInAdjacentCells() const
{
    vector<GridUtilities::GridCoordinate> gridCoordinatesToCreateCellsAt;
    for (const auto& adjacentGridCoordinate : GridUtilities::getAdjacentGridCoordinates(mGridCoordinate))
    {
        if (!mGameOfLifeSimulationNode.hasBeenCheckedForCellCreation(adjacentGridCoordinate))
        {
            mGameOfLifeSimulationNode.markHasBeenCheckedForCellCreation(adjacentGridCoordinate);
            if (shouldComeToLife(mGameOfLifeSimulationNode, adjacentGridCoordinate))
            {
                gridCoordinatesToCreateCellsAt.push_back(adjacentGridCoordinate);
            }
        }
    }
    return gridCoordinatesToCreateCellsAt;
}

void GameOfLifeLivingCell::onSimulationTickBegin()
{
    if (isInPool())
    {
        return;
    }
    mWillDie = shouldDie(mGameOfLifeSimulationNode, mGridCoordinate);
    mGridCoordinatesToCreateCellsAt = attemptLivingCellCreationInAdjacentCells();
}

void GameOfLifeLivingCell::onSimulationTickEnd()
{
    if (isInPool())
    {
        return;
    }
    for (const auto& gridCoordinate : mGridCoordinatesToCreateCellsAt)
    {
        mGameOfLifeSimulationNode.createCell(gridCoordinate);
    }
    mGridCoordinatesToCreateCellsAt.clear();
    if (mWillDie)
    {
        mGameOfLifeSimulationNode.killCell(mGridCoordinate);
    }
}
