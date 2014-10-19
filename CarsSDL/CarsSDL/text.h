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
	int			size;

	int x, y;

public:
	Text(std::string fontname, int size = 16);
	~Text();

	void Draw(int x, int y, std::string content, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0);
};

#endif