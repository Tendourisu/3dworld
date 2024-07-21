#include <cmath>
#include <ostream>
#include <sstream>
#include <string>
#include "Point.hpp"
#include "../Errors.hpp"

template <std::size_t N>
Point<N>::Point(unsigned int id): Id(id) {}

template <std::size_t N>
Point<N>::Point(unsigned int id, const double* coordinates): BasePoint<N>(coordinates), Id(id) {}

template <std::size_t N>
Point<N>::Point(unsigned int id, const BasePoint<N>& other): BasePoint<N>(other), Id(id) {}

template <std::size_t N>
Point<N>::Point(const Point<N>& other): Point(other.Id, other) {}

template <std::size_t N>
Point<N>& Point<N>::operator=(const Point<N>& other) {
    if (this != &other) {
        Id = other.Id;
        BasePoint<N>::operator=(other);
    }
    return *this;
}

template <std::size_t N>
std::string Point<N>::ToString() const {
    return ToString(false);
}

template <std::size_t N>
std::string Point<N>::ToString(bool withId) const {
    std::ostringstream stream;
    if (withId) {
        stream << "#";
        stream << Id;
    }
    stream << BasePoint<N>::ToString();
    return stream.str();
}