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
const float TICK_INTERVAL = 0.2f;
const float ZOOM_FACTOR = 0.2f;
const int RANDOM_GRID_RANGE = 600;
const int RANDOM_NUM_CELLS_TO_SPAWN = 30000;

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
    
    srand(time(nullptr));
    mIsSimulationPaused = false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int zOrder = 0;
    
    // Add the viewport
    auto viewPortNode = Node::create();
    viewPortNode->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f));
    addChild(viewPortNode, zOrder++);
    
    // Add the camera
    auto cameraNode = CameraNode::create();
    viewPortNode->addChild(cameraNode, zOrder++);
    
    // Add the game of life simulation
    mGameOfLifeSimulationNode = GameOfLifeSimulationNode::create();
    cameraNode->addChild(mGameOfLifeSimulationNode, zOrder++);
    
    // Add the create glider on tap node
    auto createGliderOnTapNode = CreateGliderOnTapNode::create();
    createGliderOnTapNode->setGameOfLifeSimulationNode(mGameOfLifeSimulationNode);
    addChild(createGliderOnTapNode, zOrder++);
    
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
        mIsSimulationPaused = false;
    });
    mPlayButton->setPosition(Vec2(closeButton->getPosition().x - 60.0f, closeButton->getPosition().y));
    
    // Add the pause button
    mPauseButton = MenuItemImage::create("button_black_pause.png", "button_black_pause.png",
    [this](Ref* sender)
    {
        engine::NotificationCenter::getInstance().notify(GameOfLifeScene::PAUSE);
        mPauseButton->setVisible(false);
        mPlayButton->setVisible(true);
        mIsSimulationPaused = true;
    });
    mPauseButton->setPosition(mPlayButton->getPosition());
    
    // Add the read from file button
    auto readFromFileButton = MenuItemImage::create("file.png", "file.png",
    [this, cameraNode, createGliderOnTapNode](Ref* sender)
    {
        resetSimulation(cameraNode, createGliderOnTapNode);
        mGameOfLifeSimulationNode->createCellsFromFile("input.txt");
    });
    readFromFileButton->setPosition(mPlayButton->getPosition().x - 60.0f, mPlayButton->getPosition().y);
    
    // Add the random cell create button
    auto randomCreateButton = MenuItemImage::create("MissingNo.png", "MissingNo.png",
    [this, cameraNode, createGliderOnTapNode](Ref* sender)
    {
        resetSimulation(cameraNode, createGliderOnTapNode);
        mGameOfLifeSimulationNode->createRandomCells(RANDOM_GRID_RANGE, RANDOM_NUM_CELLS_TO_SPAWN);
    });
    randomCreateButton->setPosition(readFromFileButton->getPosition().x - 60.0f, readFromFileButton->getPosition().y);
    
    // Add the zoom in button
    auto zoomInButton = MenuItemImage::create("Zoom-In-icon.png", "Zoom-In-icon.png",
    [viewPortNode](Ref* sender)
    {
        viewPortNode->setScale(viewPortNode->getScale() * (1.0f + ZOOM_FACTOR));
    });
    zoomInButton->setPosition(randomCreateButton->getPosition().x - 60.0f, randomCreateButton->getPosition().y);
    
    // Add the zoom out button
    auto zoomOutButton = MenuItemImage::create("Zoom-Out-icon.png", "Zoom-Out-icon.png",
    [viewPortNode](Ref* sender)
    {
        viewPortNode->setScale(viewPortNode->getScale() * (1.0f - ZOOM_FACTOR));
    });
    zoomOutButton->setPosition(zoomInButton->getPosition().x - 60.0f, zoomInButton->getPosition().y);
    
    auto menu = Menu::create(closeButton, mPlayButton, mPauseButton, readFromFileButton, randomCreateButton, zoomInButton, zoomOutButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, zOrder++);
    
    mGameOfLifeSimulationNode->createRandomCells(RANDOM_GRID_RANGE, RANDOM_NUM_CELLS_TO_SPAWN);
    mGameOfLifeSimulationNode->runSimulation(TICK_INTERVAL);
    
    return true;
}

void GameOfLifeScene::resetSimulation(CameraNode* cameraNode, CreateGliderOnTapNode* createGliderOnTapNode)
{
    cameraNode->setPosition(Vec2::ZERO);
    auto newGameOfLifeSimulationNode = GameOfLifeSimulationNode::create();
    cameraNode->addChild(newGameOfLifeSimulationNode, mGameOfLifeSimulationNode->getLocalZOrder());
    mGameOfLifeSimulationNode->removeFromParent();
    mGameOfLifeSimulationNode = newGameOfLifeSimulationNode;
    createGliderOnTapNode->setGameOfLifeSimulationNode(mGameOfLifeSimulationNode);
    mGameOfLifeSimulationNode->runSimulation(TICK_INTERVAL);
    if (mIsSimulationPaused)
    {
        engine::NotificationCenter::getInstance().notify(GameOfLifeScene::PAUSE);
    }
}
