#ifndef BULLET_H
#define BULLET_H

#include "point.h"
#include "sprite.h"

#define SPEED 12

class Bullet
{
private:
    Sprite *spr;

    Point pos;
    unsigned xSpeed;

public:

    Bullet(Point _s, bool dir);
    ~Bullet();

    bool dead;

    void draw();

    inline Point getPos() const {return pos;};
};

#endif // BULLET_H
