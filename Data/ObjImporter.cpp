#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "ObjImporter.hpp"
#include "../Errors.hpp"
#include "../Models/Model.hpp"

void ObjImporter::Import(std::string path, Model3D& model) const {
    std::ifstream file;
    file.open(path, std::ios::in);
    if (!file.is_open()) {
        throw FileOpenException();
    }
    std::vector<std::shared_ptr<Point3D>> points;
    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        if (line.length() == 0) continue;
        std::istringstream stream(line);
        char kind;
        stream >> kind;
        switch (kind) {
            case '#': {
                continue;
            }
            case 'g': {
                stream.get();
                std::string name;
                std::getline(stream, name);
                model.Name = name;
                break;
            }
            case 'v': {
                double coords[3] = {0};
                stream >> coords[0] >> coords[1] >> coords[2];
                points.push_back(std::shared_ptr<Point3D>(new Point3D(points.size() + 1, coords)));
                break;
            }
            case 'l': {
                int indices[2] = {0};
                stream >> indices[0] >> indices[1];
                model.AddLineUnchecked(Line3D(points[indices[0]-1], points[indices[1]-1]));
                break;
            }
            case 'f': {
                int indices[3] = {0};
                stream >> indices[0] >> indices[1] >> indices[2];
                model.AddFaceUnchecked(Face3D(points[indices[0]-1], points[indices[1]-1], points[indices[2]-1]));
                break;
            }
            default: {
                throw FileFormatException();
            }
        }
    }
    file.close();
}