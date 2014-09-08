#include "money.h"
#include <math.h>

#define RTIME 100

Money::Money(unsigned _x, unsigned _y)
{
    // INITIATE SPRITES
    spr = new Sprite("data/spr_money.png",2,1);

    gPos = Point(_x,_y);
    pos = Point(_x*GRID_WIDTH, _y*GRID_HEIGHT);

    rTime = 0;

    stolen = false;
}

Money::~Money()
{
    if (spr) delete spr;
}

//Set it to respawn.
void Money::respawn()
{
    //rTime = RTIME;

    //Reset position.
    gPos.x = floor(pos.x/GRID_WIDTH);
    gPos.y = floor(pos.y/GRID_HEIGHT);
    gPos.y++;
    pos.x = gPos.x*GRID_WIDTH;
    pos.y = gPos.y*GRID_HEIGHT;

    stolen = false;
}

void Money::draw()
{
    if (!rTime ||
        (rTime < RTIME/4 && rTime%4 > 1)) //Flash on re-entry.
    {
        spr->setPos(pos.x+SPRITEX_OFF, pos.y+SPRITEY_OFF);
        if (spr) spr->draw(false, 0);
    }

    if (rTime) rTime--;

    //Fix positioning:
    while (gPos.y != 5 and gPos.y != 12 and gPos.y != 19 and gPos.y != 26)
    {
        respawn();
    }
}

void Money::getStolen(Point _pos)
{
    pos = _pos;
    stolen = true;
}
