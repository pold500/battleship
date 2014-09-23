//
//  Ship.h
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//

#ifndef __TowerDefence__Ship__
#define __TowerDefence__Ship__
#include <cocos2d.h>
#include "Point2D.h"
#include "QuadTreeOccupant.h"

USING_NS_CC;



enum class ShipType
{
    Deck1=1,
    Deck2,
    Deck3,
    Deck4
};

enum class ShipOrientation
{
    horizontal,
    vertical
};

enum class ShipState
{
    Norm,
    Damaged,
    Destroyed
};
class Ship: public CCSprite, public QuadTreeOccupant {
    //DISALLOW_COPY_AND_ASSIGN(Ship);
    float alpha;
    ShipType shipType;
    ShipOrientation shipOrientation;
    Ship()=delete;

    /* Top left corner position from where ship start drawing */
    CCPoint headPosition;
    Point2D<int> shipPositionInDeck;
    const std::size_t deckSizeInPixels = 19;
    void setContentSizeDependOnOrientation(ShipOrientation shipOrientation);

    Ship(ShipType type, ShipOrientation orientation);
    Ship(ShipType type, ShipOrientation orientation, const Point2D<int>& position={0,0});
    size_t damage;

    ShipState shipState;
public:
    //static Ship* create(ShipType type, ShipOrientation orientation = ShipOrientation::horizontal);
    static Ship* create(ShipType type, ShipOrientation orientation = ShipOrientation::horizontal, const Point2D<int>& coords={0,0});
    ShipType        getShipType() const;
    void            setShipType(ShipType type);
    const CCPoint& getPosition();
    const Point2D<int>& getPositionInDeck() const;
    void setPositionInDeck(const Point2D<int>& position);
    ShipOrientation getOrientation();

    ShipState getShipState() { return shipState; }

    void setDamage()
    {
        damage--;
        shipState = ShipState::Damaged;
        if(damage==0)
        {
            shipState = ShipState::Destroyed;
        }

    }

    virtual void draw(void);

    std::size_t getNumberOfDecks();
    const Point2D<int>& getShipPositionInDeck();
    void setAlpha(float value);
};



#endif /* defined(__TowerDefence__Ship__) */
