#ifndef DOOR_H
#define DOOR_H

#include "sprite.h"
#include "point.h"

class Door
{
private:
    Sprite *spr;

    Point gPos;

public:
    Door();
    ~Door();

    void draw();
};

#endif
