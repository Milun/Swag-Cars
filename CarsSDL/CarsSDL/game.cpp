#include "game.h"
#include <iostream>

Game::Game()
{
	Game(6);
}

Game::Game(int numCars)
{
	int max = 6;

	if (numCars > max)
	{
		numCars = max;
	}
	else if (numCars <= 0)
	{
		numCars = 1;
	}

	spr = new Sprite("spr_charge.png");
	sprBar = new Sprite("spr_bar.png");

	text = new Text("lucon.ttf");
	textTime = new Text("lucon.ttf", 28);

	color[0].r = 255; color[0].g = 0; color[0].b = 0;
	color[1].r = 0; color[1].g = 255; color[1].b = 0;
	color[2].r = 0; color[2].g = 0; color[2].b = 255;
	color[3].r = 255; color[3].g = 150; color[3].b = 0;
	color[4].r = 255; color[4].g = 0; color[4].b = 255;
	color[5].r = 0; color[5].g = 255; color[5].b = 255;
	color[6].r = 0; color[6].g = 0; color[6].b = 0;
	color[7].r = 100; color[7].g = 0; color[7].b = 255;
	color[8].r = 0; color[8].g = 255; color[8].b = 100;
	color[9].r = 255; color[9].g = 0; color[9].b = 100;

	for (unsigned i = 0; i < numCars-1; i++)
	{
		cars.push_back(new Car(i * 135 + 1,
			"spr_car_" + std::to_string(i) + ".png",
			"spr_icon_" + std::to_string(i) + ".png"));
	}
}

void Game::Draw()
{
	spr->Draw(0, 220);

	text->Draw(850, 10, "Profit: $" + ThousandString(std::to_string(profit)) + ".00" );
	text->Draw(850, 40, "Total waiting time caused: " + SecondsToTime(timeWasted) + " (+" + std::to_string(noOfWaitingCars) + ") per second");

	

	textTime->Draw(850, 100, ToTime((int)gTime));
	
	// Always have an array ready of cars waiting.
	carsWaiting.clear();
	for (unsigned i = 0; i < cars.size(); i++)
	{
		if (cars.at(i)->GetChargeMe())
		{
			carsWaiting.push_back(cars.at(i));
		}
	}

	ChargeCars();
	CalcWaitingTime();

	DrawSchedule();
}

void Game::DrawSchedule()
{
	double multi = 620.0f / ((double)finishTime);
	double offset = 0;

	double barOffset = GetTimeDouble() - (double)startTime;

	// Draw text to show it.
	textTime->Draw(10, 470, "By Due:");
	for (unsigned i = 0; i < carsByDue.size(); i++)
	{
		// RYAN! We need to draw a rectangle
		// from:	140 + (offset)*multi
		// to:		140 + (offset + carsByDue.at(i)->GetMaxChargeTime())*multi
		
		//MILTON!
		//My DrawRect function needs to be passed a Surface to draw on, but there's none in this class, not sure what to do

		carsByDue.at(i)->GetIcon()->Draw(180 + (offset + carsByDue.at(i)->GetMaxChargeTime())*multi - 49, 450);
		offset += carsByDue.at(i)->GetMaxChargeTime();
	}

	offset = 0;

	// Draw text to show it.
	textTime->Draw(10, 570, "Hodgson's:");
	for (unsigned i = 0; i < carsFinalDraw.size(); i++)
	{
		carsFinalDraw.at(i)->GetIcon()->Draw(180 + (offset + carsFinalDraw.at(i)->GetMaxChargeTime())*multi - 49, 550);
		offset += carsFinalDraw.at(i)->GetMaxChargeTime();
	}

	sprBar->Draw(160 + barOffset*multi, 446);
}

