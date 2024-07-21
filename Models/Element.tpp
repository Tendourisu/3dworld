#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "BasePoint.hpp"
#include "Element.hpp"
#include "Point.hpp"
#include "../Errors.hpp"

#include <iostream>

template <std::size_t N, std::size_t P>
Element<N, P>::Element(const std::shared_ptr<Point<N>>* points) {
    if (!SetPoints(points)) {
        throw IdenticalPointException();
    }
}

template <std::size_t N, std::size_t P>
Element<N, P>::Element(const Element<N, P>& other): Element(other.m_Points) {}

template <std::size_t N, std::size_t P>
const std::vector<std::shared_ptr<Point<N>>> Element<N, P>::GetPointsVector() const {
    return std::vector<std::shared_ptr<Point<N>>>(m_Points, m_Points + P);
}

template <std::size_t N, std::size_t P>
bool Element<N, P>::SetPoint(std::size_t index, const std::shared_ptr<Point<N>>& point) {
    if (index >= P) {
        throw IndexOutOfBoundsException(index);
    }
    for (int i = 0; i < P; i++) {
        if (i == index) continue;
        if (*m_Points[i] == *point) return false;
    }
    m_Points[index] = point;
    return true;
}

template <std::size_t N, std::size_t P>
bool Element<N, P>::IsValid(const BasePoint<N>* points) {
    for (int i = 0; i < P; i++) {
        for (int j = i + 1; j < P; j++) {
            if (points[i] == points[j]) return false;
        }
    }
    return true;
}

template <std::size_t N, std::size_t P>
bool Element<N, P>::IsValid(const std::shared_ptr<Point<N>>* points) {
    for (int i = 0; i < P; i++) {
        for (int j = i + 1; j < P; j++) {
            if (*points[i] == *points[j]) return false;
        }
    }
    return true;
}

template <std::size_t N, std::size_t P>
bool Element<N, P>::SetPoints(const std::shared_ptr<Point<N>>* points) {
    if (IsValid(points)) {
        for (int i = 0; i < P; i++) {
            m_Points[i] = points[i];
        }
        return true;
    }
    else {
        return false;
    }
}

template <std::size_t N, std::size_t P>
bool Element<N, P>::IsSame(const Element<N, P>& e1, const Element<N, P>& e2) {
    for (int i = 0; i < P; i++) {
        if (*e1.m_Points[i] != *e2.m_Points[i]) return false;
    }
    return true;
}

template <std::size_t N, std::size_t P>
bool Element<N, P>::IsSame(const BasePoint<N>* points) const {
    for (int i = 0; i < P; i++) {
        if (*m_Points[i] != points[i]) return false;
    }
    return true;
}

template <std::size_t N, std::size_t P>
bool Element<N, P>::IsSame(const Element<N, P>& other) const {
    return IsSame(*this, other);
}

template <std::size_t N, std::size_t P>
std::string Element<N, P>::ToString() const {
    std::ostringstream stream;
    stream << "[ ";
    for (int i = 0; i < P; i++) {
        stream << *m_Points[i] << " ";
    }
    stream << "]";
    return stream.str();
}

template <std::size_t N, std::size_t P>
Element<N, P>& Element<N, P>::operator=(const Element<N, P>& other) {
    if (this != &other) {
        SetPoints(other.m_Points);
    }
    return *this;
}

template <std::size_t N, std::size_t P>
bool Element<N, P>::operator==(const Element<N, P>& other) const {
    return IsSame(other);
}

template <std::size_t N, std::size_t P>
bool Element<N, P>::operator!=(const Element<N, P>& other) const {
    return !IsSame(other);
}

template <std::size_t N, std::size_t P>
std::ostream& operator<<(std::ostream& stream, const Element<N, P>& element) {
    stream << element.ToString();
    return stream;
}