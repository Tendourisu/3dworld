#include <fstream>
#include <filesystem>
#include <string>
#include "ObjExporter.hpp"
#include "../Models/Model.hpp"

void ObjExporter::Export(std::string path, const Model3D& model) const {
    std::ofstream file;
    file.open(path, std::ios::trunc);
    if (!file.is_open()) {
        throw FileOpenException();
    }
    file << "# " << std::filesystem::path(path).filename().c_str() << std::endl;
    file << "g " << model.Name << std::endl;
    for (auto point: model.CollectPoints()) {
        file << "v ";
        for (int i = 0; i < 3; i++) {
            file << " " << point->GetCoordinate(i); 
        }
        file << std::endl;
    }
    for (auto line: model.Lines) {
        file << "l  ";
        file << line->Start->Id << " ";
        file << line->End->Id << std::endl;
    }
    for (auto face: model.Faces) {
        file << "f  ";
        file << face->First->Id << " ";
        file << face->Second->Id << " ";
        file << face->Third->Id << std::endl;
    }
    file.close();
}