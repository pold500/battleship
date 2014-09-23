//
//  BushAI.cpp
//  TowerDefence
//
//  Created by Pavel on 9/23/14.
//
//

#include "BushAI.h"
#include "GameGridLayer.h"
#include <stdlib.h>

void BushAI::performAttack(GameGridLayer *playerGrid)
{
    int x = rand()%10;
    int y = rand()%10;
    while(playerGrid->isCellTested(x, y)) //while cell is known
    {
        x = rand()%10;
        y = rand()%10;
    }
    playerGrid->hitCell(x, y);
}