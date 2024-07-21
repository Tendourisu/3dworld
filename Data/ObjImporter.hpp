#pragma once

#include <string>
#include "AbstractImporter.hpp"
#include "../Models/Model.hpp"

class ObjImporter: public AbstractImporter {
    public:
        void Import(std::string path, Model3D& model) const override;
};