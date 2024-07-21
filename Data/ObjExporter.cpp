#include <fstream>
#include <filesystem>
#include <string>
#include "ObjExporter.hpp"
#include "../Models/Model.hpp"

bool ObjExporter::CheckExtension(std::string path) const {
    return std::filesystem::path(path).extension() == ".obj";
}

void ObjExporter::Save(std::ofstream& file, const Model3D& model) const {
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