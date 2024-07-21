#pragma once

#include "BaseConsoleView.hpp"
#include "../../Controllers/Controller.hpp"

class ConsoleView: public BaseConsoleView {
    public:
        using BaseConsoleView::BaseConsoleView;
        void Run(Controller& controller) const override;
    private:
        bool LoadModel(Controller& controller, std::string* path) const;
        void SaveModel(const Controller& controller, std::string defaultPath) const;
        void ShowHelp() const;
        void ShowStatistics(const Controller& controller) const;
};