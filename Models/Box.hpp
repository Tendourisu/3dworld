#pragma once

#include "BasePoint.hpp"

template <std::size_t N>
class Box {
    public:
        Box(const BasePoint<N>& vertex1, const BasePoint<N>& vertex2);
        Box(const Box<N>& other);

        const BasePoint<N>& Vertex1 { m_Vertex1 };
        const BasePoint<N>& Vertex2 { m_Vertex2 };

        void SetVertex(const BasePoint<N>& vertex1, const BasePoint<N>& vertex2);

        static bool IsSame(const Box<N>& b1, const Box<N>& b2);
        bool IsSame(const Box<N>& other) const;

        double GetVolume() const;

        Box<N>& operator=(const Box<N>& other);
        bool operator==(const Box<N>& other) const;
        bool operator!=(const Box<N>& other) const;
    
    private:
        BasePoint<N> m_Vertex1;
        BasePoint<N> m_Vertex2;
};

#include "Box.tpp"