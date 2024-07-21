#include <iostream>
#include <sstream>
#include "FacesConsoleView.hpp"
#include "../../Controllers/Controller.hpp"

void FacesConsoleView::Run(Controller& controller) const {
    Output << Brush::FG_PURPLE << "Switched to Face3D mode." << Brush::CLEAR << std::endl;
    Output << Brush::FG_GRAY << "Type ? for help." << Brush::CLEAR << std::endl;
    while (true) {
        auto command = Ask("faces#> ", false, true);
        if (command == "?") {
            ShowHelp();
        }
        else if (command == "quit") {
            break;
        }
        else if (command == "list") {
            ListFaces(controller);
        }
        else if (command == "pts") {
            ListPoints(controller);
        }
        else if (command == "add") {
            AddFace(controller);
        }
        else if (command == "del") {
            RemoveFace(controller);
        }
        else if (command == "edit") {
            ModifyFace(controller);
        }
        else {
            Output << Brush::FG_RED << "Unrecognized command '" << command << "'." << std::endl;
        }
    }
    Output << Brush::FG_PURPLE << "Exited Face3D mode." << Brush::CLEAR << std::endl;
}

void FacesConsoleView::ShowHelp() const {
    Output << Brush::FG_PURPLE << "Available commands:" << Brush::CLEAR << std::endl;
    Output << Brush::FG_PURPLE << "  ?"     << Brush::CLEAR << "\t";
    Output << "Show this message." << std::endl;
    Output << Brush::FG_PURPLE << "  quit"  << Brush::CLEAR << "\t";
    Output << "Exit Face3D mode." << std::endl;
    Output << Brush::FG_PURPLE << "  list"  << Brush::CLEAR << "\t";
    Output << "List all faces in model." << std::endl;
    Output << Brush::FG_PURPLE << "  pts"  << Brush::CLEAR << "\t";
    Output << "List all points in a face." << std::endl;
    Output << Brush::FG_PURPLE << "  add"  << Brush::CLEAR << "\t";
    Output << "Add a face to model." << std::endl;
    Output << Brush::FG_PURPLE << "  del"  << Brush::CLEAR << "\t";
    Output << "Remove a face from model." << std::endl;
    Output << Brush::FG_PURPLE << "  edit"  << Brush::CLEAR << "\t";
    Output << "Modify a face in model." << std::endl;
}

void FacesConsoleView::ListFaces(const Controller& controller) const {
    auto faces = controller.GetFaces();
    Output << Brush::FG_PURPLE << "Faces in model:" << Brush::CLEAR << std::endl;
    for (int i = 0; i < faces.size(); i++) {
        Controller::State state;
        controller.GetFaceStateById(i + 1, &state); // should not fail.
        Output << "  ";
        if (state == Controller::State::S_CREATED) {
            Output << Brush::BG_CYAN;
        }
        else if (state == Controller::State::S_MODIFIED) {
            Output << Brush::BG_YELLOW;
        }
        Output << "#" << (i + 1) << " ";
        Output << *faces[i];
        if (state == Controller::State::S_CREATED) {
            Output << " (created)" << Brush::CLEAR;
        }
        else if (state == Controller::State::S_MODIFIED) {
            Output << " (modified)" << Brush::CLEAR;
        }
        Output << "\n    area = " << faces[i]->GetArea() << std::endl;
    }
}

void FacesConsoleView::ListPoints(const Controller& controller) const {
    std::vector<std::string> choices;
    for (auto face: controller.GetFaces()) {
        choices.push_back(face->ToString());
    }
    auto id = Choose("Select a face to inspect:", choices);
    std::vector<std::shared_ptr<Point3D>> points;
    auto result = controller.GetFacePointsById(id, &points);
    if (result == Controller::Result::R_ID_OUT_OF_BOUNDS) {
        Output << Brush::FG_RED << "error: #" << id << " is not a valid face id." << Brush::CLEAR << std::endl;
    }
    else {
        Output << Brush::FG_PURPLE << "Points in face #" << id << ":" << Brush::CLEAR << std::endl;
        for (auto point: points) {
            Output << "  " << point->ToString(true) << std::endl;
        }
    }
}

void FacesConsoleView::AddFace(Controller& controller) const {
    Output << Brush::FG_GRAY << "Existing points will be reused if possible." << Brush::CLEAR << std::endl;
    std::istringstream firstCoords(Ask("First point (x y z): ", false, false));
    std::istringstream secondCoords(Ask("Second point (x y z): ", false, false));
    std::istringstream thirdCoords(Ask("Third point (x y z): ", false, false));
    double x1;
    double y1;
    double z1;
    double x2;
    double y2;
    double z2;
    double x3;
    double y3;
    double z3;
    firstCoords >> x1 >> y1 >> z1;
    secondCoords >> x2 >> y2 >> z2;
    thirdCoords >> x3 >> y3 >> z3;
    if (firstCoords.bad() || secondCoords.bad() || thirdCoords.bad()) {
        Output << Brush::FG_RED << "error: Invalid coordinates." << Brush::CLEAR << std::endl;
        return;
    }
    auto result = controller.AddFace(x1, y1, z1, x2, y2, z2, x3, y3, z3);
    if (result == Controller::Result::R_IDENTICAL_POINTS) {
        Output << Brush::FG_RED << "error: Identical points within element." << Brush::CLEAR << std::endl;
    }
    else if (result == Controller::Result::R_IDENTICAL_ELEMENTS) {
        Output << Brush::FG_RED << "error: Identical elements within model." << Brush::CLEAR << std::endl;
    }
    else {
        Output << Brush::FG_GREEN << "Successfully created face." << Brush::CLEAR << std::endl;
    }
}

void FacesConsoleView::RemoveFace(Controller& controller) const {
    std::vector<std::string> choices;
    for (auto face: controller.GetFaces()) {
        choices.push_back(face->ToString());
    }
    auto id = Choose("Select a face to remove:", choices);
    auto result = controller.RemoveFaceById(id);
    if (result == Controller::Result::R_ID_OUT_OF_BOUNDS) {
        Output << Brush::FG_RED << "error: #" << id << " is not a valid face id." << Brush::CLEAR << std::endl;
    }
    else {
        Output << Brush::FG_GREEN << "Successfully removed face #" << id << "." << Brush::CLEAR << std::endl;
    }
}

void FacesConsoleView::ModifyFace(Controller& controller) const {
    std::vector<std::string> choices;
    for (auto face: controller.GetFaces()) {
        choices.push_back(face->ToString());
    }
    auto id = Choose("Select a face to modify:", choices);
    std::istringstream pointIndexFace(Ask("Point index [1~3]: ", false));
    int pointIndex;
    pointIndexFace >> pointIndex;
    if (pointIndexFace.bad()) {
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
    auto result = controller.ModifyFace(id, pointIndex, x, y, z);
    if (result == Controller::Result::R_ID_OUT_OF_BOUNDS) {
        Output << Brush::FG_RED << "error: #" << id << " is not a valid face id." << Brush::CLEAR << std::endl;
    }
    else if (result == Controller::Result::R_POINT_INDEX_ERROR) {
        Output << Brush::FG_RED << "error: Invalid point index '" << pointIndex << "'." << Brush::CLEAR << std::endl;
    }
    else {
        Output << Brush::FG_GREEN << "Successfully modified face #" << id << "." << Brush::CLEAR << std::endl;
    }
}