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
#define MATRIX_SIZE 10
#define NO_SHIP     255


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

    if(displayMarkline)
    {
        std::set<Point2D<int>> adjacentPoints;
        CCPoint gridCoords = this->convertToNodeSpace(mShip->getPosition());
        Point2D<int> shipInsertionPoint { (int)(gridCoords.x / cellLength), (int)(gridCoords.y / cellLength) };
        insertShipAndFillAdjacentPoints(shipInsertionPoint, mShip->getShipType(), mShip->getOrientation(), adjacentPoints);
        for(auto& point: adjacentPoints)
        {
            if(isCanPlace(gridCoords, mShip->getShipType(), mShip->getOrientation()))
                drawHighlightedCell(ccp((float)point.x * cellLength, (float)point.y * cellLength), true);
            else
                drawHighlightedCell(ccp((float)point.x * cellLength, (float)point.y * cellLength), false);
        }
    }
    
}

void GameGridLayer::drawHighlightedCell(const CCPoint& point, const bool canPlace)
{
    glLineWidth(4.f); //set smaller width for a line
    if(canPlace)
        ccDrawSolidRect(ccp(point.x, point.y), ccp(point.x + cellLength, point.y + cellLength),
                    ccc4f((1.f/256) * 69, (1.f/256)*206, (1.f/256)*237, 0.8f));
    else
        ccDrawSolidRect(ccp(point.x, point.y), ccp(point.x + cellLength, point.y + cellLength),
                        ccc4f((1.f/256) * 250, (1.f/256)*10, (1.f/256)*0, 0.8f));

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

    return (matrix[x][y]==CellState::Miss||matrix[x][y]==CellState::Hit);
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
    for(int i=0; i<10; i++) //y
    {
        for(int j=0; j<10; j++) //x
        {
            tree->Query(AABB({j*cellLength, i*cellLength},{j*cellLength , i*cellLength }),queryResult);
            matrix[i][j] = queryResult.size(); //number of ships in each cell
            total += queryResult.size();
            queryResult.clear();
        }
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


CellState GameGridLayer::hitCell(int i, int j)
{
    //ship manages it's state, so when the damage reaches the maximum point, we would know.
    //Convert coord's back.
    if(!isCellTested(i,j))
    {
        if(matrix[i][j]== CellState::Empty)
        {
            matrix[i][j] = CellState::Miss;
            CCSprite* missSprite = CCSprite::create("Miss.png");
            missSprite->setPosition(ccp(i*cellSize, j*cellSize));
            missSprite->setAnchorPoint(ccp(0.f, 0.f));
            missSprite->setScale(2.f);
            this->addChild(missSprite);
            return (matrix[i][j]);
        }
        if(matrix[i][j] == CellState::ShipPresent)
        {
            matrix[i][j] = CellState::Hit;
            notifyShipCollection({i,j}); //so we can update ship state
            updateMatrix(); //destroyed ships update the state of the cells in the matrix
            return matrix[i][j];
        }

    }

    return CellState::Miss;
}

void GameGridLayer::notifyShipCollection(const Point2D<int>& point)
{
    if(shipMatrix[point.x][point.y]!=NO_SHIP)
    {
        int shipNumber = shipMatrix[point.x][point.y];
        Ship* ship = m_shipsVector[shipNumber];
        CCPoint clickPosition = {point.x * (float)cellSize, point.y * (float)cellSize };
        ship->setCellDamage(clickPosition.x, clickPosition.y);
    }
}

void GameGridLayer::updateMatrix()
{
    for(const auto& ship: m_shipsVector)
    {
        if(ship->getShipState()==ShipState::Destroyed)
        {
            //recolor cells that this ship takes
            auto pos = ship->getPositionInDeck();
            auto orient = ship->getOrientation();
            if(orient==ShipOrientation::horizontal)
            {
                for(int i=pos.x; i<pos.x+ship->getNumberOfDecks(); i++)
                {
                    matrix[i][pos.y] = CellState::Destroyed;
                }
            } else {
                for(int i=pos.y; i<pos.y+ship->getNumberOfDecks(); i++)
                {
                    matrix[pos.x][i] = CellState::Destroyed;
                }
            }
        }
    }
}

void GameGridLayer::insertShipAndFillAdjacentPoints(const Point2D<int>& startPoint,
                                                    const ShipType shipType,
                                                    const ShipOrientation orientation,
                                                    std::set<Point2D<int>>& pointsSet)
{

    const size_t shipSize = static_cast<size_t>(shipType);
    pointsSet.insert(startPoint);
    //foreach cell of the ship
    std::vector<Point2D<int>> vectors;
    for(int i=-1; i<=1; i++)
        for(int j=-1; j<=1; j++)
        {
            vectors.push_back(Point2D<int>(i, j));
        }


    Point2D<int> step = ( orientation == ShipOrientation::horizontal )?
    Point2D<int>{1,0}:Point2D<int>{0,1};
    Point2D<int> pos;
    for(int i=0; i<shipSize; i++)
    {
        pos = startPoint + step * i;
        for(int i=0; i<=8; i++)
        {
            Point2D<int> fillPoint = pos + vectors[i];
            pointsSet.insert(fillPoint);
        }
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
    m_shipsVector.assign(begin(shipsVector), end(shipsVector));

    for(int i=0; i<MATRIX_SIZE; i++)
        for(int j=0; j<MATRIX_SIZE; j++)
    {
        shipMatrix[i][j] = NO_SHIP;
    }

    for(int i=0; i<MATRIX_SIZE; i++)
    for(int j=0; j<MATRIX_SIZE; j++)
    {
        matrix[i][j] = CellState::Empty;
    }

    for(const auto& ship: shipsVector)
    {
        Point2D<int> coord = ship->getPositionInDeck();
        if(ship->getOrientation()==ShipOrientation::horizontal)
        {
            for(int i=coord.x; i<coord.x+ship->getNumberOfDecks(); i++)
            {
                matrix[i][coord.y] = CellState::ShipPresent;
            }
        } else {
            for(int j=coord.y; j<coord.y+ship->getNumberOfDecks(); j++)
            {
                matrix[coord.x][j] = CellState::ShipPresent;
            }
        }
    }

    for(int shipNumber = 0; shipNumber < m_shipsVector.size(); shipNumber++)
    {
        Ship *ship = m_shipsVector[shipNumber];
        Point2D<int> coord = ship->getPositionInDeck();
        if(ship->getOrientation()==ShipOrientation::horizontal)
        {
            for(int i=coord.x; i<coord.x+ship->getNumberOfDecks(); i++)
            {
                shipMatrix[i][coord.y] = shipNumber;
            }
        } else {
            for(int j=coord.y; j<coord.y+ship->getNumberOfDecks(); j++)
            {
                shipMatrix[coord.x][j] = shipNumber;
            }
        }

    }

}




