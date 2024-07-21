#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../AbstractView.hpp"

class BaseConsoleView: public AbstractView {
    protected:
        enum class Brush {
            CLEAR = 0,
            FG_GRAY = 90,
            FG_RED,
            FG_GREEN,
            FG_YELLOW,
            FG_BLUE,
            FG_PURPLE,
            FG_CYAN,
            FG_WHITE,
            BG_GRAY = 40,
            BG_RED,
            BG_GREEN,
            BG_YELLOW,
            BG_BLUE,
            BG_PURPLE,
            BG_CYAN,
            BG_WHITE,
        };

        std::string Ask(std::string prompt, bool separateLines) const;
        std::string Ask(std::string prompt, bool separateLines, bool trim) const;
        std::size_t Choose(std::string prompt, const std::vector<std::string>& choices) const;
        
    public:
        BaseConsoleView(std::istream& input, std::ostream& output);
        BaseConsoleView(const BaseConsoleView& other) = delete;

        std::istream& Input;
        std::ostream& Output;

        friend std::ostream& operator<<(std::ostream& stream, const BaseConsoleView::Brush& brush);
};