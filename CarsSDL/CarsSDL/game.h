#ifndef GAME_H
#define GAME_H

#include "car.h"

#include "global.h"

class Game
{
private:
	Car *cars[6];

public:
	Game();
	~Game();

	void Draw();
};

#endif