#ifndef GRAPH_H
#define GRAPH_H

#include "text.h"
#include "sprite.h"

class Graph {

private:
	Sprite *sprLine;
	Sprite *sprPixel;
	Text *text;

	int graph[50];
	int graphSize = 50;

	int max = 10;
	int min = 0;

public:
	Graph();
	void UpdateValues(int val);
	void Draw();
};

#endif