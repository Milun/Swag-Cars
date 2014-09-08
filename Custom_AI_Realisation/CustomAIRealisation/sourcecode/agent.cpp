#include "agent.h"
#include "global.h"

#include <math.h>

#define SIGHT_WIDTH  20*GRID_WIDTH
#define SIGHT_HEIGHT 6*GRID_HEIGHT

using namespace std;

Agent::Agent(unsigned _x, unsigned _y, vector<Money*>& _route, string _spr)
{
    // INITIATE SPRITES
    spr = new Sprite(_spr,8,2);
    sprEmote = new Sprite("data/spr_emote.png", 5, 1);
    sprDebug = new Sprite("data/spr_debug.png", 5, 2);

    // INITIATE OTHER VALUES
    xSpeed = ySpeed = 0;
    gPos = Point(_x,_y);
    pos = Point(_x*GRID_WIDTH, _y*GRID_HEIGHT);
    facing = false;
    speedMod = 1;
    emote = 0;
    addVision = 0;
    visible = true;
    dead = false;
    dying = false;
    pathExcep = false;

    pathDest = Point(_x,_y);

    rMoney = &_route;

    // AND POINTERS.
    pathEnd = NULL;

    destReached = true;
}

Agent::~Agent()
{
    if (spr) delete spr;
    if (sprDebug) delete sprDebug;
    if (sprEmote) delete sprEmote;
}

void Agent::debug()
{
    if (sprDebug)
    {
        //Draw where we're going and the gPos where we stand.
        sprDebug->drawCopy(false, 3, gPos.x*GRID_WIDTH, gPos.y*GRID_HEIGHT);
        sprDebug->drawCopy(false, 4, pos.x, pos.y);

        //Draw our vision.
        if (!ySpeed)
        {
            for (unsigned i = 0; i < SIGHT_WIDTH/GRID_WIDTH; i++)
                for (unsigned j = 0; j < SIGHT_HEIGHT/GRID_HEIGHT; j++)
                {
                    sprDebug->drawCopy(true, 2, pos.x+i*GRID_WIDTH*(-1+facing*2), pos.y-j*GRID_HEIGHT);
                    if (addVision) sprDebug->drawCopy(true, 2, pos.x+i*GRID_WIDTH*(1-facing*2), pos.y-j*GRID_HEIGHT);
                }
        }
    }
}

void Agent::setDest(Point _p)
{
    pathDest = _p;
}

void Agent::vision()
{
    //////////////
    /// VISION ///
    //////////////

    //For simplicities sake, if the enemy is on a ladder, they can't see anything.
    if (ySpeed || (gPos.y != 19 && gPos.y == 12 && gPos.y == 5 && gPos.y == 26)) {
        sight[0] = Point(0,0);
        sight[1] = Point(0,0);
    }

    //Looking to the left.
    if (!facing)
    {
        sight[0] = Point(pos.x-SIGHT_WIDTH, pos.y-SIGHT_HEIGHT);
        sight[1] = Point(pos.x+addVision, pos.y);
    }
    else //Looking to the right.
    {
        sight[0] = Point(pos.x-addVision, pos.y-SIGHT_HEIGHT);
        sight[1] = Point(pos.x+SIGHT_WIDTH, pos.y);
    }
}

