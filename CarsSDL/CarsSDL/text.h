#ifndef TEXT_H
#define TEXT_H

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

class Text
{
private:
	SDL_Surface *surface = NULL;
	TTF_Font	*font = NULL;
	SDL_Color	color;

	int x, y;

public:
	Text(std::string fontname);
	~Text();

	void Draw(int x, int y, std::string content);
};

#endif