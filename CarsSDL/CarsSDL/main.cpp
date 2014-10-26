#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <stdio.h>
#include <string>

#include <random>
#include <time.h>
#include "game.h"

Game *game;
SDL_Surface *screen;

long int gMillis = 0;
long int gTime = 0;
int gFramesToSeconds = 100;

bool delay = false;

#include "global.h"
#include "button.h"

double GetValueFromMenu(int startX, int startY, int xGap, int yGap);

bool init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << SDL_GetError();
		return true;
	}

	if (TTF_Init() == -1)
	{
		std::cout << SDL_GetError();
		return true;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << IMG_GetError();
		return true;
	}

	return false;
}

int main(int argc, char *argv[])
{
	if (init())
	{
		return 1;
	}
	
	srand(time(NULL));

	SDL_Window* window = SDL_CreateWindow("Font Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1200, 800, SDL_WINDOW_SHOWN);
	screen = SDL_GetWindowSurface(window);

	game = new Game();
	bool quitGame = false;

	//Car Menu
	//MILTON you can then pass these values into Game or whatever class will assign them to the cars
	//Might also want to write what you're setting so the user knows whats going on
	double chargeRate = GetValueFromMenu(0, 0, 50, 50);
	double chargeMax = GetValueFromMenu(0, 0, 50, 50);
	double chargeUse = GetValueFromMenu(0, 0, 50, 50);

	while (!quitGame)
	{
		clickedThisFrame = false;

		SDL_FillRect(screen, NULL, 0xffffff);
		IncrementTime();

		// Detect input
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT) // Detect if the X (close) button is pressed.
				quitGame = true;

			switch (ev.type)
			{
			case SDL_QUIT:
				quitGame = true;
				break;

			case SDL_KEYDOWN:
				printf("Key press detected\n");
				break;

			case SDL_KEYUP:
				printf("Key release detected\n");
				break;
			
			case SDL_MOUSEBUTTONDOWN:
				clickedThisFrame = true;
				clickPosX = ev.motion.x;
				clickPosY = ev.motion.y;
				break;

			default:
				break;
			}
		}

		game->Draw();

		SDL_UpdateWindowSurface(window);
		SDL_Delay(0);
	}

	//Quit SDL
	delete game;

	//Close the font that was used
	TTF_Quit();
	SDL_Quit();

	return 0;
}

double GetValueFromMenu(int startX, int startY, int xGap, int yGap)
{
	//Custom car menu
	Sprite*	increaseSprite = new Sprite("increseButton.png");
	Button* increase = new Button(startX + xGap, startY, 10, 10, increaseSprite);

	Sprite* decreaseSprite = new Sprite("decreaseButton.png");
	Button* decrease = new Button(startX, startY, 10, 10, decreaseSprite);

	Sprite*	acceptSprite = new Sprite("acceptButton.png");
	Button* accept = new Button(startX + (xGap/2), startY + yGap, 10, 10, acceptSprite);

	bool set = false;
	double value = 0; //value is what will be given to the car
	while (set == false)
	{
		if (increase->ClickedOnThisFrame())
		{
			value += 1;
		}
		else if (decrease->ClickedOnThisFrame())
		{
			value -= 1;
		}
		else if (accept->ClickedOnThisFrame())
		{
			set = true;
		}
	}

	return value;
}