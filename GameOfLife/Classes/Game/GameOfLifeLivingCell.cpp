#include "GameOfLifeLivingCell.h"
#include "GameOfLifeSimulationNode.h"
#include "GridUtilities.h"
#include "NotificationCenter.h"

using namespace game;
using namespace cocos2d;
using namespace std;

static const string SPRITE_NAME = "WhitePixel.png";

GameOfLifeLivingCell* GameOfLifeLivingCell::create(GameOfLifeSimulationNode& gameOfLifeSimulationNode, const pair<int64_t, int64_t>& gridCoordinate)
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

GameOfLifeLivingCell::GameOfLifeLivingCell(GameOfLifeSimulationNode& gameOfLifeSimulationNode, const pair<int64_t, int64_t>& gridCoordinate)
    : mGameOfLifeSimulationNode(gameOfLifeSimulationNode)
    , mGridCoordinate(gridCoordinate)
    , mWillDie(false)
{
    setPosition(gridCoordinate.first * GridUtilities::GRID_SPACE_SIZE, gridCoordinate.second * GridUtilities::GRID_SPACE_SIZE);
    
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

bool GameOfLifeLivingCell::shouldDie(const GameOfLifeSimulationNode& gameOfLifeSimulationNode, const pair<int64_t, int64_t>& gridCoordinate)
{
    if (!gameOfLifeSimulationNode.getLivingCellAtGridCoordinate(gridCoordinate))
    {
        return false;
    }
    
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

bool GameOfLifeLivingCell::shouldComeToLife(const GameOfLifeSimulationNode& gameOfLifeSimulationNode, const pair<int64_t, int64_t>& gridCoordinate)
{
    if (gameOfLifeSimulationNode.getLivingCellAtGridCoordinate(gridCoordinate))
    {
        return false;
    }
    
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

vector<pair<int64_t, int64_t>> GameOfLifeLivingCell::attemptLivingCellCreationOnAdjacentCells() const
{
    vector<pair<int64_t, int64_t>> gridCoordinatesShouldCreateCellsAt;
    for (const auto& adjacentGridCoordinate : GridUtilities::getAdjacentGridCoordinates(mGridCoordinate))
    {
        if (!mGameOfLifeSimulationNode.hasBeenCheckedForCellCreation(adjacentGridCoordinate))
        {
            mGameOfLifeSimulationNode.markHasBeenCheckedForCellCreation(adjacentGridCoordinate);
            if (shouldComeToLife(mGameOfLifeSimulationNode, adjacentGridCoordinate))
            {
                gridCoordinatesShouldCreateCellsAt.push_back(adjacentGridCoordinate);
            }
        }
    }
    return gridCoordinatesShouldCreateCellsAt;
}

void GameOfLifeLivingCell::onSimulationTickBegin()
{
    mWillDie = shouldDie(mGameOfLifeSimulationNode, mGridCoordinate);
    mGridCoordinatesToCreateCellsAt = attemptLivingCellCreationOnAdjacentCells();
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
