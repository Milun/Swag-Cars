#include "global.h"
#include "game.h"

#include <stdlib.h>
#include <time.h>

#define FORI(x) for (unsigned i = 0; i < x; i++)
#define FORJ(x) for (unsigned j = 0; j < x; j++)
#define UPDATEHOME for (unsigned i = 0; i < thieves.size(); i++) thieves.at(i)->setHome(Point(dPos.x+1, dPos.y*7+5))

Game::Game()
{
    // Clear the grid
    grid = new char*[MAP_WIDTH];
    for (unsigned i = 0; i < MAP_WIDTH; i++)
        grid[i] = new char[MAP_HEIGHT];

    for (unsigned i = 0; i < MAP_WIDTH; i++)
        for (unsigned j = 0; j < MAP_HEIGHT; j++)
            grid[i][j] = 0;

    //Reset the grid
    slowAssign();

    //Make it global
    bGrid = grid;

    //Sprites
    sprBack = new Sprite("data/spr_back.png", 1, 1);
    sprDoor = new Sprite("data/spr_door.png", 2, 1);
    sprDebug = new Sprite("data/spr_debug.png", 5, 1);

    //Money
    money.push_back(new Money(20,5));
    money.push_back(new Money(4,5));
    money.push_back(new Money(12,12));
    money.push_back(new Money(3,19));
    money.push_back(new Money(21,19));
    money.push_back(new Money(12,26));

    srand(time(NULL));
    randMoney();

    //Set up the door.
    dPos = Point(11,0);
    dMove = true;


    //Set up the games characters.
    //guards.push_back(new Guard(12, 5, money, "data/spr_enemy.png", bullets));
    //thieves.push_back(new Thief(17, 5, money, "data/spr_thief.png"));
    bullets.clear();

    message = new Message(&guards, &thieves, &bullets, &money);
}

//This is really poorly implemented. I know.
void Game::slowAssign()
{
    FORI(MAP_WIDTH)
        FORJ(MAP_HEIGHT)
            grid[i][j] = 1;

    FORI(MAP_WIDTH)
    {
        grid[i][5] = 0;
        grid[i][12] = 0;
        grid[i][19] = 0;
        grid[i][26] = 0;
    }

    FORI(MAP_HEIGHT-5)
    {
        grid[0][i] = 1;
        grid[1][i] = 1;
        grid[MAP_WIDTH-1][i] = 1;
        grid[MAP_WIDTH-2][i] = 1;
    }

    for (unsigned i = 6; i < 12; i++)
    {
        grid[2][i] = 0;
        grid[22][i] = 0;
    }

    for (unsigned i = 6; i < 12; i++)
    {
        grid[12][i] = 0;
    }

    for (unsigned i = 19; i < 26; i++)
    {
        grid[2][i] = 0;
        grid[22][i] = 0;
    }

    for (unsigned i = 13; i < 19; i++)
    {
        grid[5][i] = 0;
        grid[19][i] = 0;
    }

    for (unsigned i = 20; i < 26; i++)
    {
        grid[8][i] = 0;
        grid[16][i] = 0;
    }

    grid[0][26] = 1;
    grid[MAP_WIDTH-1][26] = 1;
}

void Game::randMoney()
{
    for (unsigned i = 0; i < money.size(); i++)
    {
        //Make a random position.
        Point randPoint = Point(0,0);

        while (bGrid[randPoint.x][randPoint.y])
            randPoint = Point(money.at(i)->getPos().x+rand()%3-1,money.at(i)->getPos().y);

        //Then set our Money to be there.
        money.at(i)->setPos(randPoint);
    }
}

Game::~Game()
{
    if (sprBack) delete sprBack;
    if (sprDebug) delete sprDebug;
    if (sprDoor) delete sprDoor;

    for (unsigned i = 0; i < guards.size(); i++)
        if (guards.at(i)) delete guards.at(i);
    for (unsigned i = 0; i < thieves.size(); i++)
        if (thieves.at(i)) delete thieves.at(i);

    for (unsigned i = 0; i < money.size(); i++)
        {if (money.at(i)) delete money.at(i);}
    money.clear();

    for (unsigned i = 0; i < bullets.size(); i++)
        {if (bullets.at(i)) delete bullets.at(i);}
    bullets.clear();

    if (message) delete message;

    FORI(MAP_WIDTH)
        delete [] grid[i];
}

