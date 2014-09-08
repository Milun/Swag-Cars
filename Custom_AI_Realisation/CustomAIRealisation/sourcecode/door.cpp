#include "door.h"

Door::Door()
{
    spr = new Sprite("data/spr_door.png", 2, 1);
}

Door::~Door()
{
    if (spr) delete spr;
}

void Door::draw()
{
    if (spr) spr->draw(false, 0);
}
