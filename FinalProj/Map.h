#pragma once
#include "HitMarker.h"
#include <time.h>
#include <vector>

using namespace std;

//The map class holds the hitmarkers, name, and top scores for a given map.
class Map
{
public:
	Map(vector<HitMarker> markersVec) : markers(markersVec){};

	struct HighScore
	{
		string PlayerName;
		clock_t PlayerTime;

		bool operator > (const HighScore& str) const
		{
			return (PlayerTime > str.PlayerTime);
		}
	};

	Map();
	~Map();

	vector<HitMarker> getMarkers();
	void SaveMap(string filename);
	void LoadMap(string filename);

private:
	vector<HitMarker> markers;
	vector<HighScore> highScores;
	string name;
};