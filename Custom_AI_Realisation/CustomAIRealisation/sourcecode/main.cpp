#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include <math.h>
#include <string>

#include "game.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

//Global Values
SDL_Surface* gbScreen;
bool bSpace;
bool bCtrl;
bool bDown;
bool bRight;
bool bUp;
bool bLeft;
bool bS;
bool bA;
bool bAlt;
std::string bDebug;
std::string bDebug2;
char **bGrid;

bool DEBUG = true;

int main(int argc, char* args[])
{
    /// INITIATE SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    // Set up the screen
    gbScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );

    //No mouse
    SDL_ShowCursor(0);

    //Set up the window
    SDL_WM_SetCaption("BAHK POB","BAHK POB");
    SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);


    /// CONTROLS

    SDL_Joystick* joystick_JOYSTICK = SDL_JoystickOpen(0);

    int quit     = 0;
    bSpace  = false;
    bDown   = false;
    bRight  = false;
    bUp     = false;
    bLeft   = false;

    /// FONT

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("data/DejaVuSans.ttf",12);


    /// MAKE THE GAME

    Game *game = new Game();


	/// GAME LOOP

	while(quit==0)///begin main game loop
	{

		// Clear the screen and buffers
		SDL_FillRect(gbScreen,NULL,SDL_MapRGBA(gbScreen->format, 0, 0, 0, 255));

		// Keyboard input
		SDL_Event event_EVENT;

		while(SDL_PollEvent(&event_EVENT))
		{
			if(event_EVENT.type == SDL_KEYDOWN)
			{
                if(event_EVENT.key.keysym.sym==SDLK_DOWN)   {bDown  = true;}
                if(event_EVENT.key.keysym.sym==SDLK_RIGHT)  {bRight = true;}
                if(event_EVENT.key.keysym.sym==SDLK_UP)     {bUp    = true;}
                if(event_EVENT.key.keysym.sym==SDLK_LEFT)   {bLeft  = true;}

                if(event_EVENT.key.keysym.sym==SDLK_ESCAPE) {quit   = true;}
                if(event_EVENT.key.keysym.sym==SDLK_d)      {DEBUG  = abs(1-DEBUG);}
                if(event_EVENT.key.keysym.sym==SDLK_s)      {bS     = true;}
                if(event_EVENT.key.keysym.sym==SDLK_a)      {bA     = true;}
                if(event_EVENT.key.keysym.sym==SDLK_SPACE)  {bSpace = true;}
                if(event_EVENT.key.keysym.sym==SDLK_LCTRL)  {bCtrl = true;}
                if(event_EVENT.key.keysym.sym==SDLK_LALT)   {bAlt = true;}
			}

			if(event_EVENT.type == SDL_KEYUP)
			{
				if(event_EVENT.key.keysym.sym==SDLK_DOWN)   {bDown  = false;}
				if(event_EVENT.key.keysym.sym==SDLK_RIGHT)  {bRight = false;}
				if(event_EVENT.key.keysym.sym==SDLK_UP)     {bUp    = false;}
				if(event_EVENT.key.keysym.sym==SDLK_LEFT)   {bLeft  = false;}
			}
		}

        /// DRAW OUR GAME

        game->draw();


        /// DRAW THE STATES

        for (unsigned i = 0; i < game->guards.size(); i++)
        {
            char text[1024];
            sprintf(text, "Guard(%i):    %s", i, game->guards.at(i)->myState.c_str());

            SDL_Color white = {255,255,255,255};

            ///Create a surface for the text.
            SDL_Surface* textS = TTF_RenderText_Solid(font, text, white);

            //Draw it if it exists
            if(textS)
            {
                SDL_Rect label_RECT = {10, 10+(20*i), textS->w, textS->h};
                SDL_BlitSurface(textS, NULL, gbScreen, &label_RECT);
                SDL_FreeSurface(textS);
            }
        }

        for (unsigned i = 0; i < game->thieves.size(); i++)
        {
            char text[1024];
            sprintf(text, "Thief(%i):    %s", i, game->thieves.at(i)->myState.c_str());

            SDL_Color white = {255,255,255,255};

            ///Create a surface for the text.
            SDL_Surface* textS = TTF_RenderText_Solid(font, text, white);

            //Draw it if it exists
            if(textS)
            {
                SDL_Rect label_RECT = {160, 10+(20*i), textS->w, textS->h};
                SDL_BlitSurface(textS, NULL, gbScreen, &label_RECT);
                SDL_FreeSurface(textS);
            }
        }

        // Show the buffer
        SDL_Flip(gbScreen);


        // Limit FPS
        SDL_Delay(10);

	} ///END LOOP

    //Send a message to cmd
    fprintf(stdout,"Game Over!");

    //Release memory
    if (game) delete game;
    if (gbScreen) SDL_FreeSurface(gbScreen);
    TTF_CloseFont(font);

    //Release controls
    if(joystick_JOYSTICK!=NULL)SDL_JoystickClose(joystick_JOYSTICK);

    /// QUIT
	SDL_Quit();

	return 0;
}




