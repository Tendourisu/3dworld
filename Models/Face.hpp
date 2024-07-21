#pragma once

#include <memory>
#include <ostream>
#include <string>
#include "Element.hpp"
#include "Point.hpp"

template <std::size_t N>
class Face: public Element<N, 3> {
    public:
        const std::shared_ptr<Point<N>>& First = { Element<N, 3>::m_Points[0] };
        const std::shared_ptr<Point<N>>& Second = { Element<N, 3>::m_Points[1] };
        const std::shared_ptr<Point<N>>& Third = { Element<N, 3>::m_Points[2] };

        Face(
            const std::shared_ptr<Point<N>>& first,
            const std::shared_ptr<Point<N>>& second,
            const std::shared_ptr<Point<N>>& third
        );
        Face(const Face<N>& other);

        bool SetFirst(const std::shared_ptr<Point<N>>& first);
        bool SetSecond(const std::shared_ptr<Point<N>>& second);
        bool SetThird(const std::shared_ptr<Point<N>>& third);
        double GetArea() const;

        std::string ToString() const override;

        Face<N>& operator=(const Face<N>& other);
};

#include "Face.tpp"