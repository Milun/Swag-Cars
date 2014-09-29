#ifndef GAME_H
#define GAME_H

#include "car.h"
#include "sprite.h"
#include "global.h"

class Game
{
private:
	Car *cars[6];
	Sprite *spr;

	int currentCar;

public:
	Game();
	~Game();

	void Draw();
};

#endif