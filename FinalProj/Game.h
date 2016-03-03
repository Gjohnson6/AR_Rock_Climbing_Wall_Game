#pragma once
#include "HitMarker.h"
#include <vector>

using namespace std;

class Game
{
public:
	Game(int width = 1280, int height = 720);
	~Game();

	void init();
	int AddMarker(int x, int y);
	void DeleteMarker(int x, int y);
	void(*DisplayFunc)();
	void(*RedisplayFunc)(int, int);
	void(*MouseFunction)(int, int, int, int);

	GLuint handle;
	int width;
	int height;
	vector<HitMarker> markers;
private:

	enum State {
		Menu,
		New,
		Play,
		Results
	};
};

