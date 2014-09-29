//
//  GameSetupLayer.h
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//

#ifndef __TowerDefence__GameSetupLayer__
#define __TowerDefence__GameSetupLayer__

#include <cocos2d.h>
#include "GameGridLayer.h"
#include <array>

#include "Ship.h"
#include "Shipdock.h"

USING_NS_CC;

class GameSetupLayer:  public CCLayer {
    CCSprite* nextSprite;
    CCSprite* randomButton;

    GameGridLayer* grid;

    std::vector<Ship*> mStaticShipsArray;
    std::vector<Ship*> mAllocatedShipsArray;
    std::vector<Ship*>::iterator it;
    struct
    {
        uint8_t _4deck;
        uint8_t _3deck;
        uint8_t _2deck;
        uint8_t _1deck;
    } mCounters;

    CCSprite* mShipsDock;
    Ship*     mCurrentlyMovingObject;

    std::array<CCPoint,     4> mShipPositionsInDock;
    std::array<CCLabelTTF*, 4> mShipsCountLabelsArray;

    void bindToGridCells(const CCPoint& location, const CCRect& gridDimensions);
    void printCellNumber(const CCPoint& mouseLocation, const CCRect& gridDimensions);
    void updateViewCounters();

public:
    bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

    static CCScene* scene();
    CREATE_FUNC(GameSetupLayer);
    
};

#endif /* defined(__TowerDefence__GameSetupLayer__) */
