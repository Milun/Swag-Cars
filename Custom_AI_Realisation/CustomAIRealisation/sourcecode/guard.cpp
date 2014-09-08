#include "guard.h"
#include <math.h>
#include "global.h"

#include <stdlib.h>
#include <time.h>

#define LOOKTIME 80
#define SHOOTTIME 8
#define MULT 10
#define DIV (LOOKTIME/MULT)
#define SOUND_QUIET GRID_HEIGHT*4
#define SOUND_LOUD  GRID_HEIGHT*8
#define PI 3.14159
#define BOREDOM 1000
#define HTIME 8

Guard::Guard(unsigned _x, unsigned  _y, vector<Money*>& _route, std::string _str, vector<Bullet*>& _bullets) : Agent(_x,_y,_route,_str)
{
    lTime = 0;
    looked = true;
    looking = false;
    chase = false;
    bullets = &_bullets;
    sTime = 0;
    sleep = false;
    boredom = BOREDOM;
    hop = false;
    hTime = 0;
    heard = false;
    sound = SOUND_QUIET;

    srand(time(NULL));

    mTarget = getMoney(); //Set an initial target (important)
}

Guard::~Guard()
{

}

void Guard::getMsg(vector<std::string> _msg)
{
    //chase = false; Don't reset this. Once you chase, you have to manually stop.

    heard = false;

    //Read through our messages.
    for (unsigned i = 0; i < _msg.size(); i++)
    {
        //Have we seen a thief?
        if (_msg.at(i) == "seen") chase = true;

        //Has the thief died?
        if (_msg.at(i) == "thiefdead") chase = false;

        //Do we hear a guard?
        if (_msg.at(i) == "heard") heard = true;
    }
}

void Guard::sLook()
{
    if (lTime < LOOKTIME)
    {
        //Clear the path or it will glitch.
        clearPath();
        pathExcep = true;
        myState = "sLook";

        switch ( int(floor( lTime/DIV ))%MULT )
        {
        case 0:
        case 2:
        case 5:
        case 7:
        case 8:
        case 9:
            xSpeed = 0;
            break;
        case 1:
        case 6:
            xSpeed = -SPEED;
            break;
        case 3:
        case 4:
            xSpeed = SPEED;
            break;
        default:
            xSpeed = 0;
            break;
        }

        lTime++;

        return;
    }
    else
    {
        pathExcep = false;
        looked = true;
        looking = false;
        return;
    }
}

void Guard::sPatrol()
{
    //If we've looked left and right (and are standing still).
    if (looked && xSpeed == ySpeed)
    {
        //Step 1. Set our sights for the nearest, unchecked Money.
        Money* mTarget = getMoney(&rCheck);

        //Step 2. If we've checked all Money, make a new checklist and try again.
        if (mTarget == NULL)
        {
            rCheck.clear();
            mTarget = getMoney();//getMoney(&rCheck);
        }

        //Step 3. Add it to the list of Money we've checked.
        rCheck.push_back(mTarget);

        //We've checked. Turn it off.
        looked = false;

        setDest(mTarget->getPos());
    }
}

void Guard::fsmPatrol()
{
    ////////////////////////////
    /// FINITE STATE MACHINE ///
    ////////////////////////////

    // Reset emotions.
    if (!sleep) emote = 0;
    sound = SOUND_QUIET;

    //Have we caught a thief in the act?
    if (chase and !sleep and !hop)
    {
        myState = "sChase";
        sound = SOUND_LOUD;
        sChase();
        return;
    }

    //Changes patrol states only when we reach the Money.
    if (destReached)
    {
        //If we're at our destination aren't looking, start looking.
        if (!looking) {
            lTime = 0;
            looking = true;
        }

        // If we're at the destination, and have already looked, go to the next one.
        if (looked) {
            //This state only needs to be run for one step. The guard will lock on to that location and move towards it.
            //looked = false;
            looking = false;
        }
    }

    /////////////
    /// SLEEP ///
    /////////////

    if (heard && !hop && sleep)
    {
        hop = true;
        pos.y -= GRID_HEIGHT;
        clearPath();
        setDest(gPos);
        ySpeed = 0;
        hTime = HTIME;
        boredom = BOREDOM;
    }

    if (rand()%boredom == 0 and ySpeed == 0) //Randomly fall asleep and wake up.
    {
        if (sleep && !hop) //Wake up!
        {
            //hop = true;
            //pos.y -= GRID_HEIGHT;
            clearPath();
            setDest(gPos);
            ySpeed = 0;
            hTime = HTIME;
            boredom = BOREDOM;
        }
        else if (!heard) {
            //Don't fall asleep while you're in range of a guard.
            pos.x = gPos.x*GRID_WIDTH;
            sleep = true;
            boredom = BOREDOM;
            return;
        }
        else
        {
            boredom = BOREDOM;
        }
    }

    if (hop)
    {
        myState = "sHop";
        sHop();
        return;
    }
    else if (sleep)
    {
        myState = "sSleep";
        sSleep();
    }
    else
    {

        ///////////////////////////
        /// ENTER LOOKING STATE ///
        ///////////////////////////

        myState = "sPatrol";

        //Runs the look state.
        sLook();

        //////////////////////////
        /// ENTER PATROL STATE ///
        //////////////////////////

        //Runs the patrol state.
        sPatrol();

        //Chip away at our boredom.
        if (boredom) boredom--;
    }

    if (boredom <= 0) boredom = 1;
}

void Guard::sHop()
{
    pathExcep = true;
    hTime--;
    emote = 2;
    ySpeed = 0;

    if (!hTime)
    {
        pos.y += GRID_HEIGHT;
        hop = false;
        sleep = false;
        ySpeed = 0;
        pathExcep = false;
        clearPath();
    }
}

void Guard::fSound()
{
    ////////////////////
    /// HANDLE SOUND ///
    ////////////////////

    //if (xSpeed) sound = SOUND_QUIET; //We're walking, so we make a quiet noise

    // Draw it.
    if (sprDebug && DEBUG)
        for (unsigned i = 0; i < 60; i++)
            sprDebug->drawCopy(true, 0, pos.x+cos((i*6)*PI/180)*sound-SPRITEX_OFF, pos.y+sin((i*6)*PI/180)*sound);
}

void Guard::sChase()
{
    emote = 2;
    pathExcep = false;

    //CHASE HIM!
    setDest(target->getGPos());

    //SHOOT HIM!
    if (!ySpeed)
    {
        if (sTime)
            sTime--;
        else
        {
            sTime = SHOOTTIME;
            bullets->push_back(new Bullet(pos, facing));
        }
    }

    //Rats, we lost him! Resume patrol.
    if (target && (target->dead || target->dying))
    {
        chase = false;
        destReached = true;
        xSpeed = 0;
        //target = NULL;

        //Clear our partol route.
        rCheck.clear();
        clearPath();
    }
}

void Guard::sSleep()
{
    clearPath();
    setDest(gPos);
    emote = 3; //Show Z's.
    xSpeed = 0;
    ySpeed = 0;
    //Reset the boredom.
    if (boredom) boredom--;
}

void Guard::step()
{
    fSound();
    fsmPatrol();
}
