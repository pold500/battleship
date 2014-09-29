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
        return ((this->x * 10 + this->y) == (rhs.x * 10 + rhs.y));
    }
    const bool operator<(const Point2D<int>& rhs) const
    {
        return ((this->x * 10 + this->y) <  (rhs.x * 10 + rhs.y));
    }
    const Point2D<T> operator+(const Point2D& rhs) const
    {
        return Point2D<T>(this->x + rhs.x, this->y + rhs.y );
    }
    const Point2D<T> operator*(const Point2D<int>& rhs) const
    {
        return Point2D<int>(this->x*rhs.x, this->y*rhs.y);
    }
    const Point2D<T> operator*(const int rhs) const
    {
        return Point2D<int>(this->x*rhs, this->y*rhs);
    }
    const Point2D<T> operator=(const Point2D<int>& rhs)
    {
        this->x =  rhs.x;
        this->y =  rhs.y;
        return Point2D<T>(this->x, this->y);
    }
    const Point2D<T>& operator+=(const Point2D<int>& rhs)
    {
        return Point2D<T>(this->x+rhs.x, this->y*rhs.y);
    }
};

#endif
