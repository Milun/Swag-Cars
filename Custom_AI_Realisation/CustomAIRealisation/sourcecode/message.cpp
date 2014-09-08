#include "message.h"
#include <math.h>

#define LW 20

/// Returns true if you're in the guards line of sight.
bool Message::seenByGuard(Point _pos)
{
    //Check the vision of all guards.
    if (guards->size())
    {
        for (unsigned i = 0; i < guards->size(); i++)
        {
            //Ignore sleeping guards.
            if (guards->at(i)->sleep) continue;

            //Get the top left and bottom right points.
            Point tl = (*guards->at(i)->getSight());
            Point br = (*(guards->at(i)->getSight()+1));

            //Return true if we're seen.
            if (tl.x <= _pos.x && tl.y <= _pos.y && br.x >= _pos.x && br.y >= _pos.y)
                return true;
        }
    }

    vector<Money*> dummyM;
    vector<Bullet*> dummyB;
    //Create the dummy thief.
    dummy = new Guard(0,0,dummyM,"data/spr_thief.png", dummyB);

    return false;
}

/// Returns true if the thief is dead.
bool Message::thiefDead()
{
    if (thieves->size())
        if (thieves->at(0)->dead)
            return true;
    return false;
}

/// Returns true if there's a bullet in the thief
bool Message::checkHit(Point _pos)
{
    //Check the positions of all bullets
    if (bullets->size())
    {
        for (unsigned i = 0; i < bullets->size(); i++)
        {
            //Get the position of the bullet
            Point pos = bullets->at(i)->getPos();

            //Return true if it's in the thief.
            if (pos.x >= _pos.x-LW && pos.y >= _pos.y-LW*2 && pos.x <= _pos.x+LW && pos.y <= _pos.y+LW*2)
            {
                bullets->at(i)->dead = true;
                return true;
            }
        }
    }

    return false;
}

//Returns true if all money is being/has been stolen
bool Message::countMoney()
{
    if (money)
    {
        unsigned temp = 0;

        for (unsigned i = 0; i < money->size(); i++)
            if (money->at(i)->stolen) temp++;

        if (temp == money->size()) return true;
    }

    return false;
}

///Returns true if there's a thief in sight (only sees them if they're holding a bag).
Thief* Message::seeThief(Point *_pos)
{
    //Check the position of all thieves.
    if (thieves->size())
    {
        for (unsigned i = 0; i < thieves->size(); i++)
        {
            //Get the top left and bottom right points.
            Point tl = (*_pos);
            Point br = (*(_pos+1));

            //Get the position of the thief.
            Point pos = thieves->at(i)->getPos();

            //Return true if we see a thief holding a bag.
            if (tl.x <= pos.x && tl.y <= pos.y && br.x >= pos.x && br.y >= pos.y)
                if (thieves->at(i)->getLifting()) return thieves->at(i);
        }
    }

    return NULL;
}

///Returns true if there's a guard in sight.
bool Message::seeGuard(Point *_pos)
{
    //Check the position of all guards.
    if (guards->size())
    {
        for (unsigned i = 0; i < guards->size(); i++)
        {
            //Ignore sleeping guards.
            if (guards->at(i)->sleep) continue;

            //Get the top left and bottom right points.
            Point tl = (*_pos);
            Point br = (*(_pos+1));

            //Get the position of the thief.
            Point pos = guards->at(i)->getPos();

            //Return true if we see a guard
            if (tl.x <= pos.x && tl.y <= pos.y && br.x >= pos.x && br.y >= pos.y)
                if (guards->at(i)) return true;
        }
    }

    return false;
}

