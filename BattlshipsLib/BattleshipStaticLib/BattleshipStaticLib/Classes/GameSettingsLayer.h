//
//  GameSettingsLayer.h
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//

#ifndef __TowerDefence__GameSettingsLayer__
#define __TowerDefence__GameSettingsLayer__

#include <cocos2d.h>

USING_NS_CC;

class GameSettingsLayer: public cocos2d::CCLayer {


public:

bool checkCirclesCollide(CCPoint center1, float radius1, CCPoint center2, float radius2);
virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

virtual bool init();
static cocos2d::CCScene* scene();
CREATE_FUNC(GameSettingsLayer);

};



#endif /* defined(__TowerDefence__GameSettingsLayer__) */
