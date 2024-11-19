#pragma once

#include <iostream>

template <typename T>
class Point2D
{
public:
    Point2D(T x, T y) : _x(x), _y(y) {}
    Point2D() : _x(0), _y(0) {}
    Point2D(const Point2D<T> &original) : _x(original._x), _y(original._y) {}

    T getX() { return _x; }
    T getY() { return _y; }

    void setX(T x) { _x = x; }
    void setY(T y) { _y = y; }

    void translate(T x, T y)
    {
        _x += x;
        _y += y;
    }

    friend std::ostream &operator<<(std::ostream &os, const Point2D &p)
    {
        os << '(' << p._x << ", " << p._y << ')';
        return os;
    }

private:
    T _x, _y;
};