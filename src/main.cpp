#include <iostream>

#include "parcelle.hpp"
#include "constructible.hpp"

int main()
{
    std::vector summits = {
        Point2D(1, 2),
        Point2D(2, 4),
        Point2D(4, 1)};

    Polygon poly(summits);

    Parcelle parc(1, std::string("City"), poly);
}