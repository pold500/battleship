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
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++ )
        {
            //Draw depends on the state of the matrix
            //matrix[i][j];
            if(matrix[i][j]==Hit)
            {
                //Overlay a Burst.png srpite

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
    return (matrix[x][y]==Hit||matrix[x][y]==Miss)?true:false;
}

CellState GameGridLayer::hitCell(int x, int y)
{
    modifyMatrix(x, y);
    return matrix[x][y];
}

void GameGridLayer::notifyShipCollection(const Point2D<int>& point)
{
    std::vector<QuadTreeOccupant *> queryResult;
    quadTree->Query(AABB({point.x,point.y}, {cellSize, cellSize}), queryResult);
    if(!queryResult.empty())
    {
        Ship* ship = static_cast<Ship*>(queryResult.back());
        ship->setDamage();
        switch(ship->getShipState())
        {
            case ShipState::Damaged:
                matrix[point.x][point.y] = CellState::Hit;
                break;
            case ShipState::Destroyed:
                matrix[point.x][point.y] = CellState::Destroyed;
        }
    }
}

void GameGridLayer::modifyMatrix(int x, int y)
{
    if(matrix[x][y]==ShipPresent)
    {
        matrix[x][y] = Hit; //if we have a 1deck ship? destroyed?
        notifyShipCollection({x, y});
        spriteMatrix[x][y] = CCSprite::create("Burst.png");
        spriteMatrix[x][y]->setPosition(ccp(x*cellSize, y*cellSize));
        spriteMatrix[x][y]->setScale(2.0f);
        spriteMatrix[x][y]->setAnchorPoint(ccp(0.f,0.f));
        this->addChild(spriteMatrix[x][y]);
    } else if(matrix[x][y]== Miss){
        matrix[x][y] = Miss;
        spriteMatrix[x][y] = CCSprite::create("Miss.png");
        spriteMatrix[x][y]->setPosition(ccp(x*cellSize, y*cellSize));
        spriteMatrix[x][y]->setScale(2.0f);
        spriteMatrix[x][y]->setAnchorPoint(ccp(0.f,0.f));
        this->addChild(spriteMatrix[x][y]);
    } else if (matrix[x][y]== Destroyed)
    {
        matrix[x][y] = Destroyed;
        CCSprite* sprite = CCSprite::create("Destroyed.png");
        sprite->setPosition(ccp(x*cellSize, y*cellSize));
        sprite->setScale(2.0f);
        sprite->setAnchorPoint(ccp(0.f,0.f));
        this->addChild(sprite);
    }
}

void GameGridLayer::loadShips(const std::vector<std::shared_ptr<Ship>>& shipsVector)
{
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
        {
            matrix[i][j] = Empty;
        }
    //TODO: Replace by std::move here
    m_shipsVector.assign(std::begin(shipsVector), std::end(shipsVector));



    for(auto& ship: m_shipsVector)
    {
        Point2D<int> pp = ship->getPositionInDeck();
        CCPoint globalShipCoord(pp.x * cellLength , pp.y * cellLength );

        quadTree->AddOccupant(ship.get());

        ship->setVisible(showFleet);
        this->addChild(ship.get()); //adds ships visually
        ship->setPosition(globalShipCoord);
    }

}


