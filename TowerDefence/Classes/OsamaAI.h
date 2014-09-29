//
//  OsamaAI.h
//  TowerDefence
//
//  Created by Pavel on 9/23/14.
//
//

#ifndef __TowerDefence__OsamaAI__
#define __TowerDefence__OsamaAI__
#include "AIInterface.h"
#include "GameGridLayer.h"
#include "CellState.h"
#include <set>
#include <vector>
#include <list>



class OsamaAI: public AIInterface
{
    class AutomataState {

    } innerState;

    enum class State
    {
        searching, //in this state AI randomly searches for a points of interest
        discoveringDirection,
        destroyingShip
    };
    size_t m_shipSize;
    uint32_t shotsCount;
    State state;

    bool doesIntersectsWithBoundary(Point2D<int>& osamaPoint);
    void insertPointAndFillAdjacentPoints(const Point2D<int>& startPoint, size_t shipSize,
                                          ShipOrientation orientation);


    Point2D<int> startingPoint;

    bool isCellTested(int x, int y);

    std::set<Point2D<int>>    pointsSet;
    std::vector<Point2D<int>> shotsVector;
    enum class Direction { up, down, left, right };

    Direction choosenDirection;

    //struct Directions { bool up, down, left, right; };
  
    std::set<Direction> usedDirections;
    //Directions checkedDirections;

    bool m_isOsamaChoosenWrongDirection; //this flag is set to true when osama fails to destroy a ship in choosen direction
    Point2D<int> m_originalShot;

    CellState memoryMatrix[10][10];

public:
    OsamaAI():state(State::searching), memoryMatrix(), shotsCount(0),
    m_isOsamaChoosenWrongDirection(false), m_originalShot(0,0)
    {
        ::memset(&memoryMatrix, (int)CellState::Empty, 10*10);
    }
    void performAttack(GameGridLayer* playerGrid);
};

#endif /* defined(__TowerDefence__OsamaAI__) */
