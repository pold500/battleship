//
//  Helpers.cpp
//  TowerDefence
//
//  Created by Pavel on 9/26/14.
//
//

#include "Helpers.h"
#include <cstdlib> //for srand
#include "Ship.h"

void fillWithShips(std::vector<Ship*>& shipsVector)
{
    srand(time(NULL));
//    ShipOrientation so = static_cast<ShipOrientation>( rand()%1 );
    Point2D<int> coord2d { rand()%9, rand()%9 };
    std::set<std::pair<Point2D<int>, Ship>> usedCoords;
    shipsVector.push_back((Ship::create(
                                        ShipType::Deck1,
                                        ShipOrientation::horizontal,
                                        Point2D<int>{0,0})));

    shipsVector.push_back((Ship::create(
                                        ShipType::Deck1,
                                        ShipOrientation::horizontal,
                                        Point2D<int>{0,2})));

    shipsVector.push_back((Ship::create(ShipType::Deck1, ShipOrientation::horizontal,
                                        Point2D<int>{0,4})));

    shipsVector.push_back((Ship::create(ShipType::Deck1, ShipOrientation::horizontal,
                                        Point2D<int>{0,6})));

    shipsVector.push_back((Ship::create(ShipType::Deck2, ShipOrientation::horizontal,
                                        Point2D<int>{7,8})));

    shipsVector.push_back((Ship::create(ShipType::Deck2, ShipOrientation::horizontal,
                                        Point2D<int>{3,8})));

    shipsVector.push_back((Ship::create(ShipType::Deck2, ShipOrientation::horizontal,
                                        Point2D<int>{3,6})));

    shipsVector.push_back((Ship::create(ShipType::Deck3, ShipOrientation::vertical,
                                        Point2D<int>{9,0})));

    shipsVector.push_back((Ship::create(ShipType::Deck3, ShipOrientation::vertical,
                                        Point2D<int>{7,0})));


    shipsVector.push_back((Ship::create(ShipType::Deck4, ShipOrientation::vertical,
                                        Point2D<int>{5,0})));


}
