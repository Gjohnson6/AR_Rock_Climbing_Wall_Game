#include "Map.h"



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

vector<string> Map::getScores()
{
	return topScores;
}
