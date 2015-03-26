#include "GameOfLifeScene.h"
#include "GameOfLifeSimulationNode.h"
#include "CameraNode.h"
#include "CreateFloaterOnTapNode.h"

using namespace game;
using namespace cocos2d;
using namespace std;

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
    auto closeButton = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", bind(&GameOfLifeScene::menuCloseCallback, this, placeholders::_1));
	closeButton->setPosition(Vec2(origin.x + visibleSize.width - closeButton->getContentSize().width / 2, origin.y + closeButton->getContentSize().height / 2));
    auto menu = Menu::create(closeButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, zOrder++);
    
    // Add the game of life simulation
    mGameOfLifeSimulationNode = GameOfLifeSimulationNode::create();
    cameraNode->addChild(mGameOfLifeSimulationNode, zOrder++);
    
    // Add the create floater on tap node
    auto createFloaterOnTapNode = CreateFloaterOnTapNode::create();
    createFloaterOnTapNode->setGameOfLifeSimulationNode(mGameOfLifeSimulationNode);
    addChild(createFloaterOnTapNode, zOrder++);
    
    spawnRandomCells(mGameOfLifeSimulationNode, 600, 40000);
    mGameOfLifeSimulationNode->runSimulation(0.2f);
    
    return true;
}

void GameOfLifeScene::menuCloseCallback(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
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
