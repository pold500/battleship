//
//  GameSetupLayer.cpp
//  TowerDefence
//
//  Created by Pavel on 9/18/14.
//
//

#include "GameSetupLayer.h"
#include "Ship.h"
#include "ShipDock.h"
#include "BSGameLayer.h"
#include <string>

CCSprite* blankSpriteWithSize(const CCSize& size);

const std::size_t gridCells = 10;

bool GameSetupLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint location = pTouch->getLocation();

    //process buttons
    //nextSprite
    //randomButton
    if(nextSprite->boundingBox().containsPoint(location))
    {
        CCDirector::sharedDirector()->replaceScene(BSGameLayer::scene());
        return true;
    }

    for(Ship* ship : shipArray)
    {
        CCRect rect = ship->boundingBox();
        if(ship->boundingBox().containsPoint(location))
        {
            currentlyMovingObject = ship;
            break;
        }
        //break;
    }
    return true;
}

const float cellSize = 19.f;
const int numberOfCells = 10;


void GameSetupLayer::printCellNumber(const CCPoint& mouseLocation, const CCRect& gridDimensions)
{
    if(gridDimensions.containsPoint(mouseLocation))
    {
        CCPoint gridSpace  = mouseLocation;//grid->convertToNodeSpace(shipLocation);
        CCPoint globalSpace = this->convertToWorldSpace(mouseLocation);
        //CCPoint currentPos = currentlyMovingObject->getPosition();
        //detect the cell delimiter more close to the target
        //currentlyMovingObject->setPosition(, );

        CCPoint reminders, residues;

        reminders.x = floor((globalSpace.x - gridDimensions.origin.x ) / cellSize );
        reminders.y = floor((globalSpace.y - gridDimensions.origin.y ) / cellSize );

        shipsCountLabelsArray[0]->setString(std::to_string(reminders.x).c_str());
        shipsCountLabelsArray[1]->setString(std::to_string(reminders.y).c_str());
    }
}

void GameSetupLayer::bindToGridCells(const CCPoint& shipLocation, const CCRect& gridDimensions)
{
    if(gridDimensions.containsPoint(shipLocation))
    {
        CCPoint gridSpace  = shipLocation;//grid->convertToNodeSpace(shipLocation);
        CCPoint globalSpace = this->convertToWorldSpace(shipLocation);
        CCPoint currentPos = currentlyMovingObject->getPosition();
        //detect the cell delimiter more close to the target
        //currentlyMovingObject->setPosition(, );

        CCPoint reminders, residues;

        reminders.x = floor((globalSpace.x - gridDimensions.origin.x ) / cellSize );
        reminders.y = floor((globalSpace.y - gridDimensions.origin.y ) / cellSize );

        residues.x  = fmodf((globalSpace.x - gridDimensions.origin.x ) , cellSize );
        residues.y  = fmodf((globalSpace.y - gridDimensions.origin.y ) , cellSize );


        if(reminders.x == numberOfCells || reminders.y == numberOfCells)
            return;

        CCPoint localToGridCoord { globalSpace.x - gridDimensions.origin.x,
                                   globalSpace.y - gridDimensions.origin.y };

        if( residues.x < ( cellSize / 2.f ) )
        {
            localToGridCoord.x = reminders.x + (cellSize * 0.5f);
        }
        if( residues.y < (cellSize / 2.f))
        {
            localToGridCoord.y = reminders.y + (cellSize * 0.5f);
        }
        CCPoint displacement {gridDimensions.origin};
        CCPoint globalCoords = localToGridCoord + displacement;
        currentlyMovingObject->setPosition(globalCoords);
    }
}

void GameSetupLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    printCellNumber(pTouch->getLocationInView(), CCRect(grid->getPosition().x, grid->getPosition().y,
                                                        grid->getContentSize().width, grid->getContentSize().height));
    if(currentlyMovingObject!=nullptr) {
        currentlyMovingObject->setPosition(ccp(pTouch->getLocation().x, pTouch->getLocation().y));
        bindToGridCells(pTouch->getLocationInView(), CCRect(grid->getPosition().x, grid->getPosition().y,
                                                      grid->getContentSize().width, grid->getContentSize().height));
        currentlyMovingObject->setAlpha(0.5f);
    }
}
void GameSetupLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(currentlyMovingObject!=nullptr)
    {
        CCRect rect = currentlyMovingObject->boundingBox();
        if(!shipDestRect.intersectsRect(rect))
        {
            //drop the ship to the dock
            CCPoint currPos = currentlyMovingObject->getPosition();
            CCPoint point = shipOriginsPoint[currentlyMovingObject->getNumberOfDecks()-1];
            currentlyMovingObject->setPosition(point);

        }
        else
        {
            //it intersects
            //turn on binding to a grid
            //decrease the count in dock
            //update view counters
            //get the ship type
            //decrease model counter
            //update view counter
            
            ShipType type = currentlyMovingObject->getShipType();
            ShipDock::getInstance()->decreaseCounter(type);
            updateViewCounters();
        }
        currentlyMovingObject->setAlpha(1.f);
        currentlyMovingObject = nullptr;
    }
}

