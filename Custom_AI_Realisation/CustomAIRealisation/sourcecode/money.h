#ifndef MONEY_H
#define MONEY_H

#include "point.h"
#include "sprite.h"
#include "global.h"

#include <cstddef>

class Money
{
private:
    Sprite *spr;
    Point pos;
    Point gPos;

    unsigned rTime;   //If it's dropped, it'll reappear in this much time.

public:
    Money(unsigned _x, unsigned _y);
    ~Money();

    void draw();
    inline Point getPos() const {return gPos;};
    inline void setPos(Point _p) {gPos = _p; pos = Point(gPos.x*GRID_WIDTH, gPos.y*GRID_HEIGHT);};

    inline bool operator==(const Money& other)
    {
        if (gPos == other.getPos()) return true;
        return false;
    }

    bool stolen;

    void getStolen(Point _pos);
    void respawn();
};

#endif // MONEY_H
