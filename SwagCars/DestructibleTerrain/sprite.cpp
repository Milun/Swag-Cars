#include "sprite.h"

Sprite::Sprite(char filename[])
{
	// Load our sprite to convert into texture.
	SDL_Surface *spr = IMG_Load(filename);

	if (spr)
	{
		// Record the size of our sprite:
		size = Point (spr->w, spr->h);
		
		// Load the texture
		loadTexture(spr, tex);

		// And we don't need our sprite anymore.
		SDL_FreeSurface(spr);
	}
}

void Sprite::loadTexture(SDL_Surface* spr, GLuint& _tex)
{
	// Store information about the texture.
	GLenum tex_format;

	// Analyse the type of image we are working with.
	GLint colors = spr->format->BytesPerPixel;

	if (colors == 4)
		if (spr->format->Rmask == 0x000000ff)
			tex_format = GL_RGBA;
		else
			tex_format = GL_BGRA;
	else if (colors == 3)
		if (spr->format->Rmask == 0x000000ff)
			tex_format = GL_RGB;
		else
			tex_format = GL_BGR;
	else
		return;

	// Create a texture.
	glGenTextures(1, &_tex);
	glBindTexture(GL_TEXTURE_2D, _tex);

	// Allow it to be stretched.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	// Describe how we want the texture to be stored.
	glTexImage2D(	GL_TEXTURE_2D,	// Type
					0,				// Level of detail
					colors,			// Amount of colors
					spr->w, spr->h,	// Dimensions
					0,				// "This value must be 0"
					tex_format,		// Format of the pixel data
					GL_UNSIGNED_BYTE,	// Type of pixel data
					spr->pixels);	// Pointer to image data
}

void Sprite::draw(Point pos, Point scale)
{
	Point s = scale;
	if (s.x == s.y && s.x == 0) s = size;

	// Prevent color blending.
	setColor(createColor(255,255,255));

	// State what texture will the following code affect.
	glBindTexture(GL_TEXTURE_2D, tex);

	// Draw the texture as a plane (can allow for distortion even).
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0);
		glVertex2f(pos.x, pos.y + s.y);

		glTexCoord2i(1, 0);
		glVertex2f(pos.x + s.x, pos.y + s.y);

		glTexCoord2i(1, 1);
		glVertex2f(pos.x+s.x, pos.y);

		glTexCoord2i(0, 1);
		glVertex2f(pos.x, pos.y);
	glEnd();
}

/**
 * Draws a sprite with transparency enabled (like a .png)
 */
void Sprite::drawAlpha(Point pos, Point scale)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	draw(pos, scale);
}

Sprite::~Sprite()
{
	glDeleteTextures(1, &tex);
}