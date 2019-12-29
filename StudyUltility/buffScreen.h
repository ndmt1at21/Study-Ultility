#pragma once
#include <Windows.h>
#include <conio.h>
#include <fstream>

#define FOREGROUND_WHITE 7
#define BACKGROUND_BLACK 0

class BuffScreen
{
protected:
	CHAR_INFO* _buffer;
	int _nScreenWidth;
	int _nScreenHeight;
	HANDLE _hConsole;

public:
	BuffScreen();
	BuffScreen(int, int);
	~BuffScreen();

	void init(char ch);
	void clrscr();
	void setCursor(int);
	void fileToBuffScr(const char* fileName, COORD pos);
	void strToBuffScr(const char* str, COORD pos);
	void charToBuffScr(const char ch, COORD pos);

	void output();
};

