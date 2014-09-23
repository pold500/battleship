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

bool OsamaAI::intersectsWithBoundary(Point2D<int>& osamaPoint)
{
    return pointsSet.find(osamaPoint)!=pointsSet.end()?true:false;
}

struct IntRect
{

};


void OsamaAI::insertIntoBoundary(Point2D<int> startPoint, Point2D<int> endPoint)
{
    pointsSet.insert(startPoint);
    int miX = (startPoint.x - 1 < 0)? 0 : startPoint.x - 1;
    int hiX = (  endPoint.x + 1 ) % 10;
    int miY = (startPoint.y - 1 < 0)? 0 : startPoint.y - 1;
    int hiY = (endPoint.y + 1) % 10;
    for(int i=miX; i<hiX; i++) //everything in that rectangle
        for(int j=miY; j<hiY; j++)
        {
            pointsSet.insert(Point2D<int>(i,j));
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
    return (memoryMatrix[x][y]==CellState::Hit||memoryMatrix[x][y]==CellState::Miss||
            memoryMatrix[x][y]==CellState::Destroyed);
}

void OsamaAI::performAttack(GameGridLayer* playerGrid)
{
    switch(state)
    {
        case State::searching:
        {
            Point2D<int> point { rand()%10, rand() % 10};
            //Osama do not hit a cells that are boundary to destroyed ships
            while (intersectsWithBoundary(point)) {
                point = {rand()%10, rand()%10}; //generate new coordinates
            }

            if(!playerGrid->isCellTested(point.x, point.y)) {
                CellState cellState = memoryMatrix[point.x][point.y] = playerGrid->hitCell(point.x, point.y);

                switch(cellState)
                {
                    case CellState::Hit:
                    {
                        startingPoint = point;
                        if(point.x==0)
                            usedDirections.insert(Direction::left);
                        if(point.y==0)
                            usedDirections.insert(Direction::down);
                        if(point.x==9)
                            usedDirections.insert(Direction::right);
                        if(point.y==9)
                            usedDirections.insert(Direction::up);
                        shotsVector.push_back(point);
                        pointsSet.insert(point);
                        state = State::discoveringDirection; //transitionate to a new state
                    }
                    break;

                    case CellState::Destroyed:
                        //write ship coord and boundary into a set
                        //it's and 1 deck ship otherwise we should hit from another state
                        insertIntoBoundary(point, point);
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

            unusedDirections.clear();

            std::vector<Direction> vec1 (begin(usedDirections), end(usedDirections));
            std::vector<Direction> vec2 (begin(allDirections ), end(allDirections ));

            std::sort(begin(vec1), end(vec1), [](const Direction& d1, const Direction& d2)
                      { return (static_cast<int>(d1) < static_cast<int>(d2)); });
            std::sort(begin(vec2), end(vec2), [](const Direction& d1, const Direction& d2)
                      { return (static_cast<int>(d1) < static_cast<int>(d2)); });
            std::set_symmetric_difference(begin(vec1), end(vec1), begin(vec2),
                                end  (vec2), inserter(unusedDirections,
                                                      unusedDirections.begin()));


            for(Direction direction: unusedDirections) {
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

                cellState = playerGrid->hitCell(newShot.x, newShot.y);

                if(cellState==CellState::Hit)
                {
                    //right direction was choosen. continue to destroy ship in that direction
                    state = State::destroyingShip;
                    break;
                }
                else if(cellState==CellState::Destroyed) {
                    //we destroyed a ship with that shot.
                    //store him in a matrix
                    //store it bounds
                    insertIntoBoundary(startingPoint, newShot);
                    state = State::searching;
                    break;
                }
                else if(cellState==CellState::Miss)
                {
                    //choose another direction
                    //recall the shot
                    //shotsVector.push_back(newShot);
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
            auto cellState = playerGrid->hitCell(newShot.x, newShot.y);
            switch(cellState)
            {
                case CellState::Destroyed:
                    state = State::searching; //Osama destroyed the ship
                    break;
                default: break;
            }
        }
        break;

    }



}