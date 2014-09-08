#include "thief.h"

#define LIFT 35
#define LIFTSPEED 2
#define SLOW 2
#define HIT 4  //Used for flashing.

#include <stdlib.h>
#include <time.h>
#include <math.h>

Thief::Thief(unsigned _x, unsigned  _y, vector<Money*>& _route, std::string _str, unsigned _health) : Agent(_x,_y,_route,_str)
{
    spawn(Point(_x,_y));

    dead = false; //Start off dead and ready to be dropped off.

    sprOther = new Sprite("data/spr_other.png", 2, 1);

    srand(time(NULL));
    sMoney.clear();

    addVision = 20*GRID_WIDTH; //Thieves see further.
    health = _health;
}

Thief::~Thief()
{
    if (sprOther) delete sprOther;
}

void Thief::spawn(Point _s, unsigned _health)
{
    dead = false;

    gPos = _s;
    pos = Point(gPos.x*GRID_WIDTH, gPos.y*GRID_HEIGHT);

    item = hold = near = false;

    target = getMoney();
    setDest(target->getPos());

    lift = 0;
    seen = false;
    heard = false;
    see = false;
    pathExcep = false;
    hit = 0;

    home = _s;
    health = _health;

    run = false;
    running = false;
    visible = true;

    destReached = true;
    clearPath();

    xSpeed = false;
    ySpeed = false;

    emote = 0;

    fixSpeed();
}

void Thief::seeMoney(vector<Money*> _m)
{
    //sMoney.clear();

    for (unsigned i = 0; i < _m.size(); i++)
    {
        bool dupe = false;

        for (unsigned j = 0; j < sMoney.size(); j++)
        {
            if (sMoney.at(j) == _m.at(i))
            {
                dupe = true;
            }
        }

        if (!dupe) sMoney.push_back(_m.at(i));
    }
}

void Thief::step()
{
    fsm();

    drawLife();
}

void Thief::drawLife()
{
    if (sprOther)
    {
        for (unsigned i = 0; i < health; i++)
            sprOther->drawCopy(false, 0, pos.x-(health/2)*12+16*i, pos.y-GRID_HEIGHT*2);
    }
}

void Thief::setHome(Point _h)
{
    home = _h;
}

// Recieve messages and change values accordingly.

void Thief::getMsg(vector<std::string> _msg)
{
    seen = false;
    heard = false;
    see = false;
    stop = false;

    //Read through our messages.
    for (unsigned i = 0; i < _msg.size(); i++)
    {
        //Have we been seen ?
        if (_msg.at(i) == "seen") seen = true;

        //Do we hear the guard?
        if (_msg.at(i) == "heard") heard = true;

        //Do we see the guard?
        if (_msg.at(i) == "see") see = true;

        //Are we hit?
        if (_msg.at(i) == "hit") hit = HIT;

        //Is all the money gone?
        if (_msg.at(i) == "money") stop = true;
    }
}

void Thief::fixSpeed()
{
    if (speedMod != 1)
    {
        speedMod = 1;
        pos.x = floor(pos.x/GRID_WIDTH)*GRID_WIDTH;
        pos.y = floor(pos.y/GRID_HEIGHT)*GRID_HEIGHT;
    }
}

void Thief::fsm()
{
    ////////////////////////////
    /// FINITE STATE MACHINE ///
    ////////////////////////////

    /// Reset emotions.
    emote = 0;
    visible = true;

    /// You are dying
    if (!health && !dead)
    {
        myState = "sDie";

        pathExcep = true;
        sDie();
        return;
    }

    /// This state runs alongside others..
    if (hit)
    {
        myState = "sHit";
        run = true;

        sHit();
    }

    /// If we reached home after being caught, leave.
    if (run && gPos == home)
    {
        myState = "sDead";

        dead = true;
        xSpeed = 0;
        ySpeed = 0;
        return;
    }

    /// If we hear the guard coming, DROP THE BAG!
    if (heard && !ySpeed && lift)
    {
        myState = "sDropBag(safe)";

        sDropBag();
        return;
    }

    /// If we're seen by the guard and are caught in the act, DROP THE BAG!
    if (seen && !ySpeed && lift)
    {
        myState = "sDropBag(danger)";

        fixSpeed();
        sDropBag();
        run = true;
        return;
    }


    /// THEN RUN (call once)!
    /// Will run until it gets home.
    if (run)
    {
        myState = "sRun";

        emote = 2;
        fixSpeed();
        sRun();
        running = true;;
        return;
    }
    if (running) {emote = 2; myState = 11; return;}

    /// If we're seen by the guard, or hear the guard, or see the guard but are not caught in the act, act casual.
    if ((seen || heard || see) && !ySpeed && !hold && !lift)
    {
        myState = "sWander";

        fixSpeed();
        sWander();
        return;
    }

    /// If we have no bag, go get one!
    if (!item)
    {
        myState = "sGetBag";

        sGetBag();
        return;
    }

    /// If we're near a bag, but not holding it, pick it up!
    if (!seen && near && !hold)
    {
        myState = "sLiftBag";

        sLiftBag();
        return;
    }

    /// If there's a bag over our heads, return home with the prize!
    if (hold)
    {
        myState = "sSteal";
        sSteal();
        target->getStolen(Point(pos.x, pos.y-lift));
    }

    /// If we make it home, leave with the spoils.
    if (hold && gPos == home)
    {
        myState = "sDropBag(exit)";
        sDropBag();
        return;
    }
}

