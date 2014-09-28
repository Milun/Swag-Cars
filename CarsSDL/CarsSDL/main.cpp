#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <stdio.h>
#include <string>

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
{
	//Holds offsets
	SDL_Rect offset;

	//Get offsets
	offset.x = x;
	offset.y = y;

	//Blit
	SDL_BlitSurface(source, clip, destination, &offset);
}

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << SDL_GetError();
		return 1;
	}

	if (TTF_Init() == -1)
	{
		std::cout << SDL_GetError();
		return 1;
	}

	int imgFlags = IMG_INIT_PNG;
	
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << IMG_GetError();
//		return 1;
	}
	
	SDL_Window* window = SDL_CreateWindow("Font Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 400, SDL_WINDOW_SHOWN);

	SDL_Surface* screen = SDL_GetWindowSurface(window);


	TTF_Font* font = TTF_OpenFont("lazy.ttf", 28);

	SDL_Surface* testImage = IMG_Load("spr_car.png");



	if (testImage == nullptr)
	{
		//fuck;
		std::cout << "fuck";
	}

	SDL_Color textColor = { 110, 10, 10 };

	SDL_Surface* message = TTF_RenderText_Solid(font, "The quick brown fox jumps over the lazy dog", textColor);

	bool quitGame = false;

	SDL_Rect offset;
	offset.x = 40;
	offset.y = 40;

	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	while (!quitGame)
	{
		SDL_FillRect(screen, NULL, 0xffffff);

		// Detect input
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT) // Detect if the X (close) button is pressed.
				quitGame = true;
		}

		offset.y++;

		//apply_surface(0, 150, message, screen);
		apply_surface(100, 100, testImage, screen);

		SDL_BlitSurface(message, NULL, screen, &offset);
			

		SDL_UpdateWindowSurface(window);
		//SDL_GL_SwapWindow(window);

		SDL_Delay(1);
	}

	int test;
	std::cin >> test;
	
	//Quit SDL
	SDL_FreeSurface(message);
	SDL_FreeSurface(testImage);

	//Close the font that was used
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();

	return 0;
}