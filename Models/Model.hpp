#pragma once

#include <memory>
#include <vector>
#include "BasePoint.hpp"
#include "Box.hpp"
#include "Face.hpp"
#include "Line.hpp"
#include "Point.hpp"

// Model class.
template <std::size_t N>
class Model {
    public:
        Model();
        Model(std::string name);
        Model(const Model<N>& other) = delete;
        
        std::string Name;
        const std::vector<std::shared_ptr<Line<N>>>& Lines { m_Lines };
        const std::vector<std::shared_ptr<Face<N>>>& Faces { m_Faces };

        std::vector<std::shared_ptr<Point<N>>> CollectPoints() const;
        void AddLine(
            const BasePoint<N>& start,
            const BasePoint<N>& end
        );
        void AddFace(
            const BasePoint<N>& first,
            const BasePoint<N>& second,
            const BasePoint<N>& third
        );
        void AddLineUnchecked(const Line<N>& line);
        void AddFaceUnchecked(const Face<N>& face);
        void ModifyLine(
            std::size_t index,
            int pointIndex,
            const BasePoint<N>& point
        );
        void ModifyFace(
            std::size_t index,
            int pointIndex,
            const BasePoint<N>& point
        );
        bool RemoveLine(std::size_t index);
        bool RemoveFace(std::size_t index);

        void Clear();

        Box<N> BoundingBox() const;

    private:
        unsigned int m_MaxPointId { 0 };
        std::vector<std::shared_ptr<Line<N>>> m_Lines;
        std::vector<std::shared_ptr<Face<N>>> m_Faces;

        std::shared_ptr<Point<N>> GetOrAddPoint(const BasePoint<N>& point);
        void ShiftPoints();
};

#include "Model.tpp"