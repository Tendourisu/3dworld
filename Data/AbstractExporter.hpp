#pragma once

#include <string>
#include "../Models/Model.hpp"

using Line3D = Line<3>;
using Face3D = Face<3>;
using Model3D = Model<3>;
using Point3D = Point<3>;
using BasePoint3D = BasePoint<3>;

class AbstractExporter {
    public:
        virtual void Export(std::string path, const Model3D& model) const = 0;
};