void Game::Sort()
{
	// Clear the lists for a new sort.
	carsByDue.clear();
	carsLate.clear();
	carsFinal.clear();
	carsFinalDraw.clear();

	finishTime = 0;
	startTime = gTime;

	// Get the total amount of time this will all charge for.
	for (unsigned i = 0; i < carsWaiting.size(); i++)
	{
		finishTime += carsWaiting.at(i)->GetMaxChargeTime();

		printf((std::to_string(carsWaiting.at(i)->GetMaxChargeTime()) + " + ").c_str());
		cout << "\n";
	}

	printf(("Hey: " + std::to_string(finishTime) + "  \n").c_str());

	// Sort cars by the time they are due.
	while (carsWaiting.size() > 0)
	{
		long double minDueTime = 9999999;

		Car* tmp;
		for (unsigned i = 0; i < carsWaiting.size(); i++)
		{
			if (carsWaiting.at(i)->GetDueTime() < minDueTime)
			{
				minDueTime = carsWaiting.at(i)->GetDueTime();
				tmp = carsWaiting.at(i);
			}
		}

		carsByDue.push_back(tmp);
		carsWaiting.erase(find(carsWaiting.begin(), carsWaiting.end(), tmp));
	}

	// Now, go through the sorted cars and find the first car that will be late.
	bool optimal = false;
	vector<Car*> carsByDueTemp = carsByDue;
	while (!optimal)
	{
		long float addTime = 0;

		// Find the first car which will be late.
		for (unsigned i = 0; i < carsByDueTemp.size(); i++)
		{
			addTime += carsByDueTemp.at(i)->GetChargeTime();

			// If this car will be late...
			if (addTime > carsByDueTemp.at(i)->GetDueTime())
			{
				long float maxChargeTime = -1;
				Car* temp = nullptr;

				for (unsigned j = 0; j < i; j++)
				{
					if (carsByDueTemp.at(j)->GetChargeTime() > maxChargeTime)
					{
						temp = carsByDueTemp.at(j);
						maxChargeTime = temp->GetChargeTime();
					}
				}

				// Remove the longest task and put it into another list.
				if (temp)
				{
					carsLate.push_back(temp);
					carsByDueTemp.erase(find(carsByDueTemp.begin(), carsByDueTemp.end(), temp));
				}

				break;
			}

			optimal = true;
		}
	}

	for (unsigned i = 0; i < carsByDueTemp.size(); i++)
	{
		carsFinal.push_back(carsByDueTemp.at(i));
		carsFinalDraw.push_back(carsByDueTemp.at(i));
	}
	for (unsigned i = 0; i < carsLate.size(); i++)
	{
		carsFinal.push_back(carsLate.at(i));
		carsFinalDraw.push_back(carsByDueTemp.at(i));
	}
}

std::string Game::ThousandString(std::string pass)
{
	unsigned int length = strlen(pass.c_str()); 
	std::string output;

	unsigned int offset = length % 3; // Get the comma offset
	for (unsigned int i = 0; i < length; ++i) // Loop each character
	{
		// If our Index%3 == CommaOffset and this isn't first character, add a comma
		if (i % 3 == offset && i)
		{
			output += ','; // Add the comma
		}
		output += pass.at(i);
	}
	
	return output;
}

void Game::ChargeCars()
{
	if (carsWaiting.size() <= 0) return;
	if (carsFinal.size() <= 0) Sort();
	if (carsFinal.size() <= 0) return;

	currentCar = carsFinal.at(0);

	if (currentCar != nullptr && carsFinal.size() > 0)
	{
		if (!currentCar->Charge())
		{
			currentCar->StopCharge();

			carsFinal.erase(find(carsFinal.begin(), carsFinal.end(), currentCar));
			
			// Resort the list if you're done.
			//if (carsEarly.size() <= 0) Sort();
			//currentCar = carsEarly.at(0);

			currentCar = nullptr;
		}
		else
		{
			// Increase profits.
			if (NextSecondInterval())
			profit += 1;

			/*if (currentCar->GetChargeMe() == false)
			{
				cars[currentCar] = 0;
			}*/
		}
	}
}

void Game::CalcWaitingTime()
{
	noOfWaitingCars = 0;

	for (unsigned i = 0; i < cars.size(); i++)
	{
		cars.at(i)->Draw();

		// Cars that are charging do not count as "waiting" (their time is not being wasted afterall).
		if (cars.at(i)->GetChargeMe())
		{
			noOfWaitingCars++;

			// Add to the overall time wasted waiting of all cars.
			if (NextSecondInterval())
			{
				timeWasted++;
				waitingSinceLast++;
			}
		}
	}

	if (NextSecondInterval() && gTime % 30 == 0)
	{
		//UpdateWaitingGraph(waitingSinceLast);
		waitingSinceLast = 0;
	}
}


/*Car* Game::NextCarToCharge()
{
	Car* car = nullptr;

	iterator = cars.begin();
	car = iterator->first;
	for (iterator; iterator != cars.end(); iterator++)
	{
		if (iterator->second > cars[car])
		{
			car = iterator->first;
		}
	}

	return car;
}*/

/*Car* Game::NextCarToCharge()
{
	// Cars that have waited for too long are our highest priority.
	if (currentCar && currentCar->GetWaitTime() > maxWaitTime)
	{
		return currentCar;
	}

	// Only swap if the car has been charging too long (temporary).
	//if (!currentCar || currentCar->GetChargeTime() > 500)
	{

		iterator = cars.begin();

		// Go through all cars:
		for (iterator; iterator != cars.end(); iterator++)
		{
			// Ignore the current car. Don't count cars that aren't waiting.
			if ( iterator->first == currentCar ||
				!iterator->first->GetChargeMe() ) continue;

			// For starters, stop charging.
			if (currentCar) currentCar->StopCharge();

			// If we don't have a current car, give us one.
			currentCar = iterator->first;

			// If a car has exceeded the wait time, switch.
			if (iterator->first && iterator->first->GetWaitTime() >= maxWaitTime)
			{
				currentCar = iterator->first;
				return currentCar;
			}
		}

	}

	return currentCar;
}*/

Game::~Game()
{
	for (unsigned i = 0; i < cars.size(); i++)
	{
		delete cars.at(i);
	}

	delete spr;
	delete sprBar;
	delete text;
	delete textTime;
}