#include "GameOfLifeScene.h"
#include "GameOfLifeSimulationNode.h"
#include "CameraNode.h"
#include "CreateGliderOnTapNode.h"
#include "NotificationCenter.h"

using namespace game;
using namespace cocos2d;
using namespace std;

const string GameOfLifeScene::PAUSE = "Pause";
const string GameOfLifeScene::PLAY = "Play";

Scene* GameOfLifeScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameOfLifeScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameOfLifeScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    srand(time(NULL));
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int zOrder = 0;
    
    // Add the camera
    auto cameraNode = CameraNode::create();
    addChild(cameraNode, zOrder++);

    // Add the close button
    auto closeButton = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
    [](Ref* sender)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
        MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
        return;
#endif
        
        Director::getInstance()->end();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    });
	closeButton->setPosition(Vec2(origin.x + visibleSize.width - closeButton->getContentSize().width / 2, origin.y + closeButton->getContentSize().height / 2));
    
    // Add the play button
    mPlayButton = MenuItemImage::create("play.png", "play.png",
    [this](Ref* sender)
    {
        engine::NotificationCenter::getInstance().notify(GameOfLifeScene::PLAY);
        mPlayButton->setVisible(false);
        mPauseButton->setVisible(true);
    });
    mPlayButton->setPosition(Vec2(closeButton->getPosition().x - 60.0f, closeButton->getPosition().y));
    
    // Add the pause button
    mPauseButton = MenuItemImage::create("button_black_pause.png", "button_black_pause.png",
    [this](Ref* sender)
    {
        engine::NotificationCenter::getInstance().notify(GameOfLifeScene::PAUSE);
        mPauseButton->setVisible(false);
        mPlayButton->setVisible(true);
    });
    mPauseButton->setPosition(mPlayButton->getPosition());
    
    auto menu = Menu::create(closeButton, mPlayButton, mPauseButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, zOrder++);
    
    // Add the game of life simulation
    mGameOfLifeSimulationNode = GameOfLifeSimulationNode::create();
    cameraNode->addChild(mGameOfLifeSimulationNode, zOrder++);
    
    // Add the create glider on tap node
    auto CreateGliderOnTapNode = CreateGliderOnTapNode::create();
    CreateGliderOnTapNode->setGameOfLifeSimulationNode(mGameOfLifeSimulationNode);
    addChild(CreateGliderOnTapNode, zOrder++);
    
    spawnRandomCells(mGameOfLifeSimulationNode, 600, 40000);
    mGameOfLifeSimulationNode->runSimulation(0.2f);
    
    return true;
}

void GameOfLifeScene::spawnRandomCells(GameOfLifeSimulationNode* gameOfLifeSimulationNode, long long gridRange, long long numCells)
{
    long long numCellsCreated = 0;
    while (numCellsCreated < numCells)
    {
        const auto& randomGridCoordinate = GridUtilities::GridCoordinate((rand() % gridRange) - (gridRange / 2), (rand() % gridRange) - (gridRange / 2));
        if (!gameOfLifeSimulationNode->doesGridCoordinateContainLivingCell(randomGridCoordinate))
        {
            gameOfLifeSimulationNode->createCell(randomGridCoordinate);
            numCellsCreated++;
        }
    }
}
