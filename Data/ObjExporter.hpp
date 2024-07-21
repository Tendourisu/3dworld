#pragma once

#include <string>
#include "AbstractExporter.hpp"
#include "../Models/Model.hpp"

class ObjExporter: public AbstractExporter {
    protected:
        bool CheckExtension(std::string path) const override;
        void Save(std::ofstream& file, const Model3D& model) const override;
};