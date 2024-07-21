#pragma once

#include <stdexcept>

class IndexOutOfBoundsException: public std::invalid_argument {
    public:
        IndexOutOfBoundsException(std::size_t index): invalid_argument("Index out of bounds."), m_Index(index) {}
        const std::size_t& Index { m_Index };
    private:
        std::size_t m_Index;
};

class DowngradeException: public std::invalid_argument {
    public:
        DowngradeException(): invalid_argument("Cannot downgrade point dimension.") {}
};

class FileOpenException: public std::invalid_argument {
    public:
        FileOpenException(): invalid_argument("Cannot open specific file.") {}
};

class FileFormatException: public std::invalid_argument {
    public:
        FileFormatException(): invalid_argument("File contains invalid format.") {}
};

class IdenticalPointException: public std::invalid_argument {
    public: 
        IdenticalPointException(): invalid_argument("Two of provided points are the same.") {}
};

class IdenticalElementException: public std::invalid_argument {
    public:
        IdenticalElementException(): invalid_argument("Element coincides with existing ones.") {}
};