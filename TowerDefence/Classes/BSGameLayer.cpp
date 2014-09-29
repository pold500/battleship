//
//  BSGameLayer.cpp
//  TowerDefence
//
//  Created by Pavel on 9/22/14.
//
//

#include "BSGameLayer.h"
#include "Ship.h"
#include <memory>
#include <set>

#include "Point2d.h"
#include "BSGameLayer.h"
#include "GameDriver.h"
#include "OsamaAI.h"


#include "Helpers.h"



bool BSGameLayer::init()
{
    OsamaAI *osamaAI = new OsamaAI();
    GameDriver::getInstance()->setAI(osamaAI);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    mPlayerGrid->setPosition(10 , 30);
    mEnemyGrid->setPosition( winSize.width / 2 + 20, 30);



    mPlayerGrid->setColor(ccColor3B{255,50,255});
    std::vector<Ship*> shipsVector;

    fillWithShips(shipsVector);

    mPlayerGrid->mShowFleet = true;

    mPlayerGrid->loadShips(shipsVector);

    std::vector<Ship*> shipsVector2;
    fillWithShips(shipsVector2);

    mEnemyGrid->mShowFleet = false;

    mEnemyGrid->loadShips(shipsVector2);

    CCSprite* background = CCSprite::create("Background.jpg");
    background->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
    this->addChild(background);

    this->addChild(mPlayerGrid.get());
    this->addChild(mEnemyGrid.get());

    this->setTouchEnabled(true);
    this->setTouchMode(ccTouchesMode::kCCTouchesOneByOne);
    return true;
}

const size_t cellSize = 19;


bool BSGameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(GameDriver::getInstance()->canAttack()) {
        CCPoint location = pTouch->getLocation();
        if(mEnemyGrid->boundingBox().containsPoint(location))
        {
            //detect grid coordinate
            CCPoint gridCoord = mEnemyGrid->convertToNodeSpace(location);
            int xCell = floor(gridCoord.x / cellSize);
            int yCell = floor(gridCoord.y / cellSize);
            if(!mEnemyGrid->isCellTested(xCell, yCell))
            {
                mEnemyGrid->hitCell(xCell, yCell);
                //mPlayerGrid->getGridStatus();
                GameDriver::getInstance()->aiPerformAttack(mPlayerGrid.get());
            }

        }
    }
    return true;
}

cocos2d::CCScene* BSGameLayer::scene()
{
    CCScene *scene = CCScene::create();
    BSGameLayer *layer = BSGameLayer::create();
    scene->addChild(layer);
    return scene;
}