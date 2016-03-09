#pragma once
#include "HitMarker.h"
#include <vector>

using namespace std;

//The map class holds the hitmarkers, name, and top scores for a given map.
class Map
{
public:
	Map();
	~Map();

	vector<HitMarker> getMarkers();
	vector<string> getScores();

private:
	vector<HitMarker> markers;
	vector<string> topScores;
	string name;
	
};

