//nD SDK Example 1

#include <SDL/SDL.h>

//font support
#include <SDL/SDL_ttf.h>

//sound support
#include <SDL/SDL_mixer.h>

//image loading
#include <SDL/SDL_image.h>


///===========================================================================================================================
int main(int argc, char* args[])
{///===========================================================================================================================


    //------------------------------------------
    //init sdl
    //------------------------------------------
        SDL_Init(SDL_INIT_EVERYTHING);

    //------------------------------------------
    //init sound
    //------------------------------------------

        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

        Mix_AllocateChannels(32);

        //load a sound
        Mix_Chunk* action_WAV = Mix_LoadWAV("data/chime.wav");

        //load a music file
        Mix_Music*  background_MUS = Mix_LoadMUS("data/clicktrack.s3m");

       //play some background music
		Mix_PlayMusic(background_MUS,-1);


    //------------------------------------------
    //init video
    //------------------------------------------
        SDL_Surface* screen_SURFACE = SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );

        //don't show the mouse cursor
        SDL_ShowCursor(0);

        //set the window caption and icon
        SDL_WM_SetCaption("nD SDK Example 1","nD SDK Example 1");
        SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);

    //------------------------------------------
    //init controls
    //------------------------------------------

        SDL_Joystick* joystick_JOYSTICK = SDL_JoystickOpen(0);

        int quit=0;
        int action_pressed=0;
        int down_pressed=0;
        int right_pressed=0;
        int up_pressed=0;
        int left_pressed=0;


    //------------------------------------------
    //init font
    //------------------------------------------

        TTF_Init();
        TTF_Font *font_16px =TTF_OpenFont("data/DejaVuSans.ttf",16);



    //load our logo
    SDL_Surface* logo_SURFACE = IMG_Load("data/logo.png");

    //set his initial position
    int guy_x = 320 / 2;
    int guy_y = 240 / 2;

    //this holds the total ticks passed for the last loop
    int last_ticks=SDL_GetTicks();



	///================================================================================================================
	while(quit==0)///begin main game loop
	{///================================================================================================================

		//------------------------------------------
		//clear the screen (fill it with black) and erase framebuffer if using one
		//------------------------------------------
		SDL_FillRect(screen_SURFACE,NULL,SDL_MapRGBA(screen_SURFACE->format, 0, 0, 0, 255));

		//------------------------------------------
		//check for new button events
		//------------------------------------------
		SDL_Event event_EVENT;

		while(SDL_PollEvent(&event_EVENT))
		{
			if(event_EVENT.type == SDL_KEYDOWN)
			{
                if(event_EVENT.key.keysym.sym==SDLK_DOWN){down_pressed=1;}
                if(event_EVENT.key.keysym.sym==SDLK_RIGHT){right_pressed=1;}
                if(event_EVENT.key.keysym.sym==SDLK_UP){up_pressed=1;}
                if(event_EVENT.key.keysym.sym==SDLK_LEFT){left_pressed=1;}

                if(event_EVENT.key.keysym.sym==SDLK_ESCAPE){quit=1;}
                if(event_EVENT.key.keysym.sym==SDLK_SPACE){action_pressed=1;}
			}

			if(event_EVENT.type == SDL_KEYUP)
			{
				if(event_EVENT.key.keysym.sym==SDLK_DOWN){down_pressed=0;}
				if(event_EVENT.key.keysym.sym==SDLK_RIGHT){right_pressed=0;}
				if(event_EVENT.key.keysym.sym==SDLK_UP){up_pressed=0;}
				if(event_EVENT.key.keysym.sym==SDLK_LEFT){left_pressed=0;}
			}
		}

        //------------------------------------------
        //play a sound when the space bar is pressed
        //------------------------------------------
            if(action_pressed==1)
            {
                action_pressed=0;
                Mix_PlayChannel(-1, action_WAV, 0);
            }

        //------------------------------------------
        //set the x and y coordinates for our label when the direction keys are pressed
        //------------------------------------------

            //figure out how many milliseconds have passed since last time
            int ticks_since_last_loop = SDL_GetTicks()-last_ticks;
            last_ticks = SDL_GetTicks();

            //a speed multiplier
            float speed = 0.2;

            //move the logo based on how much time has passed
            int move_amount = ticks_since_last_loop * speed;

            if(up_pressed)guy_y-=move_amount;
            if(down_pressed)guy_y+=move_amount;
            if(left_pressed)guy_x-=move_amount;
            if(right_pressed)guy_x+=move_amount;

        //------------------------------------------
        //draw our logo graphic onscreen
        //------------------------------------------
            //make a rect with x,y,width,height
            SDL_Rect logo_RECT = {guy_x,guy_y,logo_SURFACE->w,logo_SURFACE->h};

            SDL_BlitSurface(logo_SURFACE,NULL,screen_SURFACE,&logo_RECT);

        //------------------------------------------
        //draw the label onscreen
        //------------------------------------------
            //make an array of chars to hold our text
            char text[1024];

            //print our coord variables into it
            sprintf(text,"x:%d y:%d",guy_x,guy_y);

            //make a color
            SDL_Color white_COLOR = {255,255,255,255};

            //draw our label text into a new surface
            SDL_Surface* text_SURFACE=TTF_RenderText_Solid(font_16px,text,white_COLOR);

            //make sure the surface exists
            if(text_SURFACE!=NULL)
            {
                //make a rectangle to send into SDL_BlitSurface
                SDL_Rect label_RECT = {0,240-text_SURFACE->h,text_SURFACE->w,text_SURFACE->h};

                //blit the label text surface onto our screen buffer
                SDL_BlitSurface(text_SURFACE,NULL,screen_SURFACE,&label_RECT);

                //delete our text surface, since we dont need it anymore
                SDL_FreeSurface(text_SURFACE);
            }

        //------------------------------------------
        //show our buffer on the screen
        //------------------------------------------
            SDL_Flip(screen_SURFACE);


        //------------------------------------------
        //wait 10 ms
        //------------------------------------------
            SDL_Delay(10);

    ///================================================================================================================
	}///end main game loop
    ///================================================================================================================

    //send a message to stdout (should be in /bin/Target/stdout.txt)
    fprintf(stdout,"Shutting Down!");

    //------------------------------------------
    //close graphics
    //------------------------------------------

        if(screen_SURFACE!=NULL)SDL_FreeSurface(screen_SURFACE);

        if(logo_SURFACE!=NULL)SDL_FreeSurface(logo_SURFACE);

    //------------------------------------------
    //close controls
    //------------------------------------------

        if(joystick_JOYSTICK!=NULL)SDL_JoystickClose(joystick_JOYSTICK);

    //------------------------------------------
    //close sound
    //------------------------------------------
		if(Mix_PlayingMusic())
		{
			Mix_FadeOutMusic(1000);
			Mix_FreeMusic(background_MUS);
		}

		Mix_CloseAudio();
		Mix_FreeChunk(action_WAV);


    //------------------------------------------
    //close font
    //------------------------------------------
        TTF_CloseFont(font_16px);

	SDL_Quit();

	return 0;
}