void GameSetupLayer::updateViewCounters()
{
    shipsCountLabelsArray[0]->setString(std::to_string(ShipDock::getInstance()->getFreeShipsCount(ShipType::Deck1)).c_str());
    shipsCountLabelsArray[1]->setString(std::to_string(ShipDock::getInstance()->getFreeShipsCount(ShipType::Deck2)).c_str());
    shipsCountLabelsArray[2]->setString(std::to_string(ShipDock::getInstance()->getFreeShipsCount(ShipType::Deck3)).c_str());
    shipsCountLabelsArray[3]->setString(std::to_string(ShipDock::getInstance()->getFreeShipsCount(ShipType::Deck4)).c_str());
}

void GameSetupLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}

bool GameSetupLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }



    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* background = CCSprite::create("Background.jpg");
    background->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
    this->addChild(background);

    CCLabelTTF *text = CCLabelTTF::create("Game setup", "Verdana", 25.f, CCSize(225,40), CCTextAlignment::kCCTextAlignmentLeft);

    text->setPosition(CCPoint(270, 290));
    this->addChild(text);

    nextSprite   = CCSprite::create("nextButton.jpg");
    nextSprite->setPosition(CCPoint(400, 10));
    this->addChild(nextSprite);

    randomButton = CCSprite::create("randomButton.jpg");
    randomButton->setPosition(CCPoint(50, 10));
    this->addChild(randomButton);

    grid = new GameGridLayer();
    grid->setPosition(CCPoint(10, 50));
    grid->setContentSize(CCSize(200,200));
    auto pos  = grid->getPosition();
    auto size = grid->getContentSize();
    this->addChild(grid);

    this->shipDestRect.setRect(10, 135, 19*10, 19*10);

    //std::string to_string( int value );
    shipDock = ShipDock::getInstance();

    shipsCountLabelsArray[0] = CCTextFieldTTF::create(std::to_string(
                                shipDock->getFreeShipsCount(ShipType::Deck1)).c_str(), "Verdana", 15.f,
                                CCSize(50,20), CCTextAlignment::kCCTextAlignmentCenter);
    shipsCountLabelsArray[0]->setPosition(ccp(250, 260));


    shipsCountLabelsArray[1] = CCTextFieldTTF::create(std::to_string(shipDock->getFreeShipsCount(ShipType::Deck2)).c_str(),
                                                         "Verdana", 15.f,
                                                          CCSize(50,20), CCTextAlignment::kCCTextAlignmentCenter);
    shipsCountLabelsArray[1]->setPosition(ccp(260, 260));


    shipsCountLabelsArray[2] = CCTextFieldTTF::create(std::to_string(shipDock->getFreeShipsCount(ShipType::Deck3)).c_str(),
                                                         "Verdana", 15.f,
                                                         CCSize(50,20), CCTextAlignment::kCCTextAlignmentCenter);;
    shipsCountLabelsArray[2]->setPosition(ccp(270, 260));

    shipsCountLabelsArray[3] = CCTextFieldTTF::create(std::to_string(shipDock->getFreeShipsCount(ShipType::Deck4)).c_str(),
                                                         "Verdana", 15.f,
                                                         CCSize(50,20), CCTextAlignment::kCCTextAlignmentCenter);
    shipsCountLabelsArray[3]->setPosition(ccp(280, 260));


    this->addChild(shipsCountLabelsArray[0]);
    this->addChild(shipsCountLabelsArray[1]);
    this->addChild(shipsCountLabelsArray[2]);
    this->addChild(shipsCountLabelsArray[3]);

    Ship *ship0 = shipDock->getShipFromDock(ShipType::Deck1);
    ship0->setAnchorPoint({0.5,0.5});
    shipOriginsPoint[0] = CCPoint(250,230);

    ship0->setPosition(CCPoint(250,230));

    shipArray.push_back(ship0);
    this->addChild(ship0);

    Ship *ship1 = Ship::create(ShipType::Deck2);
    ship1->setAnchorPoint({0.5,0.5});
    shipOriginsPoint[1] = CCPoint(250+19/2.,210);

    ship1->setPosition(CCPoint(250+19/2.,210));

    shipArray.push_back(ship1);
    this->addChild(ship1);
//
//    Ship *ship2 = Ship::create(ShipType::Deck3);
//    shipOriginsPoint[2] = CCPoint(250,190);
//    ship2->setPosition(CCPoint(250,190));
//    shipArray[2] = ship2;
//    this->addChild(ship2);
//
//    Ship *ship3 = Ship::create(ShipType::Deck4);
//    shipOriginsPoint[3] = CCPoint(250,170);
//    ship3->setPosition(CCPoint(250,170));
//    shipArray[3] = ship3;
//    this->addChild(ship3);

    this->shipOriginRect.setRect(250, 230, 100, 100);

    this->setTouchEnabled(true);
    this->setTouchMode(ccTouchesMode::kCCTouchesOneByOne);

    return true;
}

CCSprite* blankSpriteWithSize(const CCSize& size)
{
    CCSprite *sprite = new CCSprite();
    GLubyte  *buffer = (GLubyte*)malloc(sizeof(GLubyte)*4);
    for (int i=0;i<4;i++) {buffer[i]=255;}
    CCTexture2D *tex = new CCTexture2D();
    tex->initWithData(buffer, CCTexture2DPixelFormat::kCCTexture2DPixelFormat_RGB5A1,
                                                 1, 1, size);
    sprite->setTexture(tex);
    sprite->setTextureRect(CCRectMake(0, 0, size.width, size.height));
    free(buffer);
    return sprite;
}

cocos2d::CCScene* GameSetupLayer::scene()
{
    CCScene *scene = CCScene::create();
    GameSetupLayer *layer = GameSetupLayer::create();
    scene->addChild(layer);
    return scene;
}