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






bool BSGameLayer::init()
{
    OsamaAI *_ai = new OsamaAI();
    GameDriver::getInstance()->setAI(_ai);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    player->setPosition(10 , 30);
    ai->setPosition( winSize.width / 2 + 20, 30);



    player->setColor(ccColor3B{255,50,255});
    std::vector<Ship*> shipsVector;

    //fillWithShips(shipsVector);

    Ship* ship = Ship::create(ShipType::Deck3, ShipOrientation::horizontal,
                              Point2D<int>{0,4});
    ship->shipState = ShipState::Destroyed;
    shipsVector.push_back(ship);

    ship = Ship::create(ShipType::Deck4, ShipOrientation::horizontal, Point2D<int>(2,5));
    ship->shipState = ShipState::Destroyed;
    shipsVector.push_back(ship);

    player->m_showFleet = true;

    player->loadShips(shipsVector);


    //player->loadShips(shipsVector); //load ships vector for a player screen on the left
    //same for the screen on the right

//    std::vector<Ship*> shipsVector2;
//    fillWithShips(shipsVector2);
//    ai->m_showFleet = true;
//    ai->loadShips(shipsVector2);

    CCSprite* background = CCSprite::create("Background.jpg");
    background->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
    this->addChild(background);

    this->addChild(player.get());
    this->addChild(ai.get());

    this->setTouchEnabled(true);
    this->setTouchMode(ccTouchesMode::kCCTouchesOneByOne);
    return true;
}

const size_t cellSize = 19;


bool BSGameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(GameDriver::getInstance()->canAttack()) {
        CCPoint location = pTouch->getLocation();
        if(ai->boundingBox().containsPoint(location))
        {
            //detect grid coordinate
            CCPoint gridCoord = ai->convertToNodeSpace(location);
            int xCell = floor(gridCoord.x / cellSize);
            int yCell = floor(gridCoord.y / cellSize);
            if(!ai->isCellTested(xCell, yCell))
            {
                ai->hitCell(xCell, yCell);
                //ai->hitCell(gridCoord);
                GameDriver::getInstance()->aiPerformAttack(player.get());
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