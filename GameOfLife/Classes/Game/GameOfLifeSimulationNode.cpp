#include "GameOfLifeSimulationNode.h"
#include "GameOfLifeLivingCell.h"
#include "NotificationCenter.h"
#include "GameOfLifeScene.h"
#include <chrono>
#include <fstream>

using namespace game;
using namespace cocos2d;
using namespace std;

GameOfLifeSimulationNode::GameOfLifeSimulationNode()
    : mTickAction(nullptr)
{
    mPauseCallback = make_shared<function<void()>>(bind(&GameOfLifeSimulationNode::onPause, this));
    engine::NotificationCenter::getInstance().subscribe(GameOfLifeScene::PAUSE, mPauseCallback);
    mPlayCallback = make_shared<function<void()>>(bind(&GameOfLifeSimulationNode::onPlay, this));
    engine::NotificationCenter::getInstance().subscribe(GameOfLifeScene::PLAY, mPlayCallback);
}

GameOfLifeSimulationNode::~GameOfLifeSimulationNode()
{}

void GameOfLifeSimulationNode::createCellsFromFile(const string& filePath)
{
    const auto& fullFilePath = FileUtils::getInstance()->fullPathForFilename(filePath);
    fstream ifs(fullFilePath, ifstream::in);
    // File not found
    if (!ifs.is_open())
    {
        return;
    }
    string input;
    getline(ifs, input);
    while (ifs.good())
    {
        auto gridXInputStartPosition = input.find("(");
        auto gridXInputEndPosition = input.find(",");
        auto gridYInputStartPosition = input.find(" ");
        if (gridYInputStartPosition == string::npos) // Just in case there is no space after the comma
        {
            gridYInputStartPosition = gridXInputEndPosition;
        }
        auto gridYInputEndPosition = input.find(")");
        const auto& gridXString = input.substr(gridXInputStartPosition + 1, gridXInputEndPosition - gridXInputStartPosition - 1);
        const auto& gridYString = input.substr(gridYInputStartPosition + 1, gridYInputEndPosition - gridYInputStartPosition - 1);
        printf("Read in coordinate:%s,%s\n", gridXString.c_str(), gridYString.c_str());
        
        auto gridX = stoll(gridXString);
        auto gridY = stoll(gridYString);
        createCell(GridUtilities::GridCoordinate(gridX, gridY));
        
        getline(ifs, input);
    }
    ifs.close();
}

void GameOfLifeSimulationNode::createRandomCells(long long gridRange, long long numCells)
{
    long long numCellsCreated = 0;
    while (numCellsCreated < numCells)
    {
        const auto& randomGridCoordinate = GridUtilities::GridCoordinate((rand() % gridRange) - (gridRange / 2), (rand() % gridRange) - (gridRange / 2));
        if (!doesGridCoordinateContainLivingCell(randomGridCoordinate))
        {
            createCell(randomGridCoordinate);
            numCellsCreated++;
        }
    }
}

void GameOfLifeSimulationNode::runSimulation(float tickInterval)
{
    if (mTickAction)
    {
        stopAction(mTickAction);
    }
    mTickAction = RepeatForever::create(Sequence::createWithTwoActions(DelayTime::create(tickInterval), CallFunc::create(bind(&GameOfLifeSimulationNode::tickSimulation, this))));
    runAction(mTickAction);
}

bool GameOfLifeSimulationNode::doesGridCoordinateContainLivingCell(const GridUtilities::GridCoordinate& gridCoordinate)
{
    return mGridCoordinateToLivingCellMap.count(gridCoordinate) > 0;
}

void GameOfLifeSimulationNode::createCell(const GridUtilities::GridCoordinate& gridCoordinate)
{
    if (doesGridCoordinateContainLivingCell(gridCoordinate))
    {
        printf("Warning: trying to create a cell in a grid space that's already occupied\n");
        return;
    }
    auto gameOfLifeLivingCell = mLivingCellPool.retrieve();
    if (!gameOfLifeLivingCell)
    {
        gameOfLifeLivingCell = GameOfLifeLivingCell::create();
        addChild(gameOfLifeLivingCell);
    }
    gameOfLifeLivingCell->setPositionForGridCoordinate(gridCoordinate);
    mGridCoordinateToLivingCellMap[gridCoordinate] = gameOfLifeLivingCell;
}

void GameOfLifeSimulationNode::killCell(const GridUtilities::GridCoordinate& gridCoordinate)
{
    auto iter = mGridCoordinateToLivingCellMap.find(gridCoordinate);
    if (iter == mGridCoordinateToLivingCellMap.end())
    {
        printf("Warning: trying to kill a cell that has already been killed\n");
        return;
    }
    auto gameOfLifeLivingCell = iter->second;
    mLivingCellPool.put(gameOfLifeLivingCell);
    mGridCoordinateToLivingCellMap.erase(iter);
}

void GameOfLifeSimulationNode::tickSimulation()
{
    chrono::high_resolution_clock::time_point timeSimulationStart = chrono::high_resolution_clock::now();
    
    // Mark all living cells to have 0 neighbors at first. That way we won't forget to kill them if they really have no neighbors
    map<GridUtilities::GridCoordinate, int> gridCoordinateToAdjacentLivingCellCountMap;
    for (const auto& gridCoordinateToLivingCellPair : mGridCoordinateToLivingCellMap)
    {
        gridCoordinateToAdjacentLivingCellCountMap[gridCoordinateToLivingCellPair.first] = 0;
    }
    
    // Mark all grid spaces adjacent to living cells with how many living neighbors they have
    for (const auto& gridCoordinateToLivingCellPair : mGridCoordinateToLivingCellMap)
    {
        for (const auto& adjacentGridCoordinate : GridUtilities::getAdjacentGridCoordinates(gridCoordinateToLivingCellPair.first))
        {
            auto adjacentLivingCellCountIter = gridCoordinateToAdjacentLivingCellCountMap.find(adjacentGridCoordinate);
            if (adjacentLivingCellCountIter == gridCoordinateToAdjacentLivingCellCountMap.end())
            {
                gridCoordinateToAdjacentLivingCellCountMap[adjacentGridCoordinate] = 1;
            }
            else
            {
                adjacentLivingCellCountIter->second++;
            }
        }
    }
    
    // Create and kill cells based on the number of living neighbors
    for (const auto& gridCoordinateToAdjacentLivingCellCountPair : gridCoordinateToAdjacentLivingCellCountMap)
    {
        const auto& gridCoordinate = gridCoordinateToAdjacentLivingCellCountPair.first;
        auto adjacentLivingCellCount = gridCoordinateToAdjacentLivingCellCountPair.second;
        // Die if the number of adjacent living cells isn't 2 or 3
        if (doesGridCoordinateContainLivingCell(gridCoordinate))
        {
            if (adjacentLivingCellCount < 2 || adjacentLivingCellCount > 3)
            {
                killCell(gridCoordinate);
            }
        }
        // Come to life if the number of adjacent living cells is 3
        else
        {
            if (adjacentLivingCellCount == 3)
            {
                createCell(gridCoordinate);
            }
        }
    }
    
    chrono::high_resolution_clock::time_point timeSimulationEnd = chrono::high_resolution_clock::now();
    chrono::milliseconds timeTaken = chrono::duration_cast<chrono::milliseconds>(timeSimulationEnd - timeSimulationStart);
    printf("Simulation tick time taken: %lli milliseconds\n", timeTaken.count());
}

void GameOfLifeSimulationNode::onPause()
{
    pause();
}

void GameOfLifeSimulationNode::onPlay()
{
    resume();
}
