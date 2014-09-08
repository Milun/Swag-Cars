#ifndef THIEF_H
#define THIEF_H

#include "agent.h"
#include "message.h"

class Thief : public Agent
{
private:

    Sprite *sprOther;

    bool item;          //Are we looking for anything?
    bool near;          //Are we near anything?
    bool hold;          //Are we HOLDING anything?
    bool seen;          //Have we been seen?
    bool see;           //Do we see the guard?
    bool heard;         //Do we hear the guard?
    bool run;           //Should we run?
    bool running;       //Keep running.
    bool stop;          //STOP EVERYTHING! GAME OVER!
    unsigned hit;       //Have we been hit?

    unsigned health;    //How much bullets we can take.

    unsigned lift;      //Used for lifting.

    Point home;         //Where to run to when in trouble.

    void fsm();         //FINITE STATE MACHINE: Handles pretty much everything.

    void sGetBag();     //STATE: Go for the nearest bag!
    void sLiftBag();    //STATE: Lift it over your head for all to see!
    void sSteal();      //STATE: Run home with the spoils!
    void sDropBag();    //STATE: Drop the bag.
    void sWander();     //STATE: Act casual.
    void sRun();        //STATE: Run for your life.
    void sHit();        //STATE: Hit
    void sDie();        //STATE: Die

    void fixSpeed();    //Resnaps you to the grid.
    void drawLife();

    vector<Money*> sMoney;  //Stores all money the thief can see being stolen by other thieves.

public:

    Thief(unsigned _x, unsigned  _y, vector<Money*>& _route, std::string _str, unsigned _health = 5);
    ~Thief();

    void spawn(Point _s, unsigned _health = 5);       //Spawns the enemy at the specified grid location.

    void getMsg(vector<std::string> _msg);
    inline bool getLifting() {return (lift > 0);};
    void setHome(Point _h);
    void seeMoney(vector<Money*> _m);

    Money* target;

    inline bool getSpeed() {return (ySpeed != 0);}

    void step();
};

#endif // THIEF_H
