//
//  StartScreenLayer.cpp
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//

#include "StartScreenLayer.h"
#include "GameSetupLayer.h"
#include "GameSettingsLayer.h"

//class GameSetupLayer;
//class GameSettingsLayer;

bool StartScreenLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }

    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCSprite* background = CCSprite::create("Background.jpg");
    background->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
    this->addChild(background);

    const std::size_t heightOfButton         = 80 / 2;
    const std::size_t distanceBetweenButtons =  5;
    //Place other sprite buttons and title here
    CCLabelTTF* ttf1 = CCLabelTTF::create("Battleships", "Verdana", 45,
                                          CCSizeMake(350, 90), kCCTextAlignmentLeft);
    ttf1->setPosition(CCPoint(290,220));
    ttf1->setFontFillColor(ccColor3B{255,120,30});
    this->addChild(ttf1);

    std::size_t spriteYPosition = 165; //start position

    this->startGameSprite = CCSprite::create("startGameSprite.png");
    startGameSprite->setPosition(CCPoint(230,spriteYPosition));
    this->addChild(startGameSprite);

    //menuSprites.addObject(this->startGameSprite);

    spriteYPosition -= ( distanceBetweenButtons + heightOfButton );

    this->loadSettingsSprite = CCSprite::create("settingsButtonSprite.png");
    loadSettingsSprite->setPosition(CCPoint(230, spriteYPosition));
    this->addChild(loadSettingsSprite);

    //menuSprites.addObject(this->settingsButtonSprite);

    spriteYPosition -= ( distanceBetweenButtons + heightOfButton );

    this->quitGameSprite = CCSprite::create("quitButtonSprite.png");
    quitGameSprite->setPosition(CCPoint(230,  spriteYPosition));
    this->addChild(quitGameSprite);

    this->setTouchEnabled(true);

    return true;

}




bool StartScreenLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void StartScreenLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    //Converting coords of touch point.
    CCTouch* touch   = (CCTouch*)pTouches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

    //Finding a button we've clicked on.
    CCSprite* sprite = startGameSprite;
    if(sprite->boundingBox().containsPoint(location)){
        //Start game
        CCDirector::sharedDirector()->replaceScene(CCTransitionSplitCols::create(1, GameSetupLayer::scene()));
        return;
    }
    sprite = loadSettingsSprite;
    if(sprite->boundingBox().containsPoint(location)){
        //Load settings layer
        CCDirector::sharedDirector()->replaceScene(CCTransitionSplitCols::create(1, GameSettingsLayer::scene()));
        return;
    }
    sprite = quitGameSprite;
    if(sprite->boundingBox().containsPoint(location)){
        //quit game
        CCDirector::sharedDirector()->end();
        return;
    }
}

cocos2d::CCScene* StartScreenLayer::scene()
{
    CCScene *scene = CCScene::create();
    StartScreenLayer *layer = StartScreenLayer::create();
    scene->addChild(layer);
    return scene;
}