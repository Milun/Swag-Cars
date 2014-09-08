#include "bullet.h"
#include "global.h"

Bullet::Bullet(Point _s, bool dir)
{
    pos = _s;
    xSpeed = 12*(-1+(dir*2));

    spr = new Sprite("data/spr_other.png",2,1);

    dead = false;
}

Bullet::~Bullet()
{
    if (spr) delete spr;
}

void Bullet::draw()
{
    //Check to die.
    if (pos.x < -20 || pos.x > 850)
        dead = true;

    if (!dead)
    {
        //Increment position.
        pos.x += xSpeed;

        if (spr)
        {
            spr->setPos(pos.x, pos.y);
            spr->draw(0, 1);
        }
    }
}