void Agent::draw()
{
    ////////////
    // PATROL //
    ////////////

    //Move. destReached will be true if the player is at pathDest
    if (!dead && !dying && !pathExcep) destReached = moveToPos(pathDest.x, pathDest.y);

    //followPath();

    //Run the step function from the child.
    step();

    //Update where we're facing.
    if (xSpeed < 0) facing = false;
    if (xSpeed > 0) facing = true;

    //Limit xSpeed.
    if (xSpeed > SPEED) xSpeed = SPEED;
    if (xSpeed < -SPEED) xSpeed = -SPEED;

    //Fix grid clipping.
    if (!dead and !dying and !pathExcep)
    {
        if (xSpeed) pos.y = gPos.y*GRID_HEIGHT;
        if (ySpeed) pos.x = gPos.x*GRID_WIDTH;
        if (xSpeed == ySpeed) {
            pos.x = gPos.x*GRID_WIDTH;
            pos.y = gPos.y*GRID_HEIGHT;
        }
    }

    //Move
    pos.y += ySpeed/speedMod;

    if (!ySpeed) //Prevent diagonal moving (seriously, this leads to huge glitches).
        pos.x += xSpeed/speedMod;


    //Handle vision:
    vision();


    //Only update the square we stand on when we're perfectly alligned.
    if (!pathExcep)
    {
        if (pos.x % GRID_WIDTH == 0)
            gPos.x = floor(pos.x/GRID_WIDTH);
        if (pos.y % GRID_HEIGHT == 0)
            gPos.y = floor(pos.y/GRID_HEIGHT);
    }

    ////////////
    /// DRAW ///
    ////////////

    //Draw the enemy, with the offset.
    spr->setPos(pos.x+SPRITEX_OFF,pos.y+SPRITEY_OFF);

    if (visible)
    {
        if (dead || dying)
            spr->draw(facing, 6);
        else if (emote == 3)
            spr->draw(facing, 7);
        else
        {
            //Draw the enemy based on its actions.
            if (ySpeed) {
                spr->draw(true, 4, 5, 6, 0);
            }
            else {
                if (xSpeed) spr->draw(facing, 0, 3, 6, 0);
                else spr->draw(facing, 0);
            }
        }
    }

    // Draw emotes.
    if (sprEmote)
    {
        switch (emote)
        {
        case 1:
            sprEmote->setPos(pos.x, pos.y-GRID_HEIGHT*2);
            sprEmote->draw(false, 0, 1, 20);
            break;
        case 2:
            sprEmote->setPos(pos.x, pos.y-GRID_HEIGHT*2);
            sprEmote->draw(false, 2);
            break;
        case 3:
            sprEmote->setPos(pos.x, pos.y-GRID_HEIGHT*2);
            sprEmote->draw(false, 3, 4, 20);
            break;
        default:
            break;
        }
    }

    if (DEBUG)
    {
        //Draw our final path.
        //if (dest != NULL)
        //    drawPath(dest);

        //Draw our destination (purple square).
        //if (sprDebug && dest)
        //    sprDebug->drawCopy(false, 3, dest->x*GRID_WIDTH, dest->y*GRID_HEIGHT);
        debug();
    }
}

void Agent::clearPath()
{
    finalPath.clear();
}

void Agent::followPath()
{
    //Remove any tile that we are on from the path.
    if (finalPath.size() > 1 && finalPath.back() == Point(gPos.x, gPos.y))
        finalPath.erase(finalPath.end());

    //Move towards the next tile if it's there.
    if (finalPath.size())
    {
        //Reset the sprite.
        bool temp = (ySpeed != 0);

        xSpeed = (finalPath.back().x - gPos.x)*SPEED;
        ySpeed = (finalPath.back().y - gPos.y)*SPEED;

        if (temp != (ySpeed !=0)) spr->reset();
    }
}

//Function fires every time we cross a tile.
bool Agent::moveToPos(unsigned _x, unsigned _y)
{
    ////////////////////////////////////
    // PERFORM A BREADTH-FIRST SEARCH //
    ////////////////////////////////////

    //Clear the path.
    pathEnd = NULL;

    vector<Point*> searchPath;  //Stores all currently checked tiles.

    destReached = false;
    Point finish = Point(_x, _y, NULL);

    //Add our starting point.
    searchPath.push_back(&gPos);

    //Perform a Breadth-First search.
    makePath(searchPath, &gPos, finish);

    //At last, make the final path for our enemy to follow.
    if (pathEnd)
    {
        //Recalculate the path.
        if (aligned())
        {
            clearPath();
            makeFinalPath(pathEnd);
        }

        //Move along it.
        followPath();

        //Draw it for debug.
        if (DEBUG) drawPath(pathEnd);
    }

    //Clear the search path. We won't need it anymore.
    for (unsigned i = 0; i < searchPath.size(); i++)
        delete searchPath.at(i);

    if (gPos == Point(_x,_y)) return true; //Your character will be motionless if they reach the end. xSpeed can only = ySpeed when both are 0.
    return false;
}

