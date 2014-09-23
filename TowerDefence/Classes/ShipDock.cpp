//
//  ShipDock.cpp
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//
// This is a data model
#include "ShipDock.h"

ShipDock* ShipDock::instance = nullptr;

ShipDock* ShipDock::getInstance()
{
    if(instance==nullptr)
    {
        instance = new ShipDock();
    }
    return instance;
}

std::size_t ShipDock::getFreeShipsCount(ShipType type)
{
    switch (type) {
        case ShipType::Deck1:
            return shipsWith1Docks;
            break;
        case ShipType::Deck2:
            return shipsWith2Docks;
            break;
        case ShipType::Deck3:
            return shipsWith3Docks;
            break;
        case ShipType::Deck4:
            return shipsWith4Docks;
        default:
            break;
    }
}

Ship* ShipDock::getShipFromDock(ShipType type)
{
    //construct a ship and decrease a count.
    //make check first
    switch(type)
    {
        case ShipType::Deck1:
        {
            if(shipsWith1Docks!=0)
            {
                Ship* ship = Ship::create(type);
                shipsWith1Docks--;
                return ship;
            }
            break;
        }
        case ShipType::Deck2:
        {
            if(shipsWith2Docks!=0)
            {
                Ship* ship = Ship::create(type);
                shipsWith2Docks--;
                return ship;
            }
        }
        case ShipType::Deck3:
        {
            if(shipsWith3Docks!=0)
            {
                Ship* ship = Ship::create(type);
                shipsWith3Docks--;
                return ship;
            }
        }
        case ShipType::Deck4:
        {
            if(shipsWith4Docks!=0)
            {
                Ship* ship = Ship::create(type);
                shipsWith4Docks--;
                return ship;
            }

        }
    }
    return nullptr;
}



void ShipDock::decreaseCounter(ShipType type)
{
    const size_t i = static_cast<size_t>(type);
    shipsCounters[i]--;
}

ShipDock::ShipDock():
    shipsWith1Docks(4),
    shipsWith2Docks(3),
    shipsWith3Docks(2),
    shipsWith4Docks(1)

{
    shipsCounters = {0};
}


