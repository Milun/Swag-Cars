#include "text.h"
#include <iostream>

Text::Text()
{
	font = TTF_OpenFont("arial.ttf", 28);
	
	if (!font)
	{
		std::cout << "STOP";
	}

	// Make a surface
	surface = TTF_RenderText_Solid(font, "The quick brown fox jumps over the lazy dog", createColor(0, 255, 0));

	// Store information about the texture.
	GLenum tex_format;

	// Analyse the type of text we are working with.
	GLint colors;
	if (surface) colors = surface->format->BytesPerPixel;

	if (colors == 4)
		if (surface->format->Rmask == 0x000000ff)
			tex_format = GL_RGBA;
		else
			tex_format = GL_BGRA;
	else if (colors == 3)
		if (surface->format->Rmask == 0x000000ff)
			tex_format = GL_RGB;
		else
			tex_format = GL_BGR;
	else
		return;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, colors, surface->w, surface->h, 0, tex_format, GL_UNSIGNED_BYTE, surface->pixels);

	pos = Point(50, 50);
}

void Text::Update(Point s)
{
	// State what texture will the following code affect.
	glBindTexture(GL_TEXTURE_2D, tex);

	// Draw the texture as a plane (can allow for distortion even).
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex2f(pos.x, pos.y + surface->h);

	glTexCoord2i(1, 0);
	glVertex2f(pos.x + surface->w, pos.y + surface->h);

	glTexCoord2i(1, 1);
	glVertex2f(pos.x + surface->w, pos.y);

	glTexCoord2i(0, 1);
	glVertex2f(pos.x, pos.y);
	glEnd();

	SDL_GL_SwapWindow(gScreen);
}

Text::~Text()
{
	SDL_FreeSurface( surface );
	TTF_CloseFont( font );
}