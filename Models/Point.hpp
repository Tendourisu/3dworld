#pragma once

#include <ostream>
#include <string>
#include "BasePoint.hpp"

template <std::size_t N>
class Point: public BasePoint<N> {
    public:
        Point(unsigned int id);
        Point(unsigned int id, const double* coordinates);
        Point(unsigned int id, const BasePoint<N>& other);
        Point(const Point<N>& other);

        unsigned int Id;

        Point<N>& operator=(const Point<N>& other);
        
        std::string ToString() const;
        std::string ToString(bool withId) const;
};

#include "Point.tpp"