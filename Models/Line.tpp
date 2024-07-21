#include <sstream>
#include <string>
#include "Element.hpp"
#include "Line.hpp"
#include "Point.hpp"
#include "../Errors.hpp"

template <std::size_t N>
Line<N>::Line(const std::shared_ptr<Point<N>>& start, const std::shared_ptr<Point<N>>& end) {
    std::shared_ptr<Point<N>> points[] = { start, end };
    if (!this->SetPoints(points)) {
        throw IdenticalPointException();
    }
}

template <std::size_t N>
Line<N>::Line(const Line<N>& other): Element<N, 2>(other) {}

template <std::size_t N>
bool Line<N>::SetStart(const std::shared_ptr<Point<N>>& start) {
    return SetPoint(0, start);
}

template <std::size_t N>
bool Line<N>::SetEnd(const std::shared_ptr<Point<N>>& end) {
    return SetPoint(1, end);
}

template <std::size_t N>
double Line<N>::GetLength() const {
    return Point<N>::Distance(*Start, *End);
}

template <std::size_t N>
std::string Line<N>::ToString() const {
    std::ostringstream stream;
    stream << "Line ";
    stream << Element<N, 2>::ToString();
    return stream.str();
}

template <std::size_t N>
Line<N>& Line<N>::operator=(const Line<N>& other) {
    if (this != &other) {
        Element<N, 2>::operator=(other);
    }
    return *this;
}