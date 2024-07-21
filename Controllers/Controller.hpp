#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../Models/BasePoint.hpp"
#include "../Models/Line.hpp"
#include "../Models/Face.hpp"
#include "../Models/Model.hpp"
#include "../Models/Point.hpp"

using Line3D = Line<3>;
using Face3D = Face<3>;
using Model3D = Model<3>;
using Point3D = Point<3>;
using BasePoint3D = BasePoint<3>;

class Controller {
    public:
        Controller() = default;
        Controller(const Controller& other) = delete;

        enum class Result {
            R_OK,
            R_FILE_OPEN_ERROR,
            R_FILE_FORMAT_ERROR,
            R_ID_OUT_OF_BOUNDS,
            R_IDENTICAL_POINTS,
            R_IDENTICAL_ELEMENTS,
            R_POINT_INDEX_ERROR,
        };

        enum class State {
            S_UNTOUCHED,
            S_CREATED,
            S_MODIFIED  
        };

        struct Statistics {
            std::size_t TotalPointCount;
            std::size_t TotalLineCount;
            double TotalLineLength;
            std::size_t TotalFaceCount;
            double TotalFaceArea;
            double BoundingBoxVolume;
        };

        Result LoadModel(std::string path);
        Result SaveModel(std::string path) const;
        const std::vector<std::shared_ptr<Line3D>>& GetLines() const;
        const std::vector<std::shared_ptr<Face3D>>& GetFaces() const;
        Result GetLinePointsById(std::size_t id, std::vector<std::shared_ptr<Point3D>>* pointsPtr) const;
        Result GetFacePointsById(std::size_t id, std::vector<std::shared_ptr<Point3D>>* pointsPtr) const;
        Result GetLineStateById(std::size_t id, State* statePtr) const;
        Result GetFaceStateById(std::size_t id, State* statePtr) const;
        Result RemoveLineById(std::size_t id);
        Result RemoveFaceById(std::size_t id);
        Result AddLine(double x1, double y1, double z1, double x2, double y2, double z2);
        Result AddFace(
            double x1, double y1, double z1, 
            double x2, double y2, double z2,
            double x3, double y3, double z3
        );
        Result ModifyLine(std::size_t id, int pointIndex, double x, double y, double z);
        Result ModifyFace(std::size_t id, int pointIndex, double x, double y, double z);
        Statistics GetStatistics() const;

    private:
        Model3D m_Model;
        std::vector<State> m_LineStates;
        std::vector<State> m_FaceStates;
};