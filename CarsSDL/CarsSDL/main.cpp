#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << SDL_GetError();
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Font Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 400, SDL_WINDOW_SHOWN);

	SDL_Surface* screen = SDL_GetWindowSurface(window);
	TTF_Font* font = TTF_OpenFont("lazy.ttf", 28);
	SDL_Color textColor = { 110, 255, 0 };

	SDL_Surface* message = TTF_RenderText_Solid(font, "The quick brown fox jumps over the lazy dog", textColor);

	SDL_Rect offset;
	offset.x = 40;
	offset.y = 40;

	SDL_BlitSurface(message, NULL, screen, &offset);

	int test;
	std::cin >> test;
	
	return 0;
}