#pragma once
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "glm\glm.hpp"
#include <iostream>
#include <string>

using namespace std;

class HitMarker
{
public:
	HitMarker(int num, int x, int y);
	~HitMarker();

	void move(int x, int y);
	void Draw();
	bool DetectMouse(int x, int y);
	int getNum();
	void setNum(int num);
	void getHit();
	int getX();
	int getY();
	void Reset();
private:
	int num;
	int x;
	int y;
	double radius;
	double hitRadius = 40.0;
	bool dec = false;
	bool hit = false;
	double r = 1.0, g = 1.0, b = 1.0;
};

