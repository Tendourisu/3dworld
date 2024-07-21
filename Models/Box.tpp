#include <cmath>

#include "Box.hpp"

template <std::size_t N>
Box<N>::Box(const BasePoint<N>& vertex1, const BasePoint<N>& vertex2) {
    SetVertex(vertex1, vertex2);
}

template <std::size_t N>
Box<N>::Box(const Box<N>& other): Box(other.Vertex1, other.Vertex2) {}

template <std::size_t N>
void Box<N>::SetVertex(const BasePoint<N>& vertex1, const BasePoint<N>& vertex2) {
    for (int i = 0; i < N; i++) {
        if (vertex1.GetCoordinate(i) < vertex2.GetCoordinate(i)) {
            m_Vertex1.SetCoordinate(i, vertex1.GetCoordinate(i));
            m_Vertex2.SetCoordinate(i, vertex2.GetCoordinate(i));
        }
        else {
            m_Vertex1.SetCoordinate(i, vertex2.GetCoordinate(i));
            m_Vertex2.SetCoordinate(i, vertex1.GetCoordinate(i));
        }
    }
}

template <std::size_t N>
bool Box<N>::IsSame(const Box<N>& b1, const Box<N>& b2) {
    return b1.Vertex1 == b2.Vertex1 && b1.Vertex2 == b2.Vertex2;
}

template <std::size_t N>
bool Box<N>::IsSame(const Box<N>& other) const {
    return IsSame(*this, other);
}

template <std::size_t N>
double Box<N>::GetVolume() const {
    double volume = 1;
    for (int i = 0; i < N; i++) {
        volume *= (m_Vertex2.GetCoordinate(i) - m_Vertex1.GetCoordinate(i));
    }
    return volume;
}

template <std::size_t N>
Box<N>& Box<N>::operator=(const Box<N>& other) {
    if (this != &other) {
        SetVertex(other.m_Vertex1, other.m_Vertex2);
    }
    return *this;
}

template <std::size_t N>
bool Box<N>::operator==(const Box<N>& other) const {
    return IsSame(other);
}

template <std::size_t N>
bool Box<N>::operator!=(const Box<N>& other) const {
    return !IsSame(other);
}