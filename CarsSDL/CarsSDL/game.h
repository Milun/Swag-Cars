#ifndef GAME_H
#define GAME_H

#include "car.h"
#include "sprite.h"
#include "text.h"
#include "global.h"
#include "graph.h"

#include <map>
#include <string>

class Game
{
private:
	std::map<Car*,int> cars; //Each car and how long it has been waiting
	Car* currentCar = nullptr;

	Sprite *spr;
	Text *text;

	//Graph *

	typedef std::map<Car*, int>::iterator it_type;
	it_type iterator = cars.begin();

	long int profit = 0;

	long int timeWasted = 0;
	long int waitingSinceLast = 0;

	int noOfWaitingCars = 0;

	int maxWaitTime = 30;

	void UpdateWaitingGraph(int waitingThisInterval);

public:
	Game();
	Car* NextCarToCharge();
	void ChargeCars();
	std::string ThousandString(std::string pass);
	~Game();

	void Draw();
};

#endif