//
//  Point2D.h
//  TowerDefence
//
//  Created by Pavel on 9/19/14.
//
//

#ifndef TowerDefence_Point2D_h
#define TowerDefence_Point2D_h

template<typename T>
struct Point2D
{
    T x,y;

    Point2D(T _x, T _y):x(_x), y(_y){}
    Point2D(){}
    const bool operator==(const Point2D<T>& rhs) const
    {
        return (this->x==rhs.x&&this->y==rhs.y)?true:false;
    }
    const bool operator<(const Point2D<int>& rhs) const
    {
        return (rhs.x<this->x&&rhs.y<this->y)?true:false;
    }
    const Point2D<T>& operator+(const Point2D& rhs) const
    {
        return Point2D<T>(this->x + rhs.x, this->y + rhs.y );
    }

};

#endif