void Game::moveDoor()
{
    //OVERRIDE

    if (bS)
    {
        for (unsigned i = 0; i < guards.size(); i++)
            guards.at(i)->sleep = true;
        bS = false;
    }
    if (bA)
    {
        for (unsigned i = 0; i < guards.size(); i++)
            guards.at(i)->sleep = false;
        bA = false;
    }

    dMove = true;

    //Don't move the door if there is an escaping thief.
    for (unsigned i = 0; i < thieves.size(); i++)
    {
        if (thieves.at(i)->getGPos() == Point(dPos.x+1, dPos.y*7+5) && thieves.at(i)->getLifting())
        {
            dMove = false;
            break;
        }
    }


    //The door wont move if the thief is being chased.
    if (dMove)
    {
        if (bUp     && dPos.y > 0)          {dPos.y--; bUp = false; UPDATEHOME;}
        if (bLeft   && dPos.x > 2)          {dPos.x--; bLeft = false; UPDATEHOME;}
        if (bDown   && dPos.y < 3)          {dPos.y++; bDown = false; UPDATEHOME;}
        if (bRight  && dPos.x < MAP_WIDTH-5){dPos.x++; bRight = false; UPDATEHOME;}

        for (unsigned i = 0; i < thieves.size(); i++)
            thieves.at(i)->setHome(Point(dPos.x+1, dPos.y*7+5));
    }

    //Spawn the thief
    if (thieves.size() < 5)
    {
        if (bSpace)
        {
            thieves.push_back(new Thief(dPos.x+1, dPos.y*7+5, money, "data/spr_thief.png"));
            bSpace = false;
        }

        if (bAlt)
        {
            thieves.push_back(new Thief(dPos.x+1, dPos.y*7+5, money, "data/spr_super_thief.png", 25));
            bAlt = false;
        }
    }

    //Spawn a guard.
    if (guards.size() < 5 && bCtrl)
    {
        bCtrl = false;
        guards.push_back(new Guard(dPos.x+1, dPos.y*7+5, money, "data/spr_enemy.png", bullets));
    }
}

void Game::draw()
{
    if (sprBack) sprBack->draw();
    if (message) message->step();

    if (sprDoor)
    {
        moveDoor();
        sprDoor->setPos(dPos.x*GRID_WIDTH, dPos.y*GRID_HEIGHT*7+GRID_HEIGHT*3);
        sprDoor->draw(false, 1-dMove);
    }

    //Temp debug
    /*if (DEBUG && sprDebug)
    {
        for (unsigned i = 0; i < MAP_WIDTH; i++)
            for (unsigned j = 0; j < MAP_HEIGHT; j++)
                if (!bGrid[i][j]) sprDebug->drawCopy(false, 1, i*GRID_WIDTH, j*GRID_HEIGHT);
    }*/

    for (unsigned i = 0; i < money.size(); i++)
        {if (money.at(i)) money.at(i)->draw();}

    for (unsigned i = 0; i < bullets.size(); i++)
        {
            if (bullets.at(i))
            {
                //Remove dead bullets.
                if (bullets.at(i)->dead)
                {
                    delete bullets.at(i);
                    bullets.erase(bullets.begin() + i);
                    i--;
                    continue;
                }
                else
                bullets.at(i)->draw();
            }
        }

    for (unsigned i = 0; i < guards.size(); i++)
        if (guards.at(i)) guards.at(i)->draw();
    for (unsigned i = 0; i < thieves.size(); i++)
    {
        // Don't draw dead thieves.
        if (thieves.at(i) && !thieves.at(i)->dead)
            thieves.at(i)->draw();

        if (thieves.at(i)->dead)
        {
            delete thieves.at(i);
            thieves.erase(thieves.begin() + i);
            i--;
        }
    }
}

