#ifndef GAME_H
#define GAME_H

#include "car.h"
#include "sprite.h"
#include "text.h"
#include "global.h"
#include "graph.h"
#include "button.h"

#include <map>
#include <string>
#include <vector>

using namespace std;

class Game
{
private:
	Car* currentCar = nullptr;

	Button* btnPause;
	Button* btnPlay;

	Sprite *spr;
	Sprite *sprBar;
	Text *text;
	Text *textTime;

	SDL_Color	color[10];

	vector<Car*> cars; //Each car and how long it has been waiting
	vector<Car*> carsWaiting;
	vector<Car*> carsByDue;
	vector<Car*> carsLate;
	vector<Car*> carsFinal;
	vector<Car*> carsFinalDraw;

	double dueLateness = 0.0;
	int dueLatenessAmm = 0;
	double hodgeLateness = 0.0;
	int hodgeLatenessAmm = 0;

	double startTime = 0;
	double finishTime = 0;

	//bool optimal;

		//early = list of cars
		//late = list of cars
		//optimum = boolean

	//Graph *

	long int profit = 0;

	long double timeWasted = 0;
	long double waitingSinceLast = 0;

	int noOfWaitingCars = 0;

	double maxWaitTime = 30;

	void UpdateWaitingGraph(int waitingThisInterval);
	
	void Sort();
	void CalcWaitingTime();

	void DrawSchedule();

	void UpdateButtons();

public:
	Game();

	void ChargeCars();
	std::string ThousandString(std::string pass);
	~Game();

	void Draw();
};

#endif