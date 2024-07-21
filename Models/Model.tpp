#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include "BasePoint.hpp"
#include "Box.hpp"
#include "Face.hpp"
#include "Line.hpp"
#include "Point.hpp"
#include "Model.hpp"
#include "../Errors.hpp"

template <std::size_t N>
Model<N>::Model(): Name("") {}

template <std::size_t N>
Model<N>::Model(std::string name): Name(name) {}

template <std::size_t N>
std::vector<std::shared_ptr<Point<N>>> Model<N>::CollectPoints() const {
    std::vector<std::shared_ptr<Point<N>>> points;
    std::for_each(m_Lines.begin(), m_Lines.end(), [&](auto line) {
        for (auto point: line->GetPointsVector()) {
            if (std::any_of(points.begin(), points.end(), [&](auto p){
                return p->Id == point->Id;
            })) continue;
            points.push_back(point);
        }
    });
    std::for_each(m_Faces.begin(), m_Faces.end(), [&](auto face) {
        for (auto point: face->GetPointsVector()) {
            if (std::any_of(points.begin(), points.end(), [&](auto p){
                return p->Id == point->Id;
            })) continue;
            points.push_back(point);
        }
    });
    std::sort(points.begin(), points.end(), [&](auto p1, auto p2) { return p1->Id < p2->Id; });
    return points;
}

template <std::size_t N>
void Model<N>::AddLine(
    const BasePoint<N>& start, 
    const BasePoint<N>& end
) {
    const BasePoint<N> points[2] = { start, end };
    if (!Line<N>::IsValid(points)) {
        throw IdenticalPointException();
    }
    for (auto line: m_Lines) {
        if (line->IsSame(points)) {
            throw IdenticalElementException();
        }
    }
    Line<N> line(GetOrAddPoint(start), GetOrAddPoint(end));
    AddLineUnchecked(line);
}

template <std::size_t N>
void Model<N>::AddFace(
    const BasePoint<N>& first,
    const BasePoint<N>& second,
    const BasePoint<N>& third
) {
    const BasePoint<N> points[3] = { first, second, third };
    if (!Face<N>::IsValid(points)) {
        throw IdenticalPointException();
    }
    for (auto face: m_Faces) {
        if (face->IsSame(points)) {
            throw IdenticalElementException();
        }
    }
    Face<N> face(GetOrAddPoint(first), GetOrAddPoint(second), GetOrAddPoint(third));
    AddFaceUnchecked(face);
}

template <std::size_t N>
void Model<N>::AddLineUnchecked(const Line<N>& line) {
    for (auto point: line.GetPointsVector()) {
        if (point->Id > m_MaxPointId) {
            m_MaxPointId = point->Id;
        }
    }
    m_Lines.push_back(std::shared_ptr<Line<N>>(new Line<N>(line)));
}

template <std::size_t N>
void Model<N>::AddFaceUnchecked(const Face<N>& face) {
    for (auto point: face.GetPointsVector()) {
        if (point->Id > m_MaxPointId) {
            m_MaxPointId = point->Id;
        }
    }
    m_Faces.push_back(std::shared_ptr<Face<N>>(new Face<N>(face)));
}

template <std::size_t N>
void Model<N>::ModifyLine(
    std::size_t index,
    int pointIndex,
    const BasePoint<N>& point
) {
    if (index >= m_Lines.size())
        throw IndexOutOfBoundsException(index);
    auto pointPtr = GetOrAddPoint(point);
    m_Lines[index]->SetPoint(pointIndex, pointPtr);
    ShiftPoints();
}

template <std::size_t N>
void Model<N>::ModifyFace(
    std::size_t index,
    int pointIndex,
    const BasePoint<N>& point
) {
    if (index >= m_Faces.size())
        throw IndexOutOfBoundsException(index);
    auto pointPtr = GetOrAddPoint(point);
    m_Faces[index]->SetPoint(pointIndex, pointPtr);
    ShiftPoints();
}

template <std::size_t N>
bool Model<N>::RemoveLine(std::size_t index) {
    if (index >= m_Lines.size()) return false;
    m_Lines.erase(m_Lines.begin() + index);
    ShiftPoints();
    return true;
}

template <std::size_t N>
bool Model<N>::RemoveFace(std::size_t index) {
    if (index >= m_Faces.size()) return false;
    m_Faces.erase(m_Faces.begin() + index);
    ShiftPoints();
    return true;
}

template <std::size_t N>
void Model<N>::Clear() {
    m_Lines.clear();
    m_Faces.clear();
}

template <std::size_t N>
Box<N> Model<N>::BoundingBox() const {
    BasePoint<N> min;
    BasePoint<N> max;
    for (auto point: CollectPoints()) {
        for (int i = 0; i < N; i++) {
            double coord = point->GetCoordinate(i);
            if (coord < min.GetCoordinate(i)) {
                min.SetCoordinate(i, coord);
            }
            if (coord > max.GetCoordinate(i)) {
                max.SetCoordinate(i, coord);
            }
        }
    }
    return Box<N>(min, max);
}

template <std::size_t N>
std::shared_ptr<Point<N>> Model<N>::GetOrAddPoint(const BasePoint<N>& point) {
    auto points = CollectPoints();
    auto pointIter = std::find_if(points.begin(), points.end(), [&](auto p){ return *p == point; });
    if (pointIter == points.end()) {
        return std::shared_ptr<Point<N>>(new Point<N>(++m_MaxPointId, point));
    }
    return *pointIter;
}

template <std::size_t N>
void Model<N>::ShiftPoints() {
    auto points = CollectPoints();
    for (int i = 0; i < points.size(); i++) {
        points[i]->Id = i + 1;
    }
    m_MaxPointId = points.size();
}
