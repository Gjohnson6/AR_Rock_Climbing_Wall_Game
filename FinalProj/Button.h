#pragma once
#include "GL\glew.h"
#include "GL\freeglut.h"
#include <string>
#include "ButtonAction.h"

using namespace std;

//Buttons will be clickable objects that point to other objects to perform some action
//When a button is clicked it might move to a menu or open a file explorer
class Button
{
public:
	//There are two ways this constructor can work based on centerBased. Either they are entered directly or they are interpreted as the two center points, and the width and the height
	Button(int leftOrX, int rightOrY, int topOrWidth, int bottomOrHeight, string n) : left(leftOrX), right(rightOrY), top(topOrWidth), bottom(bottomOrHeight), name(n){};

	//Enter center point of button, and width and height of button
	//Button(int x, int y, int h, int w, string n);
		
	~Button();

	//Detect whether this button has been clicked.
	bool DetectClick(int x, int y);

	void Draw();
private:
	
	//The boundaries of the button. The bottom right corner is at (bottom, right, 0) for instance
	int left, right, top, bottom;
	string name;
};