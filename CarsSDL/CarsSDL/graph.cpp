#include "graph.h"

Graph::Graph()
{
	sprPixel  = new Sprite("spr_graph.png");
	sprLine = new Sprite("spr_graph_pixel.png");
	text	 = new Text("lazy.ttf");

	for (unsigned i = 0; i < graphSize; i++)
	{
		graph[i] = 0;
	}
}

void Graph::UpdateValues(int val)
{
	if (val > max) max = val;

	for (unsigned i = 0; i < graphSize - 1; i++)
	{
		graph[i] = graph[i + 1];
	}

	graph[graphSize - 1] = val;
}

void Graph::Draw()
{
	text->Draw(850, 180, std::to_string(max));
	text->Draw(850, 480, std::to_string(min));

	int average = 0;
	int averageMax = 0;

	for (unsigned i = 0; i < graphSize; i++)
	{
		average += graph[i];
		if (graph[i] != 0) averageMax++;

		int y = (int)((300.0f / (float)max) * (float)graph[i]);

		if (i < graphSize - 1)
		{
			int maxY = (int)((300.0f / (float)max) * (float)graph[i + 1]);

			if (maxY > y)
			{
				for (unsigned j = y; j < maxY; j++)
				{
					if ((maxY - y) / 4 != 0)
						sprLine->Draw(870 + i * 5 + 1 + (j - y) / ((maxY - y) / 4), 480 - j);
				}
			}
			else if (y > maxY)
			{
				for (unsigned j = maxY + 2; j < y; j++)
				{

					if ((maxY - y) / 4 != 0)
						sprLine->Draw(870 + (i + 1) * 5 - 2 - (j - y) / ((maxY - y) / 4), 480 - j);
				}
			}
			else
			{
				sprLine->Draw(870 + i * 5 + 1, 480 - y);
				sprLine->Draw(870 + i * 5 + 2, 480 - y);
				sprLine->Draw(870 + i * 5 + 3, 480 - y);
				sprLine->Draw(870 + i * 5 + 4, 480 - y);
			}
		}

		sprPixel->Draw(870 + i * 5, 480 - y);
	}

	if (averageMax != 0)
	{
		average = (int)((float)average / (float)averageMax);
		int newY = (int)((300.0f / (float)max) * (float)average);

		if (average != 0)
		{
			text->Draw(850, 480 - newY, std::to_string(average));

			for (unsigned i = 0; i < graphSize * 5; i++)
			{
				sprLine->Draw(870 + i, 480 - newY);
			}
		}
	}
}