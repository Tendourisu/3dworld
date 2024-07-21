#pragma once

#include "BaseConsoleView.hpp"
#include "../../Controllers/Controller.hpp"

class LinesConsoleView: public BaseConsoleView {
    public:
        using BaseConsoleView::BaseConsoleView;
        void Run(Controller& controller) const override;
    private:
        void ShowHelp() const;
        void ListLines(const Controller& controller) const;
        void ListPoints(const Controller& controller) const;
        void AddLine(Controller& controller) const;
        void RemoveLine(Controller& controller) const;
        void ModifyLine(Controller& controller) const;
};