//
//  MatrixAdapter.h
//  TowerDefence
//
//  Created by Pavel on 9/25/14.
//
//

#ifndef __TowerDefence__MatrixAdapter__
#define __TowerDefence__MatrixAdapter__

#include <iostream>
#include <array>

template<typename T, size_t size>
class MatrixIterator
{
    std::array<T, size> row;
public:
    const T& operator[](const size_t index)
    {
        return row.at(index);
    }
};

template<typename T, size_t size>
class MatrixAdapter
{
public:
    MatrixAdapter();
    MatrixIterator<T, size> operator[](const size_t index);
};

#endif /* defined(__TowerDefence__MatrixAdapter__) */
