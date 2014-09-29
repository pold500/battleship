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

    for(Ship* ship : mStaticShipsArray)
    {
        if(ship->boundingBox().containsPoint(location))
        {
            mCurrentlyMovingObject = ship;
            break;
        }
    }

    if(mCurrentlyMovingObject!=nullptr)
        return true;

    for(Ship* ship: mAllocatedShipsArray)
    {
        if(ship->boundingBox().containsPoint(location))
        {
            mCurrentlyMovingObject = ship;
        }
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
        //CCPoint currentPos = mCurrentlyMovingObject->getPosition();
        //detect the cell delimiter more close to the target
        //mCurrentlyMovingObject->setPosition(, );

        CCPoint reminders, residues;

        reminders.x = floor((globalSpace.x - gridDimensions.origin.x ) / cellSize );
        reminders.y = floor((globalSpace.y - gridDimensions.origin.y ) / cellSize );

        mShipsCountLabelsArray[0]->setString(std::to_string(reminders.x).c_str());
        mShipsCountLabelsArray[1]->setString(std::to_string(reminders.y).c_str());
    }
}

void GameSetupLayer::bindToGridCells(const CCPoint& shipLocationGlobal, const CCRect& gridDimensions)
{
    if(gridDimensions.containsPoint(shipLocationGlobal))
    {
        CCPoint shipPositionGrid = grid->convertToNodeSpace(shipLocationGlobal);

        CCPoint reminders, residues;
        reminders.x = floor(shipPositionGrid.x / cellSize );
        reminders.y = floor(shipPositionGrid.y / cellSize );
        if(reminders.x == numberOfCells || reminders.y == numberOfCells)
            return;

        CCPoint newCoordsGrid ( reminders.x * cellSize, reminders.y * cellSize );
        CCPoint coordsGlobal = grid->convertToWorldSpace(newCoordsGrid);
        mCurrentlyMovingObject->setAnchorPoint(ccp(0.f, 0.f));
        mCurrentlyMovingObject->setPosition(coordsGlobal);
    }
}

void GameSetupLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if(mCurrentlyMovingObject!=nullptr) {
        bool isStatic = false;
        for(Ship* ship: mStaticShipsArray)
        {
            if(mCurrentlyMovingObject==ship)
            {
                isStatic = true; //user tries to move a static ship
                break;
            }
        }
        if(isStatic) //allocate a new ship and set handle to it.
        {
            Ship* const ship = mCurrentlyMovingObject;
            if(ShipDock::getInstance()->getFreeShipsCount(ship->getShipType())!=0)
            {
                mCurrentlyMovingObject = Ship::create(ship->getShipType());
                mCurrentlyMovingObject->setPosition(ship->getPosition());
                mCurrentlyMovingObject->setAnchorPoint(ccp(0.f,0.f));
                mCurrentlyMovingObject->setContentSize(ship->getContentSize());
                mCurrentlyMovingObject->setVisible(true);
                mCurrentlyMovingObject->retain();
                this->addChild(mCurrentlyMovingObject);

                ShipDock::getInstance()->decreaseCounter(ship->getShipType());
                updateViewCounters();
                mAllocatedShipsArray.push_back(mCurrentlyMovingObject);
                it =  mAllocatedShipsArray.begin();
                return;
            }
            else
                return;
        }

        mCurrentlyMovingObject->setPosition(ccp(pTouch->getLocation().x, pTouch->getLocation().y));
        if(grid->boundingBox().intersectsRect(mCurrentlyMovingObject->boundingBox()))
        {
            bindToGridCells(pTouch->getLocation(), CCRect(grid->getPosition().x,
                                                          grid->getPosition().y,
                                                          grid->getContentSize().width,
                                                          grid->getContentSize().height));
            grid->isCanPlace(pTouch->getLocation(), mCurrentlyMovingObject->getShipType(),
                             mCurrentlyMovingObject->getOrientation());
            grid->setShip(mCurrentlyMovingObject);
            grid->displayMarkline = true;
        }
        else
        {
            grid->displayMarkline = false;
        }
        mCurrentlyMovingObject->setAlpha(0.5f);
    }
}
void GameSetupLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(mCurrentlyMovingObject!=nullptr)
    {
        if(!grid->boundingBox().intersectsRect(mCurrentlyMovingObject->boundingBox()))
        {
            //drop the ship back to the dock
            mCurrentlyMovingObject->setPosition(mShipPositionsInDock[mCurrentlyMovingObject->getShipType()-1]);
            mCurrentlyMovingObject->setAnchorPoint(ccp(0.f, 0.f));
            //remove the ship from the vector.
            ShipDock::getInstance()->increaseCounter((*it)->getShipType());
            updateViewCounters();
            this->removeChild(*it);
            mAllocatedShipsArray.erase(it);
        }
        else
        {
            //it intersects with a grid
            //turn on binding to a grid
            //decrease the count in dock
            //update view counters
            //get the ship type
            //decrease model counter
            //update view counter
            CCPoint gridCoordinates = grid->convertToNodeSpace(pTouch->getLocation());
            Point2D<int> gridPoint = { static_cast<int>(gridCoordinates.x / cellSize), static_cast<int>(gridCoordinates.y / cellSize) };
            Ship* ship = mCurrentlyMovingObject;
            grid->insertShipAndFillAdjacentPoints(gridPoint, ship->getShipType(), ship->getOrientation(),
                                                  grid->getPointsSet());
            ShipType type = mCurrentlyMovingObject->getShipType();
            if(ShipDock::getInstance()->getFreeShipsCount(type)==0)
            {
                mStaticShipsArray.at((int)type-1)->setAlpha(0.3f);
            }
            updateViewCounters();
        }

        mCurrentlyMovingObject->setAlpha(1.f);
        mCurrentlyMovingObject = nullptr;
    }
}

