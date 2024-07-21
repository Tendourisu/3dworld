#pragma once

#include <memory>
#include <string>
#include <vector>
#include "BasePoint.hpp"
#include "Point.hpp"

template <std::size_t N, std::size_t P>
class Element {
    public:
        Element(const std::shared_ptr<Point<N>>* points);
        Element(const Element<N, P>& other);

        const std::vector<std::shared_ptr<Point<N>>> GetPointsVector() const;
        bool SetPoint(std::size_t index, const std::shared_ptr<Point<N>>& point);
        bool SetPoints(const std::shared_ptr<Point<N>>* points);

        static bool IsValid(const BasePoint<N>* points);
        static bool IsValid(const std::shared_ptr<Point<N>>* points);
        static bool IsSame(const Element<N, P>& e1, const Element<N, P>& e2);
        bool IsSame(const BasePoint<N>* points) const;
        bool IsSame(const Element<N, P>& other) const;

        virtual std::string ToString() const;

        Element<N, P>& operator=(const Element<N, P>& other);
        bool operator==(const Element<N, P>& other) const;
        bool operator!=(const Element<N, P>& other) const;

        template <std::size_t M, std::size_t Q>
        friend std::ostream& operator<<(std::ostream& stream, const Element<M, Q>& element);

    protected:
        Element() = default;
        std::shared_ptr<Point<N>> m_Points[P];
};

#include "Element.tpp"