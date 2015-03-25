#include "GameOfLifeScene.h"
#include "GameOfLifeSimulationNode.h"
#include "CameraNode.h"

using namespace game;
using namespace cocos2d;
using namespace std;

void makeFloater(vector<GridUtilities::GridCoordinate>& livingCellCoordinates)
{
    auto startX = rand() % 100;
    auto startY = rand() % 100;
    livingCellCoordinates.push_back(make_pair(startX, startY));
    livingCellCoordinates.push_back(make_pair(startX + 1, startY));
    livingCellCoordinates.push_back(make_pair(startX + 2, startY));
    livingCellCoordinates.push_back(make_pair(startX + 2, startY + 1));
    livingCellCoordinates.push_back(make_pair(startX + 1, startY + 2));
}

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
    
    vector<GridUtilities::GridCoordinate> livingCellCoordinates;
    for (int i = 0; i < 100; ++i)
    {
        makeFloater(livingCellCoordinates);
    }
    for (const auto& gridCoordinate : livingCellCoordinates)
    {
        mGameOfLifeSimulationNode->createCell(gridCoordinate);
    }
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
