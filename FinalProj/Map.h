#pragma once
#include "HitMarker.h"
#include <time.h>
#include <vector>

using namespace std;

//The map class holds the hitmarkers, name, and top scores for a given map.
class Map
{
public:
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
	vector<HighScore> getScores();

	void AddHighScore(string name, clock_t time);
	int AddMarker(int x, int y);
	void MoveMarker(int index, int x, int y);

private:
	vector<HitMarker> markers;
	vector<HighScore> highScores;
	string name;
	
};

