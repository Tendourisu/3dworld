#pragma once

#include <memory>
#include <ostream>
#include <string>
#include "Element.hpp"
#include "Point.hpp"

template <std::size_t N>
class Line: public Element<N, 2> {
    public:
        const std::shared_ptr<Point<N>>& Start { Element<N, 2>::m_Points[0] };
        const std::shared_ptr<Point<N>>& End { Element<N, 2>::m_Points[1] };

        Line(const std::shared_ptr<Point<N>>& start, const std::shared_ptr<Point<N>>& end);
        Line(const Line<N>& other);

        bool SetStart(const std::shared_ptr<Point<N>>& start);
        bool SetEnd(const std::shared_ptr<Point<N>>& end);
        double GetLength() const;

        std::string ToString() const override;

        Line<N>& operator=(const Line<N>& other);
};

#include "Line.tpp"