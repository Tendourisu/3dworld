#include <memory>
#include <string>
#include <vector>
#include "Controller.hpp"
#include "../Data/ObjExporter.hpp"
#include "../Data/ObjImporter.hpp"
#include "../Models/Model.hpp"

Controller::Result Controller::LoadModel(std::string path) {
    ObjImporter importer;
    try {
        importer.Import(path, m_Model);
    }
    catch (FileOpenException) {
        return Result::R_FILE_OPEN_ERROR;
    }
    catch (FileFormatException) {
        return Result::R_FILE_FORMAT_ERROR;
    }
    catch (IdenticalPointException) {
        return Result::R_IDENTICAL_POINTS;
    }
    m_LineStates.clear();
    for (int i = 0; i < m_Model.Lines.size(); i++) {
        m_LineStates.push_back(State::S_UNTOUCHED);
    }
    m_FaceStates.clear();
    for (int i = 0; i < m_Model.Faces.size(); i++) {
        m_FaceStates.push_back(State::S_UNTOUCHED);
    }
    return Result::R_OK;
}

Controller::Result Controller::SaveModel(std::string path) const {
    ObjExporter exporter;
    try {
        exporter.Export(path, m_Model);
    }
    catch (FileOpenException) {
        return Result::R_FILE_OPEN_ERROR;
    }
    return Result::R_OK;
}

const std::vector<std::shared_ptr<Line3D>>& Controller::GetLines() const {
    return m_Model.Lines;
}

const std::vector<std::shared_ptr<Face3D>>& Controller::GetFaces() const {
    return m_Model.Faces;
}

Controller::Result Controller::GetLinePointsById(std::size_t id, std::vector<std::shared_ptr<Point3D>>* pointsPtr) const {
    if (id == 0 || id > m_Model.Lines.size()) {
        return Result::R_ID_OUT_OF_BOUNDS;
    }
    *pointsPtr = m_Model.Lines[id-1]->GetPointsVector();
    return Result::R_OK;
}

Controller::Result Controller::GetFacePointsById(std::size_t id, std::vector<std::shared_ptr<Point3D>>* pointsPtr) const {
    if (id == 0 || id > m_Model.Faces.size()) {
        return Result::R_ID_OUT_OF_BOUNDS;
    }
    *pointsPtr = m_Model.Faces[id-1]->GetPointsVector();
    return Result::R_OK;
}

Controller::Result Controller::GetLineStateById(std::size_t id, State* statePtr) const {
    if (id == 0 || id > m_LineStates.size()) {
        return Result::R_ID_OUT_OF_BOUNDS;
    }
    *statePtr = m_LineStates[id - 1];
    return Result::R_OK;
}

Controller::Result Controller::GetFaceStateById(std::size_t id, State* statePtr) const {
    if (id == 0 || id > m_FaceStates.size()) {
        return Result::R_ID_OUT_OF_BOUNDS;
    }
    *statePtr = m_FaceStates[id - 1];
    return Result::R_OK;
}

Controller::Result Controller::RemoveLineById(std::size_t id) {
    if (m_Model.RemoveLine(id - 1)) {
        m_LineStates.erase(m_LineStates.begin() + id - 1);
        return Result::R_OK;
    }
    else {
        return Result::R_ID_OUT_OF_BOUNDS;
    }
}

Controller::Result Controller::RemoveFaceById(std::size_t id) {
    if (m_Model.RemoveFace(id - 1)) {
        m_FaceStates.erase(m_FaceStates.begin() + id - 1);
        return Result::R_OK;
    }
    else {
        return Result::R_ID_OUT_OF_BOUNDS;
    }
}

Controller::Result Controller::AddLine(double x1, double y1, double z1, double x2, double y2, double z2) {
    double startCoords[] = { x1, y1, z1 };
    double endCoords[] = { x2, y2, z2 };
    BasePoint3D start(startCoords);
    BasePoint3D end(endCoords);
    try {
        m_Model.AddLine(start, end);
    }
    catch (IdenticalPointException) {
        return Result::R_IDENTICAL_POINTS;
    }
    catch (IdenticalElementException) {
        return Result::R_IDENTICAL_ELEMENTS;
    }
    m_LineStates.push_back(State::S_CREATED);
    return Result::R_OK;
}

Controller::Result Controller::AddFace(
    double x1, double y1, double z1, 
    double x2, double y2, double z2,
    double x3, double y3, double z3
) {
    double firstCoords[] = { x1, y1, z1 };
    double secondCoords[] = { x2, y2, z2 };
    double thirdCoords[] = { x3, y3, z3 };
    BasePoint3D first(firstCoords);
    BasePoint3D second(secondCoords);
    BasePoint3D third(thirdCoords);
    try {
        m_Model.AddFace(first, second, third);
    }
    catch (IdenticalPointException) {
        return Result::R_IDENTICAL_POINTS;
    }
    catch (IdenticalElementException) {
        return Result::R_IDENTICAL_ELEMENTS;
    }
    m_FaceStates.push_back(State::S_CREATED);
    return Result::R_OK;
}

Controller::Result Controller::ModifyLine(std::size_t id, int pointIndex, double x, double y, double z) {
    // check here for invalid access
    // as we cannot distinguish which index overflows
    if (id == 0 || id > m_Model.Lines.size()) {
        return Result::R_ID_OUT_OF_BOUNDS;
    }
    double coords[] = { x, y, z };
    try {
        m_Model.ModifyLine(id - 1, pointIndex - 1, BasePoint3D(coords));
    }
    catch (IndexOutOfBoundsException) {
        return Result::R_POINT_INDEX_ERROR;
    }
    m_LineStates[id - 1] = State::S_MODIFIED;
    return Result::R_OK;
}

Controller::Result Controller::ModifyFace(std::size_t id, int pointIndex, double x, double y, double z) {
    // same as Controller::ModifyLine
    if (id == 0 || id > m_Model.Faces.size()) {
        return Result::R_ID_OUT_OF_BOUNDS;
    }
    double coords[] = { x, y, z };
    try {
        m_Model.ModifyFace(id - 1, pointIndex - 1, BasePoint3D(coords));
    }
    catch (IndexOutOfBoundsException) {
        return Result::R_POINT_INDEX_ERROR;
    }
    m_FaceStates[id - 1] = State::S_MODIFIED;
    return Result::R_OK;
}

Controller::Statistics Controller::GetStatistics() const {
    Statistics stats {
        .TotalPointCount = m_Model.Lines.size() * 2 + m_Model.Faces.size() * 3,
        .TotalLineCount = m_Model.Lines.size(),
        .TotalLineLength = 0,
        .TotalFaceCount = m_Model.Faces.size(),
        .TotalFaceArea = 0,
        .BoundingBoxVolume = m_Model.BoundingBox().GetVolume()
    };
    for (auto line: m_Model.Lines) {
        stats.TotalLineLength += line->GetLength();
    }
    for (auto face: m_Model.Faces) {
        stats.TotalFaceArea += face->GetArea();
    }
    return stats;
}