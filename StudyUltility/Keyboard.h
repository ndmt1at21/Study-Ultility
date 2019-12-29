#pragma once
#include <conio.h>

#define KEY_ENTER		13
#define KEY_ESC			27
#define KEY_DOWN		80
#define KEY_UP			72

class Keyboard
{
public:
	//kiem tra co an phim k?
	static bool keyPressed()
	{
		return _kbhit();
	}

	//lay du lieu tu lan nhan do
	static int getKey()
	{
		return _getch();
	}
};

