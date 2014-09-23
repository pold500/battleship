//
//  StartScreenLayer.h
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//

#ifndef __TowerDefence__StartScreenLayer__
#define __TowerDefence__StartScreenLayer__

#include <iostream>

#include "cocos2d.h"

using namespace cocos2d;

enum PlayerChoice
{
    startGame,
    loadSettings,
    quitGame
};

class StartScreenLayer : public cocos2d::CCLayer, public cocos2d::CCTargetedTouchDelegate
{
private:
    PlayerChoice playerChoice;
    CCArray      menuButtons ;
    CCSprite*    startGameSprite;
    CCSprite*    loadSettingsSprite;
    CCSprite*    quitGameSprite;
    //StartScreenLayer()=delete;
public:
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual bool ccTouchBegan  (CCTouch *pTouch, CCEvent *pEvent);
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(StartScreenLayer);
};




#endif /* defined(__TowerDefence__StartScreenLayer__) */
