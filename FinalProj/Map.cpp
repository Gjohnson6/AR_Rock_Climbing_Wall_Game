#include "Map.h"
#include <algorithm>
#include <functional>

Map::Map()
{
}


Map::~Map()
{
}

vector<HitMarker> Map::getMarkers()
{
	return this->markers;
}

vector<Map::HighScore> Map::getScores()
{
	return highScores;
}

void Map::AddHighScore(string name, clock_t time)
{
	HighScore newScore;
	newScore.PlayerName = name;
	newScore.PlayerTime = time;
	highScores.push_back(newScore);
	std::sort(highScores.begin(), highScores.end(), greater<HighScore>());
}

int Map::AddMarker(int x, int y)
{
	int retVal = -1;
	if (markers.size() < 9)
	{
		HitMarker mark = HitMarker(markers.size() + 1, x, y);
		markers.push_back(mark);
		retVal = mark.getNum() - 1;
	}
	return retVal;
}

void Map::MoveMarker(int index, int x, int y)
{
	markers[index].move(x, y);
}
