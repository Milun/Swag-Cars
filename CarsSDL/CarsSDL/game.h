#ifndef GAME_H
#define GAME_H

#include "car.h"
#include "sprite.h"
#include "text.h"
#include "global.h"

#include <map>
#include <string>

class Game
{
private:
	std::map<Car*,int> cars; //Each car and how long it has been waiting
	Car* currentCar = nullptr;

	Sprite *spr;
	Text *text;

	typedef std::map<Car*, int>::iterator it_type;
	it_type iterator = cars.begin();

	int profit = 0;
	
	int timeWasted = 0;
	int noOfWaitingCars = 0;

	int maxWaitTime = 1000;

	bool IsNextInterval();

public:
	Game();
	Car* NextCarToCharge();
	void ChargeCars();
	std::string ThousandString(std::string pass);
	~Game();

	void Draw();
};

#endif