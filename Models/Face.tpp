#include <cmath>
#include <sstream>
#include <string>
#include "Face.hpp"
#include "Point.hpp"
#include "../Errors.hpp"

template <std::size_t N>
Face<N>::Face(
    const std::shared_ptr<Point<N>>& first, 
    const std::shared_ptr<Point<N>>& second, 
    const std::shared_ptr<Point<N>>& third
) {
    std::shared_ptr<Point<N>> points[] = { first, second, third };
    if (!this->SetPoints(points)) {
        throw IdenticalPointException();
    }
}

template <std::size_t N>
Face<N>::Face(const Face<N>& other): Element<N, 3>(other) {}

template <std::size_t N>
bool Face<N>::SetFirst(const std::shared_ptr<Point<N>>& first) {
    return SetPoint(0, first);
}

template <std::size_t N>
bool Face<N>::SetSecond(const std::shared_ptr<Point<N>>& second) {
    return SetPoint(1, second);
}

template <std::size_t N>
bool Face<N>::SetThird(const std::shared_ptr<Point<N>>& third) {
    return SetPoint(2, third);
}

template <std::size_t N>
double Face<N>::GetArea() const {
    double a = Point<N>::Distance(*First, *Second);
    double b = Point<N>::Distance(*Second, *Third);
    double c = Point<N>::Distance(*Third, *First);
    double p = (a + b + c) / 2;
    return std::sqrt(p * (p - a) * (p - b) * (p - c));
}

template <std::size_t N>
std::string Face<N>::ToString() const {
    std::ostringstream stream;
    stream << "Face ";
    stream << Element<N, 3>::ToString();
    return stream.str();
}

template <std::size_t N>
Face<N>& Face<N>::operator=(const Face<N>& other) {
    if (this != &other) {
        Element<N, 3>::operator=(other);
    }
    return *this;
}