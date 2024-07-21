#include <filesystem>
#include <iostream>
#include "ConsoleView.hpp"
#include "FacesConsoleView.hpp"
#include "LinesConsoleView.hpp"
#include "../../Controllers/Controller.hpp"

void ConsoleView::Run(Controller& controller) const {
    Output << Brush::BG_PURPLE << "                           " << Brush::CLEAR << "\n";
    Output << Brush::BG_PURPLE << "  Construct 3D World v1.0  " << Brush::CLEAR << "\n";
    Output << Brush::BG_PURPLE << "                           " << Brush::CLEAR << "\n";
    Output << std::endl;
    std::string fileName;
    if (!LoadModel(controller, &fileName)) {
        return;
    }
    Output << Brush::FG_GRAY << "Type ? for help." << Brush::CLEAR << std::endl;
    while (true) {
        auto command = Ask("#> ", false, true);
        if (command == "?") {
            ShowHelp();
        }
        else if (command == "quit") {
            break;
        }
        else if (command == "lines") {
            LinesConsoleView(Input, Output).Run(controller);
        }
        else if (command == "faces") {
            FacesConsoleView(Input, Output).Run(controller);
        }
        else if (command == "stat") {
            ShowStatistics(controller);
        }
        else if (command == "save") {
            SaveModel(controller, fileName);
        }
        else {
            Output << Brush::FG_RED << "Unrecognized command '" << command << "'." << std::endl;
        }
    }
}

bool ConsoleView::LoadModel(Controller& controller, std::string* path) const {
    auto fileName = Ask("Enter 3D model file: ", false, true);
    auto result = controller.LoadModel(fileName);
    if (result == Controller::Result::R_FILE_EXTENSION_ERROR) {
        Output << Brush::FG_RED << "error: Invalid file extension." << Brush::CLEAR << std::endl;
        return false;
    }
    else if (result == Controller::Result::R_FILE_OPEN_ERROR) {
        Output << Brush::FG_RED << "error: Cannot open file '";
        Output << std::filesystem::path(fileName).filename().c_str();
        Output << "'." << Brush::CLEAR << std::endl;
        return false;
    }
    else if (result == Controller::Result::R_FILE_FORMAT_ERROR) {
        Output << Brush::FG_RED << "error: File '";
        Output << std::filesystem::path(fileName).filename().c_str();
        Output << "' has invalid format." << Brush::CLEAR << std::endl;
        return false;
    }
    else if (result == Controller::Result::R_IDENTICAL_POINTS) {
        Output << Brush::FG_RED << "error: File '";
        Output << std::filesystem::path(fileName).filename().c_str();
        Output << "' contains invalid elements." << Brush::CLEAR << std::endl;
        return false;
    }
    Output << Brush::FG_GREEN << "Successfully loaded '";
    Output << std::filesystem::path(fileName).filename().c_str();
    Output << "'." << Brush::CLEAR << std::endl;
    *path = fileName;
    return true;
}

void ConsoleView::SaveModel(const Controller& controller, std::string defaultPath) const {
    Output << Brush::FG_GRAY << "(Enter nothing to use default value '" << defaultPath << "')" << Brush::CLEAR << std::endl;
    std::string fileName = Ask("Save to: ", false, true);
    if (fileName.empty()) {
        fileName = defaultPath;
    }
    auto result = controller.SaveModel(fileName);
    if (result == Controller::Result::R_FILE_EXTENSION_ERROR) {
        Output << Brush::FG_RED << "error: Invalid file extension." << Brush::CLEAR << std::endl;
        return;
    }
    else if (result == Controller::Result::R_FILE_OPEN_ERROR) {
        Output << Brush::FG_RED << "error: Cannot open file '";
        Output << std::filesystem::path(fileName).filename().c_str();
        Output << "'." << Brush::CLEAR << std::endl;
        return;
    }
    Output << Brush::FG_GREEN << "Successfully saved to '";
    Output << std::filesystem::path(fileName).filename().c_str();
    Output << "'." << Brush::CLEAR << std::endl;
}

void ConsoleView::ShowHelp() const {
    Output << Brush::FG_PURPLE << "Available commands:" << Brush::CLEAR << std::endl;
    Output << Brush::FG_PURPLE << "  ?"     << Brush::CLEAR << "\t";
    Output << "Show this message." << std::endl;
    Output << Brush::FG_PURPLE << "  quit"  << Brush::CLEAR << "\t";
    Output << "Exit this program." << std::endl;
    Output << Brush::FG_PURPLE << "  lines"  << Brush::CLEAR << "\t";
    Output << "Enter Line3D view & edit mode." << std::endl;
    Output << Brush::FG_PURPLE << "  faces"  << Brush::CLEAR << "\t";
    Output << "Enter Face3D view & edit mode." << std::endl;
    Output << Brush::FG_PURPLE << "  stat"  << Brush::CLEAR << "\t";
    Output << "Display statistics." << std::endl;
    Output << Brush::FG_PURPLE << "  save"  << Brush::CLEAR << "\t";
    Output << "Save this model to disk." << std::endl;
}

void ConsoleView::ShowStatistics(const Controller& controller) const {
    auto stat = controller.GetStatistics();
    Output << Brush::FG_PURPLE << "Statistics:" << Brush::CLEAR << std::endl;
    Output << Brush::FG_PURPLE << "  Total Point Count:" << Brush::CLEAR << "\t";
    Output << stat.TotalPointCount << std::endl;
    Output << Brush::FG_PURPLE << "  Total Line Count:" << Brush::CLEAR << "\t";
    Output << stat.TotalLineCount << std::endl;
    Output << Brush::FG_PURPLE << "  Total Line Length:" << Brush::CLEAR << "\t";
    Output << stat.TotalLineLength << std::endl;
    Output << Brush::FG_PURPLE << "  Total Face Count:" << Brush::CLEAR << "\t";
    Output << stat.TotalFaceCount << std::endl;
    Output << Brush::FG_PURPLE << "  Total Face Area:" << Brush::CLEAR << "\t";
    Output << stat.TotalFaceArea << std::endl;
    Output << Brush::FG_PURPLE << "  Bounding Box Volume:" << Brush::CLEAR << "\t";
    Output << stat.BoundingBoxVolume << std::endl;
}