bool Agent::aligned()
{
    return (gPos.x*GRID_WIDTH == pos.x and gPos.y*GRID_HEIGHT == pos.y);
}

/// Stores the path that must be taken.
void Agent::makeFinalPath(Point* _p)
{
    finalPath.push_back(Point(_p->x, _p->y));

    if (_p->parent) makeFinalPath(_p->parent);

    return;
}

unsigned Agent::getPathLength(Point* _p)
{
    unsigned length = 1;
    Point* p = _p;     //Holds the points parents.

    while (p->parent)
    {
        length++;
        p = p->parent;
    }

    return length;
}

bool Agent::drawPath(Point* _p)
{
    //Draw the current tile.
    if (sprDebug) sprDebug->drawCopy(false, 2, _p->x*GRID_WIDTH, _p->y*GRID_HEIGHT);

    //Check for more.
    if (_p->parent != NULL) drawPath(_p->parent);

    return false;
}

bool Agent::checkTile(vector<Point*>& _v, Point* _p)
{
    //Check the vector to see if we've already checked this tile.
    for (unsigned i = 0; i < _v.size(); i++)
        if (_v.at(i)->x == _p->x && _v.at(i)->y == _p->y)
            return true;

    return false;
}

void Agent::helpPath(vector<Point*>& _v, vector<Point*>& _s, Point* _p, Point& _e, int _x, int _y)
{
    //Check a point one step away from your current position.
    Point *p = new Point( _p->x+_x, _p->y+_y, _p);

    //If it's our destination, let it be known (will stop the function).
    if ((*_p) == _e) {
        pathEnd = _p;
        return;
    }
    else if (!checkTile(_s, p) &&
        p->x > 0 &&
        p->y > 0 &&
        p->x < MAP_WIDTH &&
        p->y < MAP_HEIGHT &&
        !bGrid[p->x][p->y])
    {
        //Otherwise, add to the vector so we don't search again
        _s.push_back(p);

        //And add to the list of things to check.
        _v.push_back(p);

        return; //Keep searching.
    }
}

Money* Agent::getMoney(vector<Money*>* _v)
{
    Money* op = NULL;
    unsigned length = 9999;

    //Loop through each Money on the field.
    for (unsigned i = 0; i < rMoney->size(); i++)
    {
        //Oh... so this was what was causing that problem...
        //AH SCREW IT!

        Point finish = Point(rMoney->at(i)->getPos());
        unsigned l = abs(finish.x-gPos.x)+abs(finish.y-gPos.y)*3;

        bool check = true;

        if (_v != NULL)
        {
            for (unsigned j = 0; j < _v->size(); j++)
                if (_v->at(j)->getPos() == rMoney->at(i)->getPos()) check = false;
        }

        //if (check && (op == NULL || _length < length))
        if (check && l < length)
        {
            length = l;
            op = rMoney->at(i);
        }
    }

    return op;
}

unsigned Agent::makePath(vector<Point*>& _v, Point* _p, Point& _e)
{
    //Check our starting position to see if we're there.
    if ((*_p) == _e)
    {
        pathEnd = _p;
        return 0;
    }

    //Otherwise, check and store the surrounding tiles.
    vector<Point*> check;
    check.push_back(_p);    //Add our starting point to the vector.
    unsigned length = 0;    //Path length.

    //Loop until we find our destination.
    while (!pathEnd)
    {
        //Set up a vector to remember this level.
        vector<Point*> level;
        level.clear();

        //For each element in our check vector
        while (check.size() != 0)
        {
            //Store our last element
            Point* l = check.back();

            //Remove it.
            check.pop_back();

            //Check its surrounding tiles.
            //These functions will also add the tiles they check to the level vector.

            helpPath(level, _v, l, _e, -1, 0);
            helpPath(level, _v, l, _e,  1, 0);
            helpPath(level, _v, l, _e,  0,-1);
            helpPath(level, _v, l, _e,  0, 1);
        }

        //Add to the length
        length++;

        //Check if a dead end was reached
        if (!level.size()) return 999;

        //Once the 'check' vector is empty, refill it with level.
        for (unsigned i = 0; i < level.size(); i++)
            check.push_back(level.at(i));
    }

    return length;
}
