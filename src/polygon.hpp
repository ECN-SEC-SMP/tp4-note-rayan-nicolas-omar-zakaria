#pragma once
#include "point2d.hpp"

#include <vector>

template <typename T>
class Polygon
{
public:
    Polygon() {}
    Polygon(const Polygon<T> &other) : _summits(other._summits), _surface(other._surface) {}

    Polygon(const std::vector<Point2D<T>> &summits) : _summits(summits)
    {
        _recalculateSurface();
    }

    const std::vector<Point2D<T>> &getSummits() { return _summits; };

    void setSummits(const std::vector<Point2D<T>> &summits)
    {
        _summits = summits;
        _recalculateSurface();
    }

    void addSummit(const Point2D<T> &p)
    {
        _summits.push_back(p);
        _recalculateSurface();
    }

    void translate(T x, T y)
    {
        for (auto &p : _summits)
            p.translate(x, y);
    }

    friend std::ostream &operator<<(std::ostream &os, const Polygon &p)
    {
        os << "[ ";

        for (const auto &point : p._summits)
            os << point << ' ';

        os << ']';

        return os;
    }

private:
    void _recalculateSurface()
    {
        double s = 0;

        for (size_t i = 0; i < _summits.size() - 1; ++i)
        {
            T x1 = _summits[i].getX();
            T y1 = _summits[i].getY();
            T x2 = _summits[i + 1].getX();
            T y2 = _summits[i + 1].getY();

            s += x1 * y2 - x2 * y1;
        }

        _surface = std::abs(s) / 2.0;
    }

private:
    std::vector<Point2D<T>> _summits;
    double _surface = 0;
};
 