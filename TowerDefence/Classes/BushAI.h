//
//  BushAI.h
//  TowerDefence
//
//  Created by Pavel on 9/23/14.
//
//

#ifndef __TowerDefence__BushAI__
#define __TowerDefence__BushAI__
#include "AIInterface.h"

//GeorgeBush intellect simulation

class BushAI: public AIInterface
{
public:
    void performAttack(GameGridLayer* playerGrid);
};

#endif /* defined(__TowerDefence__BushAI__) */
