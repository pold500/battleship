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
#include <array>
#include <vector>
#include <iostream>
USING_NS_CC;

#include "CellState.h"

enum ShipType
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


    Ship(ShipType type, ShipOrientation orientation, const Point2D<int>& position={0,0});

    float alpha;
    ShipType shipType;
    ShipOrientation shipOrientation;

    void implaceDamage()
    {
        damage--;
        if(damage==0)
            shipState = ShipState::Destroyed;
    }

    std::array<bool, 4> decksToShow;

    /* Top left corner position from where ship start drawing */
    CCPoint headPosition;
    Point2D<int> shipPositionInDeck;
    const std::size_t deckSizeInPixels = 19;
    void setContentSizeDependOnOrientation(ShipOrientation shipOrientation);

    std::vector<CCSprite*> deckBurningSprites;
    size_t damage;


    Ship()=delete;


public:
    std::array<CellState, 4> deckStates;
    ShipState shipState;

    static std::shared_ptr<Ship> create()
    {
        std::shared_ptr<Ship> pA(Ship::create());
        return pA;
    }
    ~Ship()
    {
        std::cout<<"Ship was deallocated"<<std::endl;
    }

    //Static factory
    static Ship*        create(ShipType type,
                               ShipOrientation orientation = ShipOrientation::horizontal,
                               const Point2D<int>& coords={0,0});

    //Getters and setters
    ShipType            getShipType() const;
    void                setShipType(ShipType type);
    //const CCPoint&  getPosition();

    const Point2D<int>& getPositionInDeck() const;
    void                setPositionInDeck(const Point2D<int>& position);
    ShipOrientation     getOrientation();
    void                setOrientation(const ShipOrientation&);
    /* Damage to a cells */
    void      setDeckState(size_t deck, CellState);
    CellState getDeckState(size_t deck) const;

    ShipState getShipState() { return shipState; }

    void      setShipState()=delete; //this is set through accessor to damage

    CellState setCellDamage(const float x, const  float y) //in ship coords
    {
        //Detect what cell did we hit
        CCPoint start {0, 0};
        CCPoint end   { (float)deckSizeInPixels, (float)deckSizeInPixels};

        Point2D<int> pp = this->getPositionInDeck();
        //transform grid coords to ship coords
        CCPoint deckCoords;
        deckCoords.x = x - pp.x * deckSizeInPixels;
        deckCoords.y = y - pp.y * deckSizeInPixels;

        size_t cellNumber = (shipOrientation==ShipOrientation::horizontal)? deckCoords.x / deckSizeInPixels :
        deckCoords.y / deckSizeInPixels ;

#define MAX_NUMBER_OF_DECKS 4
        if(cellNumber==MAX_NUMBER_OF_DECKS)
            return CellState::Miss;

        CellState state =  deckStates[cellNumber];

        if(state==CellState::Norm)
        {
            state = deckStates[cellNumber] = CellState::Hit;
        } else {
            //we already hit in that cell. da faq?
        }
//        for(int i=0; i<getNumberOfDecks(); i++)
//        {
//            if(shipOrientation==ShipOrientation::horizontal) {
//                CCRect  rect (i*deckSizeInPixels, start.y, i*deckSizeInPixels + end.x , end.y );
//                if(rect.containsPoint(ccp(deckCoords.x,deckCoords.y)))
//                {
//                    state = deckStates[i] = CellState::Hit;
//                    implaceDamage();
//                    break;
//                }
//            } else {
//                CCRect  rect (0, i*deckSizeInPixels, end.x, end.y + i*deckSizeInPixels);
//                if(rect.containsPoint(ccp(x,y)))
//                {
//                    state = deckStates[i] = CellState::Hit;
//                    implaceDamage();
//                    break;
//                }
//            }
//        }
        return state;
    }
    
    CellState getCellDamage(const float x, const float y)
    {
        //Detect what cell did we hit
        CCPoint start {0, 0};
        CCPoint end   { (float)deckSizeInPixels, (float)deckSizeInPixels};

        Point2D<int> pp = this->getPositionInDeck();
        //transform grid coords to ship coords
        CCPoint deckCoords;
        deckCoords.x = x - pp.x * deckSizeInPixels;
        deckCoords.y = y - pp.y * deckSizeInPixels;

        size_t cellNumber = (shipOrientation==ShipOrientation::horizontal)? deckCoords.x / deckSizeInPixels :
        deckCoords.y / deckSizeInPixels ;

        return deckStates[cellNumber];
    }

    std::size_t  getNumberOfDecks();
    void         setAlpha(float value);

    //Inherited interface

    virtual void draw(void);
    virtual void setPosition(const CCPoint& pos);


};



#endif /* defined(__TowerDefence__Ship__) */
