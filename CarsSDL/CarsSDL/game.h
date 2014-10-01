#ifndef GAME_H
#define GAME_H

#include "car.h"
#include "sprite.h"
#include "global.h"
#include <map>

class Game
{
private:
	std::map<Car*,int> cars; //Each car and how long it has been waiting
	Sprite *spr;

	typedef std::map<Car*, int>::iterator it_type;
	it_type iterator = cars.begin();

	int currentCar;

public:
	Game();
	Car* NextCarToCharge();
	~Game();

	void Draw();
};

#endif