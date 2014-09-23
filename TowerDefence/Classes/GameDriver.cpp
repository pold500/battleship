//
//  GameDriver.cpp
//  TowerDefence
//
//  Created by Pavel on 9/23/14.
//
//

#include "GameDriver.h"
#include "GameGridLayer.h"

GameDriver* GameDriver::instance = nullptr;

GameDriver* GameDriver::getInstance()
{
    if(instance==nullptr)
    {
        instance = new GameDriver();
        instance -> onEnter();
    }
    return instance;
}

void GameDriver::aiPerformAttack(GameGridLayer* userGrid)
{
    CanAttack = false;
    ai->performAttack(userGrid);
    this->scheduleOnce(schedule_selector(GameDriver::updateState),0.2f);
}

void GameDriver::updateState(float dt)
{
    CanAttack = true;
}


bool GameDriver::canAttack()
{
    return CanAttack;
}

