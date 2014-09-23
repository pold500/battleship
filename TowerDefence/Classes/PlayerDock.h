//
//  PlayerDock.h
//  TowerDefence
//
//  Created by Pavel on 9/19/14.
//
//

#ifndef __TowerDefence__PlayerDock__
#define __TowerDefence__PlayerDock__

#include <vector>
#include <cocos2d.h>
#include <algorithm>

#include "Ship.h"

USING_NS_CC;

//Deckboard model

static const int SHIP_BLOCK = 1;
static const int NO_BLOCK   = 0;

class PlayerDock {
    std::vector<Ship> shipsVector;
    int matrix[10][10];
    PlayerDock()
    {
        memset(&matrix[0][0], NO_BLOCK, 10*10*sizeof(int));
    }
public:
    void initDockWithVector  (const std::vector<Ship>& ships)
    {

        for(auto& ship: shipsVector)
        {
            //init matrix
            //get ship coordinate as point2d
            if(ship.getOrientation()==ShipOrientation::horizontal)
            {
                const Point2D<int>& shipPosition  = ship.getShipPositionInDeck();
                std::size_t numberOfDecks = ship.getNumberOfDecks();
                for(int i = shipPosition.x; i<shipPosition.x + numberOfDecks; i++)
                {
                    matrix[i][shipPosition.y] = SHIP_BLOCK;
                }

            } else { //orienation is vertical
                const Point2D<int>& shipPosition  = ship.getShipPositionInDeck();
                std::size_t numberOfDecks = ship.getNumberOfDecks();
                for(int i = shipPosition.y; i<shipPosition.y + numberOfDecks; i++)
                {
                    matrix[shipPosition.x][shipPosition.y+i] = SHIP_BLOCK;
                }
            }
        }
    }
    bool checkHitOnCoordinate(const Point2D<int>& coord);
    bool canPlaceShipByCoordinate(const CCPoint& coord);
};



#endif /* defined(__TowerDefence__PlayerDock__) */
