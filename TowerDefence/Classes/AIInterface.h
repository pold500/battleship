//
//  AIInterface.h
//  TowerDefence
//
//  Created by Pavel on 9/23/14.
//
//

#ifndef TowerDefence_AIInterface_h
#define TowerDefence_AIInterface_h

class GameGridLayer;

class AIInterface
{
public:
    virtual void performAttack(GameGridLayer* playerGrid)=0;
};


#endif
