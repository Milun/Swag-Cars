#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <string>

#include "global.h"

class Sprite
{
private:
	Point size;			// Dimensions of the image.
	
protected:
	GLuint tex;			// The texture (image) for the sprite to use.
	void loadTexture(SDL_Surface* spr, GLuint& _tex);

public:
	Sprite(char filename[]);
	~Sprite();

	void draw(Point pos, Point scale = Point(0,0));
	void drawAlpha(Point pos, Point scale = Point(0,0));
	inline Point getSize() const {return size;}


};

#endif