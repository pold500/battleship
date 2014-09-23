//
//  ShipDock.h
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//

#ifndef __TowerDefence__ShipDock__
#define __TowerDefence__ShipDock__
#include <array>
#include "Ship.h"

//Singletone
class ShipDock
{
    static ShipDock* instance;
    size_t shipsWith1Docks;
    size_t shipsWith2Docks;
    size_t shipsWith3Docks;
    size_t shipsWith4Docks;
    std::array<int, 4> shipsCounters;
    ShipDock();
public:
    static ShipDock* getInstance();
    std::size_t getFreeShipsCount(ShipType);
    Ship* getShipFromDock  (ShipType);
    void decreaseCounter(ShipType type);

};


#endif /* defined(__TowerDefence__ShipDock__) */
