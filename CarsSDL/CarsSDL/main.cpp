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
bool gSecond = false;
long int gSeconds = 0;
long int gLastMilli = 0;

#include "global.h"

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

	SDL_Window* window = SDL_CreateWindow("Font Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1200, 600, SDL_WINDOW_SHOWN);
	screen = SDL_GetWindowSurface(window);

	game = new Game();
	bool quitGame = false;

	while (!quitGame)
	{
		SDL_FillRect(screen, NULL, 0xffffff);
		CountSeconds();

		// Detect input
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT) // Detect if the X (close) button is pressed.
				quitGame = true;
		}

		game->Draw();

		SDL_UpdateWindowSurface(window);
		SDL_Delay(1);
	}

	//Quit SDL
	delete game;

	//Close the font that was used
	TTF_Quit();
	SDL_Quit();

	return 0;
}