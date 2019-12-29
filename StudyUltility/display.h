#pragma once
#include <iostream>
#include "buffScreen.h"
#include "Keyboard.h"

#include "Vector.h"
#include "Matrix.h"
#include "Equations.h"
#include "Karnaugh.h"

#define nScreenWidth 100
#define nScreenHeight  40
#define LEFT_CURSOR 15
#define TOP_Y 8
#define BOT_Y TOP_Y + 8

class Display:public BuffScreen
{
public:
	Display() :BuffScreen(nScreenHeight, nScreenWidth) {}
	void run();
	void runFunction(int numFunction);
};

