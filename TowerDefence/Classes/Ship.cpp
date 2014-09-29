//
//  Ship.cpp
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//

#include "Ship.h"
#include <iostream>

static size_t shipCounter = 0;

class DeckSprite: public CCSprite
{
public:
    DeckSprite()
    {
        std::cout<<"Decksprite was allocated"<<std::endl;
    }
    ~DeckSprite()
    {
        std::cout<<"decksprite was deallocated"<<std::endl;
    }
};


Ship::Ship(ShipType type, ShipOrientation orientation, const Point2D<int>& point):
    shipType(type),
    shipOrientation(orientation),
    shipPositionInDeck(point),
    alpha(1.f),
    CCSprite(),
    shipState(ShipState::Norm),
    decksToShow({true, true,true,true}),
    deckStates(),
    isAllocated(false)


{
    CCSprite::init();
    this->setAnchorPoint({0.0f, 0.0f});
    int length = this->getNumberOfDecks();
    damage = length;
    if(orientation==ShipOrientation::horizontal) {
        setContentSize(CCSize(deckSizeInPixels*length, deckSizeInPixels));
        aabb.lowerBound  = Vec2f(0.f, 0.f);
        aabb.upperBound  = Vec2f(deckSizeInPixels/**length*/, deckSizeInPixels);
    }
    else {
        setContentSize(CCSize(deckSizeInPixels, deckSizeInPixels*length));
        aabb.lowerBound  = Vec2f(0.f, 0.f);
        aabb.upperBound  = Vec2f(deckSizeInPixels, /*length **/ deckSizeInPixels);
    }

    for(int i=0; i<getNumberOfDecks(); i++) {
        if(orientation==ShipOrientation::horizontal)
        {
            DeckSprite* deckSprite = new DeckSprite();//(CCSprite::create("Burst.png"));
            deckSprite->initWithFile("Burst.png");
            deckSprite->setScale(2.0f);
            deckSprite->setAnchorPoint({0.0f, 0.f});
            deckSprite->setPosition(ccp((float)i*deckSizeInPixels, 0));
            deckSprite->setVisible(false);
            deckSprite->retain();
            this->addChild(deckSprite);
            deckBurningSprites.push_back(deckSprite);
            //std::cout<<"deckSprite.push_back"<<std::endl;
        } else {
            DeckSprite* deckSprite = new DeckSprite();//(CCSprite::create("Burst.png"));
            deckSprite->initWithFile("Burst.png");
            deckSprite->setScale(2.0f);
            deckSprite->setAnchorPoint({0.0f, 0.f});
            deckSprite->setPosition(ccp(0, (float)i*deckSizeInPixels));
            deckSprite->setVisible(false);
            deckSprite->retain();
            this->addChild(deckSprite);
            deckBurningSprites.push_back(deckSprite);
        }
        deckStates[i] = CellState::Norm;
    }



}
const std::size_t cellSize = 19;

void Ship::setPosition(const CCPoint& pos)
{
//    std::cout<<"Ship set position"<<std::endl;
//    std::cout.flush();
//    
    CCSprite::setPosition(pos);
    Point2D<int> gridCoords;

//    aabb.lowerBound = Vec2f(pos.x, pos.y);
//    if(shipOrientation == ShipOrientation::horizontal)
//        aabb.upperBound = Vec2f(pos.x + getNumberOfDecks() * cellSize, pos.y );
//    else
//        aabb.upperBound = Vec2f(pos.x + deckSizeInPixels, pos.y + getNumberOfDecks() * deckSizeInPixels);
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

#define MAX_DECKS 4



void Ship::setDeckState(size_t deck, CellState deckState)
{
    deckStates.at(deck) = deckState;
}

CellState Ship::getDeckState(size_t deck) const
{
    return deckStates.at(deck);
}

void Ship::draw()
{
    if(m_shipVisible) {
    //draw a fucking rectangle
    if(shipState==ShipState::Norm||shipState==ShipState::Damaged)
        ccDrawSolidRect({0.f,0.f}, getContentSize(), {1.f, 1.f, 1.f, 1.f});
    else
        ccDrawSolidRect({0.f,0.f}, getContentSize(), {0.f, 0.f, 0.f, 1.f});
    }
    size_t numberOfDecks = this->getNumberOfDecks();
    for(int i=0; i<numberOfDecks; i++)
    {
        if(decksToShow[i])
        {
            //draw this deck depending on it's state
            CellState state = deckStates[i];

            switch(state)
            {
                case CellState::Hit:
                {
                    //draw a sprite over it
                    try{

                        deckBurningSprites.at(i)->setVisible(true);

                        if(shipState==ShipState::Destroyed) {
                            deckBurningSprites.at(i)->setVisible(false);
                            m_shipVisible = true;
                        }
                    }   catch(std::exception& ex)
                    {
                        std::cout<<ex.what();
                    }
                }
                default: ;//do nothing
            }
        }
    }
    CCSprite::draw();
}


std::size_t Ship::getNumberOfDecks()
{
    return static_cast<std::size_t>(shipType);
}

ShipOrientation Ship::getOrientation()
{
    return shipOrientation;
}

Ship* Ship::create(ShipType type, ShipOrientation orientation, const Point2D<int>& coords)
{
    std::cout<<"Ship object was allocated "<<shipCounter<<std::endl;
    shipCounter++;



    Ship *ship = new Ship(type, orientation, coords);
    if (ship)
    {
        ship->autorelease();
        return ship;
    }
    CC_SAFE_DELETE(ship);
    return NULL;
}