void GameSetupLayer::updateViewCounters()
{
    mShipsCountLabelsArray[0]->setString(std::to_string(ShipDock::getInstance()->getFreeShipsCount(ShipType::Deck1)).c_str());
    mShipsCountLabelsArray[1]->setString(std::to_string(ShipDock::getInstance()->getFreeShipsCount(ShipType::Deck2)).c_str());
    mShipsCountLabelsArray[2]->setString(std::to_string(ShipDock::getInstance()->getFreeShipsCount(ShipType::Deck3)).c_str());
    mShipsCountLabelsArray[3]->setString(std::to_string(ShipDock::getInstance()->getFreeShipsCount(ShipType::Deck4)).c_str());
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



    CCSize winSize       = CCDirector::sharedDirector()->getWinSize();
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

    ShipDock* dockDataModel = ShipDock::getInstance();


    mShipsCountLabelsArray[0] = CCTextFieldTTF::create(std::to_string(
                                dockDataModel->getFreeShipsCount(ShipType::Deck1)).c_str(), "Verdana", 15.f,
                                CCSize(50,20), CCTextAlignment::kCCTextAlignmentCenter);
    mShipsCountLabelsArray[0]->setPosition(ccp(250, 260));


    mShipsCountLabelsArray[1] = CCTextFieldTTF::create(std::to_string(dockDataModel->getFreeShipsCount(ShipType::Deck2)).c_str(),
                                                         "Verdana", 15.f,
                                                          CCSize(50,20), CCTextAlignment::kCCTextAlignmentCenter);
    mShipsCountLabelsArray[1]->setPosition(ccp(260, 260));


    mShipsCountLabelsArray[2] = CCTextFieldTTF::create(std::to_string(dockDataModel->getFreeShipsCount(ShipType::Deck3)).c_str(),
                                                         "Verdana", 15.f,
                                                         CCSize(50,20), CCTextAlignment::kCCTextAlignmentCenter);;
    mShipsCountLabelsArray[2]->setPosition(ccp(270, 260));

    mShipsCountLabelsArray[3] = CCTextFieldTTF::create(std::to_string(dockDataModel->getFreeShipsCount(ShipType::Deck4)).c_str(),
                                                         "Verdana", 15.f,
                                                         CCSize(50,20), CCTextAlignment::kCCTextAlignmentCenter);
    mShipsCountLabelsArray[3]->setPosition(ccp(280, 260));


    mShipsDock = CCSprite::create("DockBack.png");
    mShipsDock->setPosition(ccp(250, 120));
    mShipsDock->setContentSize(ccp(100, 100));
    mShipsDock->setAnchorPoint(ccp(0.f, 0.f));
    mShipsDock->setVisible(true);
    this->addChild(mShipsDock);
    mShipPositionsInDock[0] = CCPoint(250, 220-20);
    mShipPositionsInDock[1] = CCPoint(250, 200-20);
    mShipPositionsInDock[2] = CCPoint(250, 180-20);
    mShipPositionsInDock[3] = CCPoint(250, 160-20);

    this->addChild(mShipsCountLabelsArray[0]);
    this->addChild(mShipsCountLabelsArray[1]);
    this->addChild(mShipsCountLabelsArray[2]);
    this->addChild(mShipsCountLabelsArray[3]);

    Ship *ship = Ship::create(ShipType::Deck1);
    ship->setAnchorPoint({0.f,0.f});
    ship->setPosition(CCPoint(mShipPositionsInDock[0]));
    ship->setVisible(true);
    mStaticShipsArray.push_back(ship);
    this->addChild(ship);

    ship = Ship::create(ShipType::Deck2);
    ship->setAnchorPoint({0.f,0.f});
    ship->setPosition(CCPoint(mShipPositionsInDock[1]));
    ship->setVisible(true);
    mStaticShipsArray.push_back(ship);
    this->addChild(ship);

    ship = Ship::create(ShipType::Deck3);
    ship->setAnchorPoint({0.f,0.f});
    ship->setPosition(CCPoint(mShipPositionsInDock[2]));
    ship->setVisible(true);
    mStaticShipsArray.push_back(ship);
    this->addChild(ship);

    ship = Ship::create(ShipType::Deck4);
    ship->setAnchorPoint({0.f,0.f});
    ship->setPosition(CCPoint(mShipPositionsInDock[3]));
    ship->setVisible(true);
    mStaticShipsArray.push_back(ship);
    this->addChild(ship);

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