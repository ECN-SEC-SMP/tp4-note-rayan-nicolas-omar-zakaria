#pragma once
#include "point2d.hpp"

#include <vector>

template <typename T>
class Polygon
{
public:
    Polygon() {}
    Polygon(const Polygon<T> &other) : _summits(other._summits), _area(other._area) {}

    Polygon(const std::vector<Point2D<T>> &summits) : _summits(summits)
    {
        _recalculateArea();
    }

    const std::vector<Point2D<T>> &getSummits() { return _summits; };

    void setSummits(const std::vector<Point2D<T>> &summits)
    {
        _summits = summits;
        _recalculateArea();
    }

    void addSummit(const Point2D<T> &p)
    {
        _summits.push_back(p);
        _recalculateArea();
    }

    void translate(T x, T y)
    {
        for (auto &p : _summits)
            p.translate(x, y);
    }

    friend std::ostream &operator<<(std::ostream &os, const Polygon &p)
    {
        for (size_t i = 0; i < p._summits.size(); ++i)
        {
            os << p._summits[i];

            if (i != p._summits.size() - 1)
                os << ' ';
        }

        return os;
    }

private:
    void _recalculateArea()
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

        _area = std::abs(s) / 2.0;
    }

private:
    std::vector<Point2D<T>> _summits;
    double _area = 0;
};
