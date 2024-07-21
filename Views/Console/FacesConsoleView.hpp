#pragma once

#include "BaseConsoleView.hpp"
#include "../../Controllers/Controller.hpp"

class FacesConsoleView: public BaseConsoleView {
    public:
        using BaseConsoleView::BaseConsoleView;
        void Run(Controller& controller) const override;
    private:
        void ShowHelp() const;
        void ListFaces(const Controller& controller) const;
        void ListPoints(const Controller& controller) const;
        void AddFace(Controller& controller) const;
        void RemoveFace(Controller& controller) const;
        void ModifyFace(Controller& controller) const;
};