#include <iostream>

#include "point2d.hpp"

int main()
{
    Point2D p(1, 2);

    std::cout << "P = " << p << std::endl;

    p.translate(3, -2);
    std::cout << "Translated by (3, -2) = " << p << std::endl;

    p.setY(10);
    std::cout << "Forced y to 10, reading y = " << p.getY() << std::endl;
}