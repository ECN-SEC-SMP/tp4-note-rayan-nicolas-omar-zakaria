#include <iostream>

#include "polygon.hpp"

int main()
{
    std::vector summits = {Point2D(0, 0), Point2D(5, 5)};

    Polygon<int> p1(summits);
    auto p2 = p1;

    std::cout << "P1 = " << p1 << ", P2 = " << p2 << std::endl;

    p1.translate(100, 100);
    std::cout << "After translation: P1 = " << p1 << ", P2 = " << p2 << std::endl;
}