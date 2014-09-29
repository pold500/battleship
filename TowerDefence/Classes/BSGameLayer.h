//
//  BSGameLayer.h
//  TowerDefence
//
//  Created by Pavel on 9/22/14.
//
//

#ifndef __TowerDefence__BSGameLayer__
#define __TowerDefence__BSGameLayer__
#include "cocos2d.h"
#include <memory>
#include "GameGridLayer.h"

USING_NS_CC;

class BSGameLayer: public CCLayer {
    //create two grids
    std::unique_ptr<GameGridLayer> mPlayerGrid;
    std::unique_ptr<GameGridLayer> mEnemyGrid;
    BSGameLayer():mPlayerGrid(new GameGridLayer()), mEnemyGrid(new GameGridLayer())
    {}
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(BSGameLayer);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

};
#endif /* defined(__TowerDefence__BSGameLayer__) */
