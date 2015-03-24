#include "GameOfLifeLivingCell.h"
#include "GameOfLifeSimulationNode.h"
#include "NotificationCenter.h"

using namespace game;
using namespace cocos2d;
using namespace std;

static const string SPRITE_NAME = "WhitePixel.png";

GameOfLifeLivingCell::GameOfLifeLivingCell()
{
    mSprite = Sprite::create(SPRITE_NAME);
    mSprite->setScale(100, 100);
    addChild(mSprite);
    
    mSimulationTickCallback = make_shared<std::function<void()>>(bind(&GameOfLifeLivingCell::onSimulationTick, this));
    engine::NotificationCenter::getInstance().subscribe(GameOfLifeSimulationNode::SIMULATION_TICK_NOTIFICATION, mSimulationTickCallback);
}

GameOfLifeLivingCell::~GameOfLifeLivingCell()
{}

void GameOfLifeLivingCell::onSimulationTick()
{
    printf("yay");
}
