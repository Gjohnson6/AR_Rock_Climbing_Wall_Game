#pragma once
#include <string>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "glm\glm.hpp"
//#include "ButtonAction.h"

using namespace std;

//Buttons will be clickable objects that point to other objects to perform some action
//When a button is clicked it might move to a menu or open a file explorer
class Button
{
public:
	//There are two ways this constructor can work based on centerBased. Either they are entered directly or they are interpreted as the two center points, and the width and the height
	Button(int l, int r, int t, int b, int m, string n) : left(l), right(r), top(t), bottom(b), menuNum(m), name(n){};

	//Enter center point of button, and width and height of button
	//Button(int x, int y, int h, int w, string n);
		
	~Button();

	//Detect whether this button has been clicked, if it has, return the menuNum
	int DetectClick(int x, int y);

	void Draw();
	string GetName();
private:
	
	//The boundaries of the button. The bottom right corner is at (bottom, right, 0) for instance
	int left, right, top, bottom;
	int menuNum = -1;
	string name;
};