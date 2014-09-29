//
//  ShipVisualTestLayer.h
//  TowerDefence
//
//  Created by Pavel on 9/26/14.
//
//

#ifndef __TowerDefence__ShipVisualTestLayer__
#define __TowerDefence__ShipVisualTestLayer__
#include <cocos2d.h>
#include <iostream>
class ShipVisualTestLayer : public cocos2d::CCLayer
{
public:
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(ShipVisualTestLayer);
};



#endif /* defined(__TowerDefence__ShipVisualTestLayer__) */
