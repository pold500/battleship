//
//  Ship.cpp
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//

#include "Ship.h"

Ship::Ship(ShipType type, ShipOrientation orientation):
    shipType(type),
    shipOrientation(orientation),
    shipPositionInDeck(0,0),
alpha(1.f), CCSprite(), shipState(ShipState::Norm)

{
    CCSprite::init();
    this->setAnchorPoint({0.0f, 0.0f});
    int length = this->getNumberOfDecks();
    damage = length;
    if(orientation==ShipOrientation::horizontal) {
        this->setContentSize(CCSize(deckSizeInPixels*length, deckSizeInPixels));
    }
    else
        this->setContentSize(CCSize(deckSizeInPixels, deckSizeInPixels*length));
}


Ship::Ship(ShipType type, ShipOrientation orientation, const Point2D<int>& position):
shipType(type),
shipOrientation(orientation),
shipPositionInDeck(position),
alpha(1.f), CCSprite(), shipState(ShipState::Norm)
{
    CCSprite::init();
    this->setAnchorPoint({0.0f, 0.0f});

    int length = this->getNumberOfDecks();

    damage = length;


    if(orientation==ShipOrientation::horizontal) {
        this->setContentSize(CCSize(deckSizeInPixels*length, deckSizeInPixels));
    }
    else
        this->setContentSize(CCSize(deckSizeInPixels, deckSizeInPixels*length));
}


void Ship::setContentSizeDependOnOrientation(ShipOrientation shipOrientation)
{
    if(shipOrientation== ShipOrientation::horizontal)
    {
        this->setContentSize(CCSize(deckSizeInPixels, deckSizeInPixels));
    }
}

const Point2D<int>& Ship::getPositionInDeck() const
{
    return shipPositionInDeck;
}
void Ship::setPositionInDeck(const Point2D<int>& position)
{
    shipPositionInDeck = position;
}

ShipType Ship::getShipType() const
{
    return this->shipType;
}

void Ship::setShipType(ShipType type)
{
    this->shipType = type;
}

void Ship::setAlpha(float value)
{
    alpha = value;
}

void Ship::draw()
{

    auto localPos = this->getPosition();

    ccDrawSolidRect({0,0}, getContentSize(), {1.f, 1.f, 1.f, 1.f});

    CCSprite::draw();

}

const Point2D<int>& Ship::getShipPositionInDeck()
{
    return shipPositionInDeck;
}

std::size_t Ship::getNumberOfDecks()
{
    return static_cast<std::size_t>(shipType);
}

const CCPoint& Ship::getPosition()
{
    return CCSprite::getPosition();
}

ShipOrientation Ship::getOrientation()
{
    return shipOrientation;
}

Ship* Ship::create(ShipType type, ShipOrientation orientation, const Point2D<int>& coords)
{
    Ship *ship = new Ship(type, orientation, coords);
    if (ship)
    {
        ship->autorelease();
        return ship;
    }
    CC_SAFE_DELETE(ship);
    return NULL;
}