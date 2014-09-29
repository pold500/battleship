//
//  GameSettingsLayer.cpp
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//

#include "GameSettingsLayer.h"

bool GameSettingsLayer::checkCirclesCollide(CCPoint center1, float radius1, CCPoint center2, float radius2)
{
    return true;
}

void GameSettingsLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    return;
}

bool GameSettingsLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* background = CCSprite::create("Background.jpg");
    background->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
    this->addChild(background);

    return true;
}

cocos2d::CCScene* GameSettingsLayer::scene()
{
    CCScene *scene = CCScene::create();
    GameSettingsLayer *layer = GameSettingsLayer::create();
    scene->addChild(layer);
    return scene;
}
