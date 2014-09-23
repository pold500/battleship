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
#include <set>
#include <vector>
#include <list>

class OsamaAI: public AIInterface
{
    enum class State
    {
        searching, //in this state AI randomly searches for a points of interest
        discoveringDirection,
        destroyingShip
    };
    State state;

    bool intersectsWithBoundary(Point2D<int>& osamaPoint);
    void insertIntoBoundary(Point2D<int> startPoint, Point2D<int> endPoint);



    Point2D<int> startingPoint;

    bool isCellTested(int x, int y);

    std::set<Point2D<int>>    pointsSet;
    std::vector<Point2D<int>> shotsVector;
    enum class Direction { up, down, left, right };

    Direction choosenDirection;

    struct Directions { bool up, down, left, right; };
    std::set<Direction> unusedDirections;
    std::set<Direction> usedDirections;
    Directions checkedDirections;

    CellState memoryMatrix[10][10];

public:
    void performAttack(GameGridLayer* playerGrid);
};

#endif /* defined(__TowerDefence__OsamaAI__) */
