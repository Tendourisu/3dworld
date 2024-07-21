#include <iostream>
#include <sstream>
#include "LinesConsoleView.hpp"
#include "../../Controllers/Controller.hpp"

void LinesConsoleView::Run(Controller& controller) const {
    Output << Brush::FG_PURPLE << "Switched to Line3D mode." << Brush::CLEAR << std::endl;
    Output << Brush::FG_GRAY << "Type ? for help." << Brush::CLEAR << std::endl;
    while (true) {
        auto command = Ask("lines#> ", false, true);
        if (command == "?") {
            ShowHelp();
        }
        else if (command == "quit") {
            break;
        }
        else if (command == "list") {
            ListLines(controller);
        }
        else if (command == "pts") {
            ListPoints(controller);
        }
        else if (command == "add") {
            AddLine(controller);
        }
        else if (command == "del") {
            RemoveLine(controller);
        }
        else if (command == "edit") {
            ModifyLine(controller);
        }
        else {
            Output << Brush::FG_RED << "Unrecognized command '" << command << "'." << std::endl;
        }
    }
    Output << Brush::FG_PURPLE << "Exited Line3D mode." << Brush::CLEAR << std::endl;
}

void LinesConsoleView::ShowHelp() const {
    Output << Brush::FG_PURPLE << "Available commands:" << Brush::CLEAR << std::endl;
    Output << Brush::FG_PURPLE << "  ?"     << Brush::CLEAR << "\t";
    Output << "Show this message." << std::endl;
    Output << Brush::FG_PURPLE << "  quit"  << Brush::CLEAR << "\t";
    Output << "Exit Line3D mode." << std::endl;
    Output << Brush::FG_PURPLE << "  list"  << Brush::CLEAR << "\t";
    Output << "List all lines in model." << std::endl;
    Output << Brush::FG_PURPLE << "  pts"  << Brush::CLEAR << "\t";
    Output << "List all points in a line." << std::endl;
    Output << Brush::FG_PURPLE << "  add"  << Brush::CLEAR << "\t";
    Output << "Add a line to model." << std::endl;
    Output << Brush::FG_PURPLE << "  del"  << Brush::CLEAR << "\t";
    Output << "Remove a line from model." << std::endl;
    Output << Brush::FG_PURPLE << "  edit"  << Brush::CLEAR << "\t";
    Output << "Modify a line in model." << std::endl;
}

void LinesConsoleView::ListLines(const Controller& controller) const {
    auto lines = controller.GetLines();
    Output << Brush::FG_PURPLE << "Lines in model:" << Brush::CLEAR << std::endl;
    for (int i = 0; i < lines.size(); i++) {
        Controller::State state;
        controller.GetLineStateById(i + 1, &state); // should not fail.
        Output << "  ";
        if (state == Controller::State::S_CREATED) {
            Output << Brush::BG_CYAN;
        }
        else if (state == Controller::State::S_MODIFIED) {
            Output << Brush::BG_YELLOW;
        }
        Output << "#" << (i + 1) << " ";
        Output << *lines[i];
        if (state == Controller::State::S_CREATED) {
            Output << " (created)" << Brush::CLEAR;
        }
        else if (state == Controller::State::S_MODIFIED) {
            Output << " (modified)" << Brush::CLEAR;
        }
        Output << "\n    length = " << lines[i]->GetLength() << std::endl;
    }
}

void LinesConsoleView::ListPoints(const Controller& controller) const {
    std::vector<std::string> choices;
    for (auto line: controller.GetLines()) {
        choices.push_back(line->ToString());
    }
    auto id = Choose("Select a line to inspect:", choices);
    std::vector<std::shared_ptr<Point3D>> points;
    auto result = controller.GetLinePointsById(id, &points);
    if (result == Controller::Result::R_ID_OUT_OF_BOUNDS) {
        Output << Brush::FG_RED << "error: #" << id << " is not a valid line id." << Brush::CLEAR << std::endl;
    }
    else {
        Output << Brush::FG_PURPLE << "Points in line #" << id << ":" << Brush::CLEAR << std::endl;
        for (auto point: points) {
            Output << "  " << point->ToString(true) << std::endl;
        }
    }
}

void LinesConsoleView::AddLine(Controller& controller) const {
    Output << Brush::FG_GRAY << "Existing points will be reused if possible." << Brush::CLEAR << std::endl;
    std::istringstream startCoords(Ask("Starting point (x y z): ", false, false));
    std::istringstream endCoords(Ask("Ending point (x y z): ", false, false));
    double x1;
    double y1;
    double z1;
    double x2;
    double y2;
    double z2;
    startCoords >> x1 >> y1 >> z1;
    endCoords >> x2 >> y2 >> z2;
    if (startCoords.bad() || endCoords.bad()) {
        Output << Brush::FG_RED << "error: Invalid coordinates." << Brush::CLEAR << std::endl;
        return;
    }
    auto result = controller.AddLine(x1, y1, z1, x2, y2, z2);
    if (result == Controller::Result::R_IDENTICAL_POINTS) {
        Output << Brush::FG_RED << "error: Identical points within element." << Brush::CLEAR << std::endl;
    }
    else if (result == Controller::Result::R_IDENTICAL_ELEMENTS) {
        Output << Brush::FG_RED << "error: Identical elements within model." << Brush::CLEAR << std::endl;
    }
    else {
        Output << Brush::FG_GREEN << "Successfully created line." << Brush::CLEAR << std::endl;
    }
}

void LinesConsoleView::RemoveLine(Controller& controller) const {
    std::vector<std::string> choices;
    for (auto line: controller.GetLines()) {
        choices.push_back(line->ToString());
    }
    auto id = Choose("Select a line to remove:", choices);
    auto result = controller.RemoveLineById(id);
    if (result == Controller::Result::R_ID_OUT_OF_BOUNDS) {
        Output << Brush::FG_RED << "error: #" << id << " is not a valid line id." << Brush::CLEAR << std::endl;
    }
    else {
        Output << Brush::FG_GREEN << "Successfully removed line #" << id << "." << Brush::CLEAR << std::endl;
    }
}

void LinesConsoleView::ModifyLine(Controller& controller) const {
    std::vector<std::string> choices;
    for (auto line: controller.GetLines()) {
        choices.push_back(line->ToString());
    }
    auto id = Choose("Select a line to modify:", choices);
    std::istringstream pointIndexLine(Ask("Point index [1~2]: ", false));
    int pointIndex;
    pointIndexLine >> pointIndex;
    if (pointIndexLine.bad()) {
        Output << Brush::FG_RED << "error: Invalid point index." << Brush::CLEAR << std::endl;
        return;
    }
    std::istringstream coords(Ask("Coordinates (x y z): ", false));
    double x;
    double y;
    double z;
    coords >> x >> y >> z;
    if (coords.bad()) {
        Output << Brush::FG_RED << "error: Invalid coordinates." << Brush::CLEAR << std::endl;
        return;
    }
    auto result = controller.ModifyLine(id, pointIndex, x, y, z);
    if (result == Controller::Result::R_ID_OUT_OF_BOUNDS) {
        Output << Brush::FG_RED << "error: #" << id << " is not a valid line id." << Brush::CLEAR << std::endl;
    }
    else if (result == Controller::Result::R_POINT_INDEX_ERROR) {
        Output << Brush::FG_RED << "error: Invalid point index '" << pointIndex << "'." << Brush::CLEAR << std::endl;
    }
    else {
        Output << Brush::FG_GREEN << "Successfully modified line #" << id << "." << Brush::CLEAR << std::endl;
    }
}