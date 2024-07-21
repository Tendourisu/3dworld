#include <cmath>
#include <ostream>
#include <sstream>
#include <string>
#include "BasePoint.hpp"
#include "../Errors.hpp"

template <std::size_t N>
BasePoint<N>::BasePoint() {
    double zeros[N] = {0};
    SetCoordinates(zeros);
}

template <std::size_t N>
BasePoint<N>::BasePoint(const double* coordinates) {
    SetCoordinates(coordinates);
}

template <std::size_t N>
BasePoint<N>::BasePoint(const BasePoint<N>& other): BasePoint(other.m_Coordinates) {}

template <std::size_t N>
double BasePoint<N>::GetCoordinate(std::size_t index) const {
    if (index >= N) {
        throw IndexOutOfBoundsException(index);
    }
    return m_Coordinates[index];
}

template <std::size_t N>
void BasePoint<N>::SetCoordinate(std::size_t index, double value) {
    if (index >= N) {
        throw IndexOutOfBoundsException(index);
    }
    m_Coordinates[index] = value;
}

template <std::size_t N>
void BasePoint<N>::SetCoordinates(const double *coordinates) {
    for (int i = 0; i < N; i++) {
        SetCoordinate(i, coordinates[i]);
    }
}

template <std::size_t N>
bool BasePoint<N>::IsSame(const BasePoint<N>& p1, const BasePoint<N>& p2) {
    for (int i = 0; i < N; i++) {
        if (p1.m_Coordinates[i] != p2.m_Coordinates[i]) {
            return false;
        }
    }
    return true;
}

template <std::size_t N>
bool BasePoint<N>::IsSame(const BasePoint<N> &other) const {
    return IsSame(*this, other);
}

template <std::size_t N>
template <std::size_t M>
BasePoint<M> BasePoint<N>::Upgrade() const {
    if (M < N) {
        throw DowngradeException();
    }
    double coordinates[M] = { 0 };
    for (int i = 0; i < N; i++) {
        coordinates[i] = GetCoordinate(i);
    }
    return BasePoint<M>(coordinates);
}

template <std::size_t N>
template <std::size_t M>
double BasePoint<N>::Distance(const BasePoint<N> p1, const BasePoint<M> p2) {
    if (N == M) {
        double sumOfSquares = 0;
        for (int i = 0; i < N; i++) {
            sumOfSquares += std::pow(p1.GetCoordinate(i) - p2.GetCoordinate(i), 2);
        }
        return std::sqrt(sumOfSquares);
    }
    else if (N > M) {
        return p1.Distance(p2.template Upgrade<N>());
    }
    else {
        return p2.Distance(p1.template Upgrade<M>());
    }
}

template <std::size_t N>
template <std::size_t M>
double BasePoint<N>::Distance(const BasePoint<M> &other) const {
    return Distance(*this, other);
}

template <std::size_t N>
std::string BasePoint<N>::ToString() const {
    std::ostringstream stream;
    stream << "[ ";
    for (int i = 0; i < N; i++) {
        stream << GetCoordinate(i) << " ";
    }
    stream << "]";
    return stream.str();
}

template <std::size_t N>
BasePoint<N>& BasePoint<N>::operator=(const BasePoint<N>& other) {
    if (this != &other) {
        SetCoordinates(other.m_Coordinates);
    }
    return *this;
}

template <std::size_t N>
bool BasePoint<N>::operator==(const BasePoint<N> &other) const {
    return IsSame(other);
}

template <std::size_t N>
bool BasePoint<N>::operator!=(const BasePoint<N> &other) const {
    return !IsSame(other);
}

template <std::size_t N>
std::ostream& operator<<(std::ostream& stream, const BasePoint<N>& point) {
    stream << point.ToString();
    return stream;
}