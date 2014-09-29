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

    std::vector<Ship*> shipArray;
    std::array<CCPoint, 4> shipOriginsPoint;

    CCRect shipOriginRect;
    CCRect shipDestRect;

    ShipDock* shipDock;
    Ship* currentlyMovingObject;

    std::array<CCLabelTTF*, 4> shipsCountLabelsArray;

    void bindToGridCells(const CCPoint& location, const CCRect& gridDimensions);
    void printCellNumber(const CCPoint& mouseLocation, const CCRect& gridDimensions);
    void updateViewCounters();

public:
    bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

    //virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

    static CCScene* scene();
    CREATE_FUNC(GameSetupLayer);
    
};

#endif /* defined(__TowerDefence__GameSetupLayer__) */
