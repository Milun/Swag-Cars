#ifndef POINT_H
#define POINT_H

#include <cstddef>

class Point
{
public:
    int x, y;
    Point *parent;

    inline Point(int _x = 0, int _y = 0, Point *par = NULL) {x = _x; y = _y; parent = par;};
    inline bool operator==(const Point& other)
    {
        if (x == other.x && y == other.y) return true;
        return false;
    }

    inline bool operator!=(const Point& other)
    {
        if (x != other.x || y != other.y) return true;
        return false;
    }
};
#endif // POINT_H
