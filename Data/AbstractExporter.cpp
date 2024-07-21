#include <fstream>
#include <string>
#include "AbstractExporter.hpp"
#include "../Errors.hpp"

void AbstractExporter::Export(std::string path, const Model3D& model) const {
    if (!CheckExtension(path)) {
        throw FileExtensionException();
    }
    std::ofstream file;
    file.open(path, std::ios::trunc);
    if (!file.is_open()) {
        throw FileOpenException();
    }
    Save(file, model);
    file.close();
}