void Thief::sDie()
{
    if (target)
    {
        target->respawn();
        target = NULL;
    }

    xSpeed = 0;
    ySpeed += 1;
    if (!dying) clearPath();
    dying = true;


    if (pos.y > 600)
    {
        dying = false;
        spawn(home);
        visible = false;
        dead = true;
    }
}

void Thief::sHit()
{
    visible = false;

    if (hit)
    {
        if (hit == HIT && health) health--;

        if (!health) ySpeed = -10;

        hit--;
    }
}

void Thief::sWander()
{
    /// Wander around randomly (act casual).
    emote = 1;

    if (rand()%15 == 0)
    {
        Point pt = Point(0,0);

        //Make sure it's a valid point.
        while (bGrid[pt.x][pt.y])
            pt = Point(gPos.x+1-(rand()%2*2), gPos.y);

        setDest(pt);
    }
}

/// STATE: Get the nearest bag!
void Thief::sGetBag()
{
    if (rMoney->size())
    {
        //Do we have the bag?
        if (target && target->getPos() == gPos)
        {
            item = true;
            near = true;
            return;
        }

        //Then we want this bag!
        //If there is even a bag left to carry
        if (!stop)
        {
            //Go towards the nearest free money bag (ignores all Money in sMoney);
            target = getMoney(&sMoney);

            //Set our sights on it (assuming we found it)
            if (target)
                setDest(target->getPos());
        }
        else
        {
            target = NULL;
            clearPath();
            setDest(gPos);
            xSpeed = 0;
            ySpeed = 0;
            emote = 3;
        }
    }
    else
    {
        target = NULL;
        clearPath();
        setDest(gPos);
        dead = false;
        health = 0;
    }
}

void Thief::sLiftBag()
{
    //Double check we have something to lift
    if (target)
    {
        lift += LIFTSPEED;

        //If it's not over our heads yet...
        if (lift < LIFT)
        {
            //Lift it up (and hold still)
            xSpeed = 0;
            target->getStolen(Point(pos.x, pos.y-lift));
        }
        else //Otherwise, we've taken the bag!
        {
            hold = true;
            sMoney.clear(); //No need to avoid other thieves bags now.
            return;
        }
    }
}

void Thief::sDropBag()
{
    //Similar to lift bag.

    //Double check that we are holding something.
    if (target)
    {
        lift -= LIFTSPEED;

        //If it's not on the ground yet...
        if (lift > 0)
        {
            //Put it down (and hold still)
            xSpeed = 0;
            target->getStolen(Point(pos.x, pos.y-lift));
        }
        else
        {
            //Let the bag respawn.
            target->respawn();
            sMoney.clear();

            // Otherwise, check if we dropped it near the exit
            if (gPos == home)
            {
                //Since we did, the bag is stolen! Remove it from the memory!
                for (unsigned i = 0; i < rMoney->size(); i++)
                {
                    if ((*rMoney->at(i)) == (*target))
                    {
                        rMoney->erase(rMoney->begin() + i);
                    }
                }

                //Go back to normal speed.
                fixSpeed();
                dead = true;
            }

            target = NULL;

            //We're not holding anything anymore.
            //Reset our values so we may steal again.
            hold = false;
            item = false;
            near = false;
            return;
        }
    }

}

void Thief::sSteal()
{
    //speedMod = SLOW; //Slow down.
    setDest(home);
    sMoney.clear();
}

void Thief::sRun()
{
    setDest(home);
    sMoney.clear();
}
