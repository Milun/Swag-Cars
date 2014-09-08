#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL.h>
#include <string>

#define GRID_WIDTH 32
#define GRID_HEIGHT 20
#define MAP_WIDTH 25
#define MAP_HEIGHT 31
#define SPRITEX_OFF -(36-GRID_WIDTH)/2
#define SPRITEY_OFF -36+GRID_HEIGHT
#define SPEED 4

extern SDL_Surface* gbScreen;
extern bool bSpace;
extern bool bCtrl;
extern bool bDown;
extern bool bRight;
extern bool bUp;
extern bool bLeft;
extern std::string bDebug;
extern std::string bDebug2;
extern bool bS;
extern bool bA;
extern bool bAlt;

extern char **bGrid; //Temp and messy.

extern bool DEBUG;

#endif // GLOBAL_H
