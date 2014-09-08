/**
 *  The parent class to all enemies
**/

#ifndef ENEMY_H
#define ENEMY_H

#include "sprite.h"
#include "point.h"
#include "money.h"

#include <vector.h>
#include <string>

class Agent
{
private:
    Sprite *spr;
    Sprite *sprEmote;

    Point* pathEnd;

    void debug();       //Draws adittional debug.

    // "VISION" FUNCTIONS //
    Point sight[2];     //The top left and bottom right corners for the players vision.
    void vision();      //Handle the main vision of the enemy.

    vector<Point> finalPath;    //Contains the nodes the enemy must follow, in order.

    void followPath();
    unsigned makePath(vector<Point*>& _v, Point* _p, Point& _e);            //Returns a debug value.
    void helpPath(vector<Point*>& _v, vector<Point*>& _s, Point* _p, Point& _e, int _x, int _y);//Helper function for makePath.
    void makeFinalPath(Point* _p);                                          //Makes the final path to follow.
    unsigned getPathLength(Point* _p);                                      //Give a point, and it'll tell you the amount of parents.

    bool aligned();

    bool checkTile(vector<Point*>& _v, Point* _p);                          //Checks if we recross paths.
    bool drawPath(Point* _p);                                               //Draws the node chain. Recursive.

protected:
    Point pos;                  //Our actual position.
    Point gPos;                 //Our grid position.
    Point pathDest;             //Where to head to.
    bool destReached;           //Are we there yet? Needs to be set to "true" every time something happens which would cause need for a path change.

    int xSpeed, ySpeed;         //Our speed.
    bool facing;                //Which direction we're facing.
    Sprite *sprDebug;
    bool visible;

    unsigned emote;             //What emotion to show (above the head).
    unsigned addVision;
    bool pathExcep;             //In case of emergencies.

    // PATH FOLLOWING FUNCTIONS //
    void setDest(Point _p);

    // MESSAGING FUNCTIONS //
    virtual void getMsg(vector<std::string> _msg) = 0;


    // PATH FOLLOWING FUNCTIONS //


    unsigned speedMod;          //For making you move slower.

    vector<Money*>* rMoney;     //Patrol route to be taken.

    void clearPath();           //Fixes a certain glitch I had.
    virtual void step() = 0;    //For things which need to happen each step.

    bool moveToPos(unsigned _x, unsigned _y); //Moves the character to a grid location. Returns true when they reach it.

    Money* getMoney(vector<Money*>* _v = NULL);          //Returns a pointer to the nearest money bag. Will ignore all Money in the parameter.

public:
    Agent(unsigned _x, unsigned  _y, vector<Money*>& _route, std::string _str); //What tile to start the enemy on.
    ~Agent();

    bool dead;
    bool dying;
    std::string myState;

    inline Point  getPos(){return pos;};
    inline Point  getGPos() {return gPos;};
    inline Point* getSight(){return &sight[0];};

    void draw();
};

#endif // ENEMY_H
