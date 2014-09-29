//
//  RTree.h
//  TowerDefence
//
//  Created by Pavel on 9/19/14.
//
//

#ifndef __TowerDefence__RTree__
#define __TowerDefence__RTree__
#include <vector>
#include <cocos2d.h>

template <typename T>
class RTree {


public:
    RTree(const std::vector<T>& vector);
    void putElementInTree(const T& object);
    void popElementFromTree(const cocos2d::CCPoint& coord);
    bool testElementByCoord(const cocos2d::CCPoint& coord);
    
};


#endif /* defined(__TowerDefence__RTree__) */
