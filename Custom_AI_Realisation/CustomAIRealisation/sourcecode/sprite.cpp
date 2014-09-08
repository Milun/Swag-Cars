#include "sprite.h"
#include <math.h>

#include "global.h"

Sprite::Sprite(std::string name, unsigned _cols, unsigned _rows) : cols(_cols), rows(_rows)
{
    //Load the image
    char *temp        = new char[name.size()+1];
    temp[name.size()] = 0;
    memcpy(temp, name.c_str(), name.size());
    img               = IMG_Load(temp);

    //Determine the size.
    w = unsigned(img->w/cols);
    h = unsigned(img->h/rows);

    //Set default position.
    setPos(0,0);

    //Set the default helper variables.
    reset();
}

Sprite::~Sprite()
{
    //Free the graphic.
    if (img) SDL_FreeSurface(img);
}

bool Sprite::draw(bool flip, unsigned s)
{
    if (img && s >= 0)
    {
        //Flip it automatically.
        SDL_Rect crop = {s%cols*w, int(floor(s/cols))*h*2+h*flip, w, h};
        SDL_Rect des  = {x, y, img->w, img->h};
        SDL_BlitSurface(img, &crop, gbScreen, &des);
    }

    return false;
}

bool Sprite::drawCopy(bool flip, unsigned s, unsigned _x, unsigned _y)
{
    if (img && s >= 0)
    {
        //Flip it automatically.
        SDL_Rect crop = {s%cols*w, int(floor(s/cols))*h*2+h*flip, w, h};
        SDL_Rect des  = {_x, _y, img->w, img->h};
        SDL_BlitSurface(img, &crop, gbScreen, &des);
    }

    return false;
}

bool Sprite::draw(bool flip, unsigned s, unsigned e, int spe, unsigned off)
{
    bool shift = false; //Is set to true on the last frame before the animation shifts.

    if (spe != 0) //Prevent 0 speed animation.
    {
        //Set the offset of the frame (if it's legal).
        if (!offset)
        {
            if (abs(e-s) >= off) frame = off;
            offset = true;
        }

        //Animate every few frames.
        if (timer == abs(spe))
        {
            timer = 0;

            if (frame < e-s) frame++;//Loop the animation.
            else             frame = 0;
        }
        else timer++;

        //Return
        if (timer == abs(spe)-1) shift = true;
    }

    if (spe >= 0) draw(flip, s+frame);    //Draw the sprite if it animates forward.
    else          draw(flip, e-frame);    //If it's reversed.

    return shift;
}

