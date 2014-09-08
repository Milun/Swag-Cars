#ifndef GUARD_H
#define GUARD_H

#include "agent.h"
#include "message.h"
#include "bullet.h"

//class Message;

class Guard;

class Guard : public Agent
{
private:

    vector<Money*> rCheck;  //Stores references to the currently checked Money.

    unsigned lTime;         //Look time.
    bool looking;
    bool looked;
    unsigned sound;         //Emit sound.
    unsigned sTime;         //Shoot time;
    unsigned hTime;         //Hop time (for waking up).

    bool hop;               //Hop when waking up.
    bool heard;

    unsigned boredom;

    Money *mTarget;         //Which Money to inspect

    vector<Bullet*>* bullets; //Know how to shoot.

    void fsmPatrol();       //FINITE STATE MACHINE: Handles changing states during patrol.
    void sLook();           //STATE: Look around before proceeding. Returns false when your done.
    void sPatrol();         //STATE: Go to the next nearest Money.
    void sChase();          //STATE: Chase the thief!
    void sSleep();          //STATE: Take a nap from boredom.
    void sHop();            //STATE: Hop when you awake.

    void fSound();           //Handles sound emited by the guard.

public:
    //"data/spr_debug.png"
    Guard(unsigned _x, unsigned  _y, vector<Money*>& _route, std::string _str, vector<Bullet*>& _bullets);
    ~Guard();

    bool sleep;
    Agent *target;          //Who to chase.

    //For messages.
    inline unsigned getSound() {return sound;};

    void getMsg(vector<std::string> _msg);
    inline void setTarget(Agent* _target) {target = _target;};

    bool chase;             //Have we caught the thief?

    void step();

};

#endif // GUARD_H
