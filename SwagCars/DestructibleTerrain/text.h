#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>

#include "global.h" 

class Text
{
private:
	SDL_Surface *surface = NULL;
	TTF_Font	*font = NULL;

	GLuint tex;		// The texture (image) for the sprite to use.

	Point pos;

public:
	Text();
	~Text();

	void Update(Point s);
};

#endif