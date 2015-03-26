#include "GameOfLifeLivingCell.h"
#include "GameOfLifeSimulationNode.h"

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
}

GameOfLifeLivingCell::GameOfLifeLivingCell()
{
    mSprite = Sprite::create(SPRITE_NAME);
    mSprite->setScale(GridUtilities::GRID_SPACE_SIZE);
    addChild(mSprite);
}

GameOfLifeLivingCell::~GameOfLifeLivingCell()
{}

void GameOfLifeLivingCell::setPositionForGridCoordinate(const GridUtilities::GridCoordinate& gridCoordinate)
{
    setPosition(gridCoordinate.first * GridUtilities::GRID_SPACE_SIZE, gridCoordinate.second * GridUtilities::GRID_SPACE_SIZE);
}
