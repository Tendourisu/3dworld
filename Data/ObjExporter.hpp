#pragma once

#include <string>
#include "AbstractExporter.hpp"
#include "../Models/Model.hpp"

class ObjExporter: public AbstractExporter {
    public:
        void Export(std::string path, const Model3D& model) const override;
};