//
//  GameGridLayer.cpp
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//

#include "GameGridLayer.h"
const std::size_t gridSize   = 10;
const std::size_t gridLength = 190; //set an even number, ending on '0'
const std::size_t cellLength = gridLength/gridSize;

void GameGridLayer::draw()
{
    this->setColor(ccColor3B{255, 50, 50});

    cocos2d::ccDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    glLineWidth(5.f);

    cocos2d::ccDrawRect(ccp(0, 0), ccp(0 + gridLength, 0 + gridLength));

    //Draw cells in a grid. 10x10
    for(size_t i=0; i<10; i++)
    {
        for(size_t j=0; j<10; j++)
        {
            drawCell(CCPoint(0 + i*cellLength, 0 + j*cellLength));
        }
    }

}

void GameGridLayer::drawCell(const CCPoint& point)
{
    glLineWidth(4.f); //set smaller width for a line
    ccDrawRect(ccp(point.x, point.y), ccp(point.x + cellLength, point.y + cellLength));
}

bool GameGridLayer::isCellTested(int x, int y)
{
    assert(x<10);
    assert(y<10);
    Point2D<int> gridCoords { x*cellSize, y*cellSize };
    CCPoint point(x, y);
    std::vector<QuadTreeOccupant *> queryResult;
    Vec2f startPoint = {point.x, point.y};
    Vec2f endPoint   = {cellSize, cellSize};
    endPoint+=startPoint;
    quadTree->Query(AABB(startPoint, endPoint), queryResult);
    if(!queryResult.empty()) //if there's a ship in that rect
    {
        Ship* ship = static_cast<Ship*>(queryResult.back());
        return (ship->getCellDamage(gridCoords.x, gridCoords.y)==CellState::Hit||
                ship->getCellDamage(gridCoords.x, gridCoords.y)==CellState::Miss||
                ship->getCellDamage(gridCoords.x, gridCoords.y)==CellState::Destroyed);
    }
    return false;
}
/* Clients determine inner integer coordinate by themsleves */


void unitTest(QuadTree* tree)
{
    std::vector<QuadTreeOccupant *> queryResult;
    CCPoint point(0,0);
    std::vector<size_t> sizes;
    for(int i=1; i<=10; i++)
    {
        tree->Query(AABB({0,0},{i*cellLength, i*cellLength}), queryResult);
        size_t size = queryResult.size();
        sizes.push_back(size);
        queryResult.clear();
    }
    size_t size = sizes.size();

    sizes.clear();
    int matrix[10][10];
    int total = 0;
    for(int i=0; i<9; i++) //y
    {
        for(int j=0; j<10; j++) //x
        {
            tree->Query(AABB({j*cellLength, i*cellLength},{j*cellLength + cellLength, i*cellLength + cellLength}),queryResult);
            matrix[i][j] = queryResult.size(); //number of ships in each cell
            total += queryResult.size();
            //std::cout<<matrix[i][j]<<" ";
            queryResult.clear();
        }
        //std::cout<<std::endl;
    }

    for(int i = 9; i>=0; i--){
        for(int j=0; j<10; j++ )
        {
            std::cout<<matrix[i][j]<<" ";

        }
        std::cout<<std::endl;
    }
    std::cout<<"total: "<<total<<std::endl;

    size = sizes.size();

}


CellState GameGridLayer::hitCell(const CCPoint& gridCoords)
{
    //unitTest(quadTree);

    CCPoint point(gridCoords);
    std::vector<QuadTreeOccupant *> queryResult;
    Vec2f startPoint = {point.x, point.y};
    Vec2f endPoint   = {cellSize, cellSize};
    endPoint+=startPoint;
    quadTree->Query(AABB(startPoint, endPoint), queryResult);
    if(!queryResult.empty()) //if there's a ship in that rect
    {
        Ship* ship = static_cast<Ship*>(queryResult.back());
        return ship->setCellDamage(gridCoords.x, gridCoords.y);
        //return ship->getCellDamage(gridCoords.x, gridCoords.y);
    }
    return CellState::Empty;
}

CellState GameGridLayer::hitCell(int x, int y)
{
    //ship manages it's state, so when the damage reaches the maximum point, we would know.
    //Convert coord's back.
    CCPoint gridCoords { (float)x * cellSize, (float)y * cellSize };
    return hitCell(gridCoords);
}

void GameGridLayer::notifyShipCollection(const Point2D<int>& point)
{
    std::vector<QuadTreeOccupant *> queryResult;
    quadTree->Query(AABB({point.x,point.y}, {cellSize, cellSize}), queryResult);
    if(!queryResult.empty()) //if there's a ship in that rect
    {
        Ship* ship = static_cast<Ship*>(queryResult.back());
        //CCPoint localShipPoint = ship->convertToNodeSpace(ccp(point.x, point.y));
        ship->setCellDamage(point.x, point.y);
    }
}

void GameGridLayer::loadShips(std::vector<Ship*>&  shipsVector)
{
    for(auto& ship: shipsVector)
    {
        Point2D<int> pp = ship->getPositionInDeck();

        matrix[pp.x][pp.y] = CellState::Norm;
        CCPoint globalShipCoord(pp.x * cellLength , pp.y * cellLength );
        ship->setVisible(m_showFleet);
        this->addChild(ship); //adds ships visually to a node
        ship->setPosition(globalShipCoord);
        quadTree->AddOccupant(ship);
    }
}




