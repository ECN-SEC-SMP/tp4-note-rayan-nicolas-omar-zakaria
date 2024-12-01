#pragma once

#include <string>
#include <vector>
#include "point2d.hpp"

namespace IO
{
    void parseFile(const std::string &filename);

    template <typename T>
    std::vector<Point2D<T>> parsePoints(const std::string &line);
}