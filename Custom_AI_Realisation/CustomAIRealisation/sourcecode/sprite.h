/**
 * Draws a sprite to the screen and animates.
**/

#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <SDL.h>
#include <SDL_image.h>

class Sprite
{
private:
    const unsigned cols, rows;  //How many sprites per sheet
    unsigned w, h;              //The dimensions of the image
    int x, y;

    unsigned frame, timer;
    bool offset;

    SDL_Surface* img;

public:
    Sprite(std::string name, unsigned _cols, unsigned _rows);
    ~Sprite();

    //Return true when the sprite is about to change.
    bool draw(bool flip = false, unsigned s = 0);       //Draws a static sprite.
    bool drawCopy(bool flip = false, unsigned s = 0,    //Draws a copy of the current sprite at a position.
                  unsigned _x = 0, unsigned _y = 0);
    bool draw(bool flip, unsigned s,
              unsigned e, int spe, unsigned off = 0);       //Draws an animated sprite.

    inline void reset(){frame = timer = offset = 0;};   //Needs to be called whenever the sprite changes.
    inline void setPos(int _x, int _y){x = _x; y = _y;};
};

#endif // SPRITE_H
