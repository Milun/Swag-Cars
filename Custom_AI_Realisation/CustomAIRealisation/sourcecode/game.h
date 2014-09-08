/**
 *  More like a level really
**/

#ifndef GAME_H
#define GAME_H

#include "sprite.h"
#include "guard.h"
#include "thief.h"
#include "money.h"
#include "message.h"
#include "bullet.h"

#include <vector.h>

class Game
{
private:
    Sprite *sprBack;
    Sprite *sprDebug;
    Sprite *sprDoor;
    Message *message;   //Conveys messages between characters.

    Point dPos;         //Position of the door.
    bool dMove;         //If you can move the door.

    vector<Bullet*> bullets;

    vector<Money*> money; //Location for moneybags (also act as waypoints).

    char **grid;

    void slowAssign();
    void moveDoor();
    void randMoney();   //Places Money at random, valid locations.

public:
    Game();
    ~Game();

    vector<Guard*>  guards;
    vector<Thief*>  thieves;

    void draw();
};

#endif // GAME_H

