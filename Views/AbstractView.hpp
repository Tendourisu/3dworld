#pragma once

#include "../Controllers/Controller.hpp"

class AbstractView {
    public:
        virtual void Run(Controller& controller) const = 0;
};