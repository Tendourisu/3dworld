#include <iostream>
#include <sstream>
#include <string>
#include "BaseConsoleView.hpp"

std::ostream& operator<<(std::ostream& stream, const BaseConsoleView::Brush& brush) {
    stream << "\033[";
    stream << static_cast<int>(brush);
    stream << "m";
    return stream;
}

BaseConsoleView::BaseConsoleView(std::istream& input, std::ostream& output): Input(input), Output(output) {}

std::string BaseConsoleView::Ask(std::string prompt, bool separateLines) const {
    return Ask(prompt, separateLines, false);
}

std::string BaseConsoleView::Ask(std::string prompt, bool separateLines, bool trim) const {
    Output << Brush::FG_BLUE << prompt << Brush::CLEAR;
    if (separateLines) {
        Output << std::endl;
    }
    std::string answer;
    std::getline(Input, answer);
    if (trim) {
        size_t start = answer.find_first_not_of(" \t\n\r");
        size_t end = answer.find_last_not_of(" \t\n\r");
        if (start == std::string::npos || end == std::string::npos) {
            return "";
        }
        return answer.substr(start, end - start + 1);
    }
    return answer;
}

std::size_t BaseConsoleView::Choose(std::string prompt, const std::vector<std::string>& choices) const {
    Output << Brush::FG_BLUE << prompt << Brush::CLEAR << std::endl;
    for (int i = 0; i < choices.size(); i++) {
        Output << (i + 1) << ". " << choices[i] << std::endl;
    }
    Output << Brush::FG_BLUE;
    if (choices.size() > 1) {
        Output << "[1~" << choices.size() << "]: " << Brush::CLEAR;
    }
    else {
        Output << "[1]: " << Brush::CLEAR;
    }
    std::istringstream line(Ask("", false));
    std::size_t index;
    line >> index;
    return index;
}