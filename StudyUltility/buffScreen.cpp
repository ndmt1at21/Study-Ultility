#include "BuffScreen.h"

BuffScreen::BuffScreen()
{
	_nScreenWidth = 50;
	_nScreenHeight = 60;
	_buffer = new CHAR_INFO[_nScreenHeight * _nScreenWidth];
	_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	init(' ');
}

BuffScreen::BuffScreen(int row, int width)
{
	_nScreenWidth = width > 0 ? width : 0;
	_nScreenHeight = row > 0 ? row : 0;
	_buffer = new CHAR_INFO[_nScreenHeight * _nScreenWidth];
	_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	init(' ');
}

BuffScreen::~BuffScreen()
{
	delete[] _buffer;
	CloseHandle(_hConsole);
}

void BuffScreen::init(char ch)
{
	for (int i = 0; i < _nScreenHeight; i++)
		for (int j = 0; j < _nScreenWidth; j++)
		{
			_buffer[i * _nScreenWidth + j].Char.AsciiChar = ch;
			_buffer[i * _nScreenWidth + j].Attributes = BACKGROUND_BLACK;
		}
}

void BuffScreen::clrscr()
{
	init(' ');
}

void BuffScreen::setCursor(int visible) // 0 hide, 1 show
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1; 
	cursor.bVisible = visible;
	SetConsoleCursorInfo(_hConsole, &cursor);
}

void BuffScreen::fileToBuffScr(const char* fileName, COORD pos)
{
	if (pos.X < 0 && pos.X >= _nScreenWidth)
		return;

	std::ifstream inFile(fileName);
	if (inFile.fail())
		return;

	char* ch = new char[_nScreenWidth + 1];
	while (inFile.getline(ch, _nScreenWidth))
	{
		ch[_nScreenWidth] = '\0';
		strToBuffScr(ch, pos);

		pos.Y++;
	}
}

void BuffScreen::strToBuffScr(const char* str, COORD pos)
{
	if (pos.X < 0 && pos.X >= _nScreenWidth)
		return;

	int i = 0;
	while (str[i] != '\0')
	{
		_buffer[pos.Y * _nScreenWidth + pos.X].Char.AsciiChar = str[i];
		_buffer[pos.Y * _nScreenWidth + pos.X].Attributes = FOREGROUND_WHITE;
		pos.X++;
		i++;
	}
}

void BuffScreen::charToBuffScr(const char ch, COORD pos)
{
	if (pos.X < 0 && pos.X >= _nScreenWidth)
		return;

	_buffer[pos.Y * _nScreenWidth + pos.X].Char.AsciiChar = ch;
	_buffer[pos.Y * _nScreenWidth + pos.X].Attributes = FOREGROUND_WHITE;
}

void BuffScreen::output()
{
	DWORD dwBytesWritten = 0;
	COORD sizeBuff = { _nScreenWidth, _nScreenHeight };
	SMALL_RECT rect = { 0, 0, (SHORT)_nScreenWidth, (SHORT)_nScreenHeight };

	WriteConsoleOutput(_hConsole, _buffer, sizeBuff, { 0, 0 }, &rect);
}