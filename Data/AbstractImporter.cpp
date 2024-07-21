#include <string>
#include <fstream>
#include "AbstractImporter.hpp"
#include "../Errors.hpp"

void AbstractImporter::Import(std::string path, Model3D& model) const {
    if (!CheckExtension(path)) {
        throw FileExtensionException();
    }
    std::ifstream file;
    file.open(path, std::ios::in);
    if (!file.is_open()) {
        throw FileOpenException();
    }
    Load(file, model);
    file.close();
}