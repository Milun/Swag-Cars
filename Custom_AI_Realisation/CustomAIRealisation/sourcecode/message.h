#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>

#include "point.h"
#include "guard.h"
#include "thief.h"
#include "bullet.h"
#include "money.h"

class Guard;
class Thief;
class Bullet;
class Money;

/**
 *  Passes messages between the thief and guard
**/

class Message
{
private:
    //Know of all the characters on the screen.
    vector<Guard*>* guards;
    vector<Thief*>* thieves;
    vector<Bullet*>* bullets;
    vector<Money*>* money;

    Agent* dummy;

    bool seenByGuard(Point _pos);
    Agent* hearGuard(Point _pos);
    Thief* seeThief(Point *_pos);
    vector<Money*> seeMoney(Point *_pos);
    bool countMoney();
    bool seeGuard(Point *_pos);
    bool checkHit(Point _pos);
    bool thiefDead();

public:
    inline Message(vector<Guard*>* _guards, vector<Thief*>* _thieves, vector<Bullet*>* _bullets, vector<Money*>* _money) {guards = _guards; thieves = _thieves; bullets = _bullets; money = _money;};
    inline ~Message() {if (dummy) delete dummy;};

    void step();
};

#endif // MESSAGE_H
