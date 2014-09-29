//
//  GameDriver.h
//  TowerDefence
//
//  Created by Pavel on 9/23/14.
//
//

#ifndef __TowerDefence__GameDriver__
#define __TowerDefence__GameDriver__
#include "AIInterface.h"
#include <cocos2d.h>

class GameGridLayer;

class GameDriver: public cocos2d::CCNode //for timer
{
    GameDriver():CanAttack(true)
    { CCNode::init(); }
    static GameDriver* instance;
    AIInterface* ai;
    void updateState(float dt);
    bool CanAttack;
public:
    static GameDriver* getInstance();
    void setAI(AIInterface* ai) { this->ai = ai; }
    bool canAttack();
    void aiPerformAttack(GameGridLayer* userGrid);

};

#endif /* defined(__TowerDefence__GameDriver__) */
