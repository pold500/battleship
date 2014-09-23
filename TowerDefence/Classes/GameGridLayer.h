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
#include "Ship.h"
#include <memory>
#include <vector>
#include "QuadTree.h"
USING_NS_CC;


//Players grid


enum CellState{
    ShipPresent,
    Empty,
    Hit,
    Miss,
    Destroyed
};

class GameGridLayer: public CCLayerColor
{
    void         drawCell(const CCPoint&);
    std::vector<std::shared_ptr<Ship>> m_shipsVector;
    QuadTree* quadTree;
    CellState matrix[10][10];
    CCSprite* spriteMatrix[10][10];
    void modifyMatrix(int x, int y);
    void notifyShipCollection(const Point2D<int>& point);

public:

    const int gridSize = 190;
    const int cellSize = gridSize / 10;

    bool showFleet; //turn on for player grid, turn off for an enemy grid

    bool isCellTested(int x, int y);
    CellState hitCell(int x, int y);


    GameGridLayer():CCLayerColor(), showFleet(false), quadTree(new QuadTree(AABB(Vec2f(0, 0), Vec2f(190.f, 190.f))))
    { std::memset(&matrix[0][0], Empty, 10*10); setContentSize(CCSize(gridSize, gridSize)); setAnchorPoint(ccp(0,0)); }
    virtual void draw();
    CREATE_FUNC(GameGridLayer);
    void loadShips(const std::vector<std::shared_ptr<Ship>>& shipsVector);
};

#endif /* defined(__TowerDefence__GameGridLayer__) */
