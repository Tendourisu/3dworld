#pragma once

#include <cstddef>

template <std::size_t N>
class BasePoint {
    public:
        BasePoint();
        BasePoint(const double* coordinates);
        BasePoint(const BasePoint<N>& other);

        double GetCoordinate(std::size_t index) const;
        void SetCoordinate(std::size_t index, double value);
        void SetCoordinates(const double* coordinates);

        static bool IsSame(const BasePoint<N>& p1, const BasePoint<N>& p2);
        bool IsSame(const BasePoint<N>& other) const;

        template <std::size_t M>
        static double Distance(const BasePoint<N> p1, const BasePoint<M> p2);
        template <std::size_t M>
        double Distance(const BasePoint<M>& other) const;
        
        virtual std::string ToString() const;

        BasePoint<N>& operator=(const BasePoint<N>& other);
        bool operator==(const BasePoint<N>& other) const;
        bool operator!=(const BasePoint<N>& other) const;

        template <std::size_t M>
        friend std::ostream& operator<<(std::ostream& stream, const BasePoint<M>& point);
        
    protected:
        template <std::size_t M>
        BasePoint<M> Upgrade() const;

    private:
        double m_Coordinates[N];
};

#include "BasePoint.tpp"