#include <string>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>

#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <gl/GL.h>

#include "global.h"
#include "car.h"

const unsigned SCREEN_WIDTH = 800;
const unsigned SCREEN_HEIGHT = 600;
#define FPS 60

bool gKeyLeft;
bool gKeyRight;
bool gKeyUp;
bool gKeyDown;
bool gKeySpace;
bool gMouseLeft;
bool gMouseRight;
bool gKeyA;
SDL_Window *gScreen;

Point gMouse;

bool gDebug = false;		// Show collisions.
bool gDelay = false;		// Delay on collision.

void initGL()
{
	// Initiate OpenGL
	glClearColor(0.0, 0.7, 0.0, 0.0); 
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D	(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);
	glEnable(GL_TEXTURE_2D);
}

int main(int argc, char* args[])
{
	// Initialise SDL
	SDL_Init( SDL_INIT_EVERYTHING );

	// Start video
	gScreen = SDL_CreateWindow("Intelligent Systems",
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								SCREEN_WIDTH, SCREEN_HEIGHT,
								SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);


	// Start openGL
	initGL();

	// Seed Random
	std::srand(time(NULL));

	gKeyLeft = gKeyRight = gKeySpace = false;

	bool quitGame	= false;
	bool keyD		= false;
	bool keyS		= false;

	int frame = 0;
	int lastTicks = SDL_GetTicks();
	int ticks;

	std::vector<Car*> cars;

	for (unsigned i = 0; i < 6; i++)
	{
		cars.push_back(new Car(i* floor( (float)SCREEN_WIDTH / 6.0f ) ));
	}

	while (!quitGame)
	{
		// White background
		glClearColor(255.0f, 255.0f, 255.0f, 1.0);

		// Detect input
		SDL_Event ev;
		while (SDL_PollEvent(&ev) )
		{
			if (ev.type == SDL_QUIT) // Detect if the X (close) button is pressed.
				quitGame = true;

			if (ev.type == SDL_MOUSEMOTION)
			{
				gMouse.x = ev.motion.x;
				gMouse.y = SCREEN_HEIGHT-ev.motion.y;	// Flip to math coordinates.
			}

			if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT)
				gMouseLeft = true;

			if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT)
				gMouseLeft = false;

			if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_RIGHT)
				gMouseRight = true;

			if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_RIGHT)
				gMouseRight = false;

			if (ev.type == SDL_KEYDOWN)
			{
				switch (ev.key.keysym.sym)
				{
					case SDLK_LEFT:		gKeyLeft = true; break;
					case SDLK_RIGHT:	gKeyRight = true; break;
					case SDLK_UP:		gKeyUp = true; break;
					case SDLK_DOWN:		gKeyDown = true; break;
					case SDLK_SPACE:	gKeySpace = true; break;
					case SDLK_d:		keyD = true; break;
					case SDLK_a:		gKeyA = true; break;
					case SDLK_s:		keyS = true; break;
					default:	break;
				}
			}

			if (ev.type == SDL_KEYUP)
			{
				switch (ev.key.keysym.sym)
				{
					case SDLK_LEFT:		gKeyLeft = false; break;
					case SDLK_RIGHT:	gKeyRight = false; break;
					case SDLK_UP:		gKeyUp = false; break;
					case SDLK_DOWN:		gKeyDown = false; break;
					case SDLK_SPACE:	gKeySpace = false; break;
					case SDLK_d:		keyD = false; break;
					case SDLK_a:		gKeyA = false; break;
					case SDLK_s:		keyS = false; break;
					default:	break;
				}
			}
		}

		// Update
		if (keyD)
		{
			gDebug = (!gDebug);
			keyD = false;
		}

		if (keyS)
		{
			gDelay = (!gDelay);
			keyS = false;
		}

		

		// Render
		glClear( GL_COLOR_BUFFER_BIT );

		for (unsigned i = 0; i < cars.size(); i++)
		{
			cars.at(i)->update();
		}

		SDL_GL_SwapWindow(gScreen);

		glFlush();

		// Limit FPS.
		frame++;
		ticks = SDL_GetTicks() - lastTicks;
		lastTicks = SDL_GetTicks();

		if (ticks < 1000/FPS)
			SDL_Delay( (1000/FPS) - ticks);
	}

	//Quit SDL

	for (unsigned i = 0; i < cars.size(); i++)
	{
		delete cars.at(i);
	}

	SDL_Quit();
	return 0;
}