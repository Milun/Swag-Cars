#include "global.h"

void drawPixel (Point pos)
{  
    glBegin(GL_POINTS);
		glVertex2i(pos.x, pos.y);
    glEnd();   
}

void drawLine (Point p0, Point p1)
{
	glDisable(GL_BLEND);

	glBegin(GL_LINES);
		glVertex2d(p0.x, p0.y);
		glVertex2d(p1.x, p1.y);
	glEnd();
}

void drawRec (Point p0, Point p1)
{
	glDisable(GL_BLEND);

	drawLine(p0, Point(p1.x, p0.y));
	drawLine(p0, Point(p0.x, p1.y));
	drawLine(p1, Point(p0.x, p1.y));
	drawLine(p1, Point(p1.x, p0.y));
}

float toRad(float val)
{
	return val*3.14159f/180.0f;
}

void drawCircle(Point centre, float rad)
{
	glDisable(GL_BLEND);

	// How many sides the "circle" should have
	const int sides = 64;

	for (float a = 0.0f; a < 360.0f; a += 360.0f/float(sides))
	{
		// Move around the circumference of the circle getting points.
		float px = float(centre.x) + (rad * cos(toRad(a)) );
		float py = float(centre.y) + (rad * sin(toRad(a)) );
		Point p0 = Point(int(px), int(py));

		px = float(centre.x) + (rad * cos(toRad(a+360.0f/float(sides))) );
		py = float(centre.y) + (rad * sin(toRad(a+360.0f/float(sides))) );
		Point p1 = Point(int(px), int(py));

		// Now draw them	
		drawLine(p0, p1);
	}	
}

/**
 * Changes the colour of anything drawn after it.
 * Uses the 256 way.
 */
void setColor(SDL_Color c)
{
	glColor3f (c.r/256.0, c.g/256.0, c.b/256.0);
}

SDL_Color createColor (int r, int g, int b)
{
	SDL_Color temp;
	temp.r = r;
	temp.g = g;
	temp.b = b;

	return temp;
}

int stringToInt(std::string s)
{
	return atoi(s.c_str());
}