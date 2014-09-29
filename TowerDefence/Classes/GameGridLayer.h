//
//  GameGridLayer.h
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//

#ifndef __TowerDefence__GameGridLayer__
#define __TowerDefence__GameGridLayer__
#include <cocos2d.h>

#include <memory>
#include <vector>

#include "Ship.h"
#include "QuadTree.h"
#include "MatrixAdapter.h"

USING_NS_CC;


//Players grid




class GameGridLayer: public CCLayerColor
{
    void         drawCell(const CCPoint&);
    std::vector<Ship*> m_shipsVector;
    QuadTree* quadTree;
    int shipMatrix [10][10]; //returns number of a ship
    CellState matrix[10][10];
    CCSprite* spriteMatrix[10][10];
    void modifyMatrix(int x, int y);
    void updateMatrix();
    void notifyShipCollection(const Point2D<int>& point);
    std::set<Point2D<int>> pointsSet;
    void drawHighlightedCell(const CCPoint& point, const bool canPlace);
    Ship* mShip;
public:
    std::set<Point2D<int>>& getPointsSet() {  return pointsSet; }
    void setShip(Ship* ship){ mShip = ship; }

    void removePointsFromSet(const Point2D<int>& shipPosition, const ShipType shipType,
                             const ShipOrientation shipOrientation);

    void insertShipAndFillAdjacentPoints(const Point2D<int>& startPoint, const ShipType shipType,
                                         const ShipOrientation orientation,
                                         std::set<Point2D<int>>& pointsSet);

    void highlightAdjacentPoints(const Point2D<int>& startPoint);


    const float gridSize = 190;
    const float cellSize = gridSize / 10;

    bool mShowFleet; //turn on for player grid, turn off for an enemy grid
    bool isCellTested(int x, int y);

    CellState hitCell(int x, int y);
    CellState hitCell(const CCPoint& gridCoords);

    bool isCanPlace(const CCPoint& location, ShipType type, ShipOrientation orientation) const
    {
        Point2D<int> point { static_cast<int>(location.x / cellSize),
                             static_cast<int>(location.y / cellSize) };
        return pointsSet.find(point) == pointsSet.end();
    }

    bool displayMarkline;

    GameGridLayer():
        CCLayerColor(),
        mShowFleet(false),
        quadTree(new QuadTree(AABB(Vec2f(0, 0), Vec2f(190.f, 190.f)))),
        displayMarkline(false)
    {
        setContentSize(CCSize(gridSize, gridSize)); setAnchorPoint(ccp(0,0));
    }



    virtual void draw();
    CREATE_FUNC(GameGridLayer);
    //void loadShips(const std::vector<std::shared_ptr<Ship>>& shipsVector);
    void loadShips(std::vector<Ship*>&  shipsVector);
};

#endif /* defined(__TowerDefence__GameGridLayer__) */
