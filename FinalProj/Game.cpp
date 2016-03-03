#include "Game.h"

Game::Game(int width, int height)
{
	this->width = width;
	this->height = height;
}


Game::~Game()
{
}

void Game::init()
{
}

int Game::AddMarker(int x, int y)
{
	int retVal = -1;
	if (this->markers.size() < 9)
	{
		HitMarker mark = HitMarker(this->markers.size() + 1, x,  y);
		this->markers.push_back(mark);
		retVal = mark.getNum() - 1;
	}

	return retVal;
}

//When deleting markers, change the markers' numbers to correspond
void Game::DeleteMarker(int x, int y)
{
	cout << "DELETE" << endl;
	for (int i = 0; i < markers.size(); i++)
	{
		if (markers[i].DetectMouse(x, y))
		{
			markers.erase(markers.begin() + i);
			for (int i = 0; i < markers.size(); i++)
			{
				markers[i].setNum(i + 1);
			}
			break;
		}
	}
}