///Returns a vector notifying the thief of all money it sees being stolen.
vector<Money*> Message::seeMoney(Point *_pos)
{
    vector<Money*> op;

    //Check the position of all thieves.
    if (thieves->size())
    {
        for (unsigned i = 0; i < thieves->size(); i++)
        {
            //Get the top left and bottom right points of the thieves vision.
            Point tl = (*_pos);
            Point br = (*(_pos+1));

            //Get the position of the potential thief.
            Point pos = thieves->at(i)->getPos();

            //If we see a thief
            if (tl.x <= pos.x && tl.y <= pos.y && br.x >= pos.x && br.y >= pos.y)
            {
                //Check if that thief is holding a bag.
                if (thieves->at(i)->getLifting())
                    op.push_back(thieves->at(i)->target);
            }
        }
    }

    return op;
}


/// Returns a Thief if a guard is in pursuit. Otherwise, just returns a dummy thief.
Agent* Message::hearGuard(Point _pos)
{
    //Check the vision of all guards.
    if (guards->size())
    {
        for (unsigned i = 0; i < guards->size(); i++)
        {
            //Don't listen if it's you. And don't listen if it's sleeping.
            if (guards->at(i)->getPos() != _pos && !guards->at(i)->sleep)
            {
                //Get the radius of the guards sound and the guards position.
                Point pos = guards->at(i)->getPos();
                unsigned sound = guards->at(i)->getSound();

                //If we hear the guard...
                if ( sound >= sqrt( pow(pos.x-_pos.x, 2) + pow(pos.y-_pos.y,2) ))
                {
                        //If the guard is chasing something.
                        if (guards->at(i)->target and guards->at(i)->chase)
                            return guards->at(i)->target;
                        else //Otherwise, just return the dummy thief.
                            return dummy;
                }
            }
        }
    }

    return NULL;
}

void Message::step()
{
    /////////////////////
    /// SEND MESSAGES ///
    /////////////////////

    // Sends messages to each agent which they interpret in their own way.

    ///////////////
    /// THIEVES ///
    ///////////////

    // For every thief
    for (unsigned i = 0; i < thieves->size(); i++)
    {
        //Set up our message vector.
        vector<std::string> msg;
        msg.clear();

        //Check if the thief is seen.
        if (seenByGuard(thieves->at(i)->getPos()))
            msg.push_back("seen");

        //Check if the thief hears the guard.
        if (hearGuard(thieves->at(i)->getPos()))
            msg.push_back("heard");

        //Check if the thief sees the guard.
        if (seeGuard(thieves->at(i)->getSight()))
            msg.push_back("see");

        //Check if the money is all gone guard.
        if (countMoney())
            msg.push_back("money");

        //Check if the thief is hit.
        if (checkHit(thieves->at(i)->getPos()))
            msg.push_back("hit");

        //Check if the thief sees another thief holding a bag.
        thieves->at(i)->seeMoney(seeMoney(thieves->at(i)->getSight()));

        //And notify them of this information.
        thieves->at(i)->getMsg(msg);
    }

    //////////////
    /// GUARDS ///
    //////////////

    // For every guard
    for (unsigned i = 0; i < guards->size(); i++)
    {
        //Set up our message vector.
        vector<std::string> msg;
        msg.clear();

        //Check if the guard sees a thief with a bag.
        if (seeThief(guards->at(i)->getSight()))
        {
            msg.push_back("seen");
            //Give the guard a target.
            guards->at(i)->setTarget(seeThief(guards->at(i)->getSight()));
        }
        //else //Remove the target otherwise.
        //guards->at(i)->setTarget(NULL);

        Agent* temp = hearGuard(guards->at(i)->getPos());
        //Check if the guard hears the guard.
        if (hearGuard(guards->at(i)->getPos()))
        {
            msg.push_back("heard");
            //If the guard hears a guard in pursuit.
            if (temp != dummy && !guards->at(i)->chase)
            {
                guards->at(i)->setTarget(temp);
                //msg.push_back("seen");

                guards->at(i)->chase = true;
            }
        }


        if (thiefDead()) msg.push_back("thiefdead");

        //And notify them of this information.
        guards->at(i)->getMsg(msg);
    }
}
