//
//  OsamaAI.cpp
//  TowerDefence
//
//  Created by Pavel on 9/23/14.
//
//

#include "OsamaAI.h"
#include "GameGridLayer.h"
#include <algorithm>
#include <fstream>
bool OsamaAI::doesIntersectsWithBoundary(Point2D<int>& osamaPoint)
{
    return pointsSet.find(osamaPoint)!=pointsSet.end()?true:false;
}

struct IntRect
{

};

bool isShotExceedsBoundary(const Point2D<int>& shot)
{
    return (shot.x<0||shot.x>9||shot.y<0||shot.y>9);
}

void OsamaAI::insertPointAndFillAdjacentPoints(const Point2D<int>& startPoint, size_t shipSize, ShipOrientation orientation)
{

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
        for(int i=0; i<=vectors.size(); i++)
        {
            Point2D<int> fillPoint = pos + vectors[i];
            pointsSet.insert(fillPoint);
        }
    }

}

#define gridSizeInCells 10

bool checkIfHitCornered(Point2D<int>& point)
{
    return(point.x == 0 || point.y == 0 || point.x == gridSizeInCells - 1 ||
                                           point.y == gridSizeInCells - 1);
}

bool OsamaAI::isCellTested(int x, int y)
{
    return pointsSet.find({x,y})!=pointsSet.end();
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::vector<Point2D<int>> hitPoints;
std::vector<Point2D<int>>::const_iterator reverseIt;
static int i = 0;

static void readTestValues()
{

    if(i==0) {
    std::string line;
    std::ifstream file("/Users/pavel/Desktop/TowerDefence-Cocos2d-x/TowerDefence/OsamaCoords.txt");
    if (file.is_open())
    {
        while ( std::getline(file,line) )
        {
            std::vector<std::string> x = split(line, ' ');
            if(line[0]=='#'||line.length()==0)
                continue;
            hitPoints.push_back(Point2D<int>(std::stoi(x[0]),std::stoi(x[1])));
        }
        file.close();
    }
        reverseIt = hitPoints.begin();
        i++;
    }
}

void fillTheSetAndTest(std::set<Point2D<int>>& set)
{
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
        {
            set.insert({i,j});
        }
    size_t setSize = set.size();
    setSize++;
}

void OsamaAI::performAttack(GameGridLayer* playerGrid)
{
    //For test purposes
    readTestValues();
    //fillTheSetAndTest(this->pointsSet);
    //this->pointsSet.clear();

    //insertPointAndFillAdjacentPoints({5,5}, 4, ShipOrientation::vertical);

    switch(state)
    {
        case State::searching:
        {
            Point2D<int> point; //{ rand()%10, rand() % 10};
            if(reverseIt!=hitPoints.end()) {
                point = {reverseIt->x, reverseIt->y };
                reverseIt++;
            } else
            {
                point = { rand()%10, rand()%10 };
            }

            //Osama do not hit a cells that are boundary to destroyed ships
            while (doesIntersectsWithBoundary(point)) {
                point = {rand()%10, rand()%10}; //generate new coordinates
            }

            if(!playerGrid->isCellTested(point.x, point.y)) {
                CellState cellState = memoryMatrix[point.x][point.y] = playerGrid->hitCell(point.x, point.y);
                pointsSet.insert(point);

                switch(cellState)
                {
                    case CellState::Hit:
                    {
                        if(point.x==0)
                            usedDirections.insert(Direction::left);
                        if(point.y==0)
                            usedDirections.insert(Direction::down);
                        if(point.x==9)
                            usedDirections.insert(Direction::right);
                        if(point.y==9)
                            usedDirections.insert(Direction::up);
                        m_originalShot = point;
                        shotsCount = 1;
                        shotsVector.push_back(point);
                        state = State::discoveringDirection; //transitionate to a new state
                    }
                    break;

                    case CellState::Destroyed:
                        //write ship coord and boundary into a set
                        //it's and 1 deck ship otherwise we should hit from another state
                        usedDirections.clear();
                        insertPointAndFillAdjacentPoints(point, 1, ShipOrientation::horizontal);
                        break;
                    default: pointsSet.insert(point);
                }
            }
        }
        break;

        //We're moving to that state if we have discovered a ship
        case State::discoveringDirection:
        {
            const Point2D<int> lastShot = shotsVector.back();
            std::set<Direction> allDirections {Direction::up, Direction::down, Direction::left, Direction::right};

            std::set<Direction> unusedDirections;

            std::vector<Direction> vec1 (begin(usedDirections), end(usedDirections));
            std::vector<Direction> vec2 (begin(allDirections ), end(allDirections ));

            std::sort(begin(vec1), end(vec1), [](const Direction& d1, const Direction& d2)
                      { return (static_cast<int>(d1) < static_cast<int>(d2)); });
            std::sort(begin(vec2), end(vec2), [](const Direction& d1, const Direction& d2)
                      { return (static_cast<int>(d1) < static_cast<int>(d2)); });
            std::set_symmetric_difference(begin(vec1), end(vec1), begin(vec2),
                                end  (vec2), inserter(unusedDirections,
                                                      unusedDirections.begin()));

            std::vector<Direction> unusedDirectionsShuffled(begin(unusedDirections), end(unusedDirections));
            std::srand(unsigned(std::time(0)));
            std::random_shuffle(begin(unusedDirectionsShuffled), end(unusedDirectionsShuffled));

            for(Direction direction: unusedDirectionsShuffled) {
                Point2D<int> vector{0,0};
                this->choosenDirection = direction;
                switch(direction)
                {
                    case Direction::up:
                        vector = {0,1};
                        break;
                    case Direction::down:
                        vector = {0,-1};
                        break;
                    case Direction::left:
                        vector = {-1,0};
                        break;
                    case Direction::right:
                        vector = {1,0};
                        break;
                }
                Point2D<int> newShot = lastShot + vector;
                CellState cellState = CellState::Empty;

                if(!doesIntersectsWithBoundary(newShot)) {
                    cellState = playerGrid->hitCell(newShot.x, newShot.y);
                    pointsSet.insert(newShot);
                }
                else
                {
                    //change the direction and hit
                    continue;
                }
                if(cellState==CellState::Hit)
                {
                    //right direction was choosen. continue to destroy ship in that direction
                    shotsCount++;
                    shotsVector.push_back(newShot);
                    state = State::destroyingShip;
                    break;
                }
                else if(cellState==CellState::Destroyed) {
                    //we destroyed a ship with that shot.
                    //store him in a matrix
                    //store it bounds

                    shotsCount++;

                    ShipOrientation shipOrientation;

                    if(direction==Direction::up||direction==Direction::down)
                        shipOrientation = ShipOrientation::vertical;
                    else
                        shipOrientation = ShipOrientation::horizontal;

                    insertPointAndFillAdjacentPoints(m_originalShot, shotsCount, shipOrientation);
                    state = State::searching;
                    shotsCount = 0;
                    usedDirections.clear();
                    break;
                }
                else if(cellState==CellState::Miss)
                {
                    //store used direction so we won't hit there again
                    usedDirections.insert(direction);
                    break;
                }
            }
            
        }
        break;

        case State::destroyingShip:
        {
            Point2D<int> vector{0,0};
            const Point2D<int> lastShot = shotsVector.back();
            switch(choosenDirection)
            {
                case Direction::up:
                    vector = {0,1};
                    break;
                case Direction::down:
                    vector = {0,-1};
                    break;
                case Direction::left:
                    vector = {-1,0};
                    break;
                case Direction::right:
                    vector = {1,0};
                    break;
            }
            Point2D<int> newShot = lastShot + vector;
            shotsVector.push_back(newShot);
            if(isShotExceedsBoundary(newShot + vector))
            {
                state = State::discoveringDirection;
                usedDirections.insert(choosenDirection);
                shotsVector.push_back(m_originalShot);
            }
            auto cellState = playerGrid->hitCell(newShot.x, newShot.y);
            switch(cellState)
            {
                case CellState::Miss:
                    usedDirections.insert(choosenDirection);
                    if(choosenDirection==Direction::up||choosenDirection==Direction::down)
                    {
                        usedDirections.insert(Direction::left);
                        usedDirections.insert(Direction::right);
                    } else {
                        usedDirections.insert(Direction::up);
                        usedDirections.insert(Direction::down);
                    }

                    shotsVector.push_back(m_originalShot);
                    pointsSet.insert(newShot);
                    state = State::discoveringDirection;
                    break;
                case CellState::Hit:
                    shotsCount++;
                    pointsSet.insert(newShot);
                    break;
                case CellState::Destroyed:
                    shotsCount++;
                    ShipOrientation shipOrientation;

                    if(choosenDirection==Direction::up||choosenDirection==Direction::down)
                        shipOrientation = ShipOrientation::vertical;
                    else
                        shipOrientation = ShipOrientation::horizontal;

                    insertPointAndFillAdjacentPoints(m_originalShot, shotsCount, shipOrientation);
                    shotsCount = 0;
                    usedDirections.clear();

                    state = State::searching; //Osama destroyed the ship
                    break;
                default: break;
            }
        }
        break;

    }



}