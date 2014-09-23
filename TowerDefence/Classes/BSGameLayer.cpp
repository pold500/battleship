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


static void fillWithShips(std::vector<std::shared_ptr<Ship>>& shipsVector)
{
    srand(time(NULL));
    ShipOrientation so = static_cast<ShipOrientation>( rand()%1 );
    Point2D<int> coord2d { rand()%9, rand()%9 };
    std::set<std::pair<Point2D<int>, Ship>> usedCoords;
    Ship* ship = Ship::create(ShipType::Deck4, so, coord2d);

}


static void fillTheMatrix(int matrix[10][10])
{
    matrix = {0};
    //1. Fill in the 4 deck ship depending on orientation.
    //2. Fill the 'occupancy' matrix
    srand(time(NULL));
    Point2D<int> pt {rand()%9, rand()%9};
    matrix[pt.x][pt.y];
    srand(time(NULL));
    int orientation = rand()%1; //0 - horiz, 1 - vertical

    int occupancyMatrix[10][10] = {0};

    if(orientation==0)
    {

        for(int i=pt.x; i<pt.x+4; i++)
        {
            matrix[i][pt.y] = 1;
            //occupancyMatrix
        }
    } else
    {
        for(int i=pt.y; i<pt.y+4; i++)
        {
            matrix[pt.x][i] = 1;
        }
    }

    ///std::set<Point2D<int>> usedPoints;

    if(orientation==0)
    {
        for(size_t i=((pt.x-1)<0)?pt.x:pt.x-1; i<((pt.x-1)<0)?pt.x+5:pt.x+6; i++)
            for(size_t j=(pt.y-1<0)?pt.y:pt.y-1; j<(pt.y-1<0)?pt.y+2:pt.y+3; j++) {
                occupancyMatrix[i][j] = 1;
                //usedPoints.insert(Point2D<int>(i, j));
            }
    }
    else
    {
        for(int i=((pt.x-1)<0)?pt.x:pt.x-1; i<(pt.x-1<0)?pt.x+5:pt.x+6; i++)
            for(int j=(pt.y-1<0)?pt.y:pt.y-1; j<(pt.y-1)?pt.y+4:pt.y+5; j++) {
                occupancyMatrix[i][j] = 1;
               // usedPoints.insert({i, j});
            }
    }

    pt = {rand()%10, rand()%10};
   // while(usedPoints.find(pt) != usedPoints.end())
    {
        pt = {rand()%10, rand()%10};
    }
   // usedPoints.insert(pt);

    //Build 2x 3d ship
    orientation = rand()%1;
    if(orientation==0)
    {
        for(int i = pt.x; i< pt.x+3 ; i++)
            matrix[i][pt.y] = 1;
    } else {
        for(int i= pt.y; i< pt.y+3; i++)
            matrix[pt.x][i];
    }
    if(orientation==0)
    {
        int i=pt.x;
        int j=pt.y;
        if(!(i-1<0)) i--;
        if(!(j-1<0)) j--;
        for(;i<pt.x+3; i++)
            for(;j<pt.y+3; j++)
            {

            }
    } else
    {

    }

}

bool BSGameLayer::init()
{
    OsamaAI *_ai = new OsamaAI();
    GameDriver::getInstance()->setAI(_ai);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    player->setPosition(10 , 30);
    ai->setPosition( winSize.width / 2 + 20, 30);
    
    player->setColor(ccColor3B{255,50,255});
    std::vector<std::shared_ptr<Ship>> shipsVector;

    fillWithShips(shipsVector);

    shipsVector.push_back(std::shared_ptr<Ship>(Ship::create(
                                                ShipType::Deck1,
                                                ShipOrientation::horizontal,
                                                Point2D<int>{0,0})));

    shipsVector.push_back(std::shared_ptr<Ship>(Ship::create(
                                                             ShipType::Deck1,
                                                             ShipOrientation::horizontal,
                                                             Point2D<int>{0,2})));

    shipsVector.push_back(std::shared_ptr<Ship>(Ship::create(ShipType::Deck1, ShipOrientation::horizontal,
                                                             Point2D<int>{0,4})));

    shipsVector.push_back(std::shared_ptr<Ship>(Ship::create(ShipType::Deck1, ShipOrientation::horizontal,
                                                             Point2D<int>{0,6})));

    shipsVector.push_back(std::shared_ptr<Ship>(Ship::create(ShipType::Deck2, ShipOrientation::horizontal,
                                                             Point2D<int>{7,8})));

    shipsVector.push_back(std::shared_ptr<Ship>(Ship::create(ShipType::Deck2, ShipOrientation::horizontal,
                                                             Point2D<int>{3,8})));

    shipsVector.push_back(std::shared_ptr<Ship>(Ship::create(ShipType::Deck2, ShipOrientation::horizontal,
                                                             Point2D<int>{3,6})));

    shipsVector.push_back(std::shared_ptr<Ship>(Ship::create(ShipType::Deck3, ShipOrientation::vertical,
                                                             Point2D<int>{9,0})));

    shipsVector.push_back(std::shared_ptr<Ship>(Ship::create(ShipType::Deck3, ShipOrientation::vertical,
                                                             Point2D<int>{7,0})));


    shipsVector.push_back(std::shared_ptr<Ship>(Ship::create(ShipType::Deck4, ShipOrientation::vertical,
                                                             Point2D<int>{5,0})));
    player->showFleet = true;


    player->loadShips(shipsVector); //load ships vector for a player screen on the left
    //same for the screen on the right
    ai->loadShips(shipsVector);

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