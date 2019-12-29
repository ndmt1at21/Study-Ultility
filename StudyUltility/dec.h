#pragma once
#include <string>
#include <bitset>

class Dec
{
private:
	unsigned int _dec;

public:
	Dec();
	Dec(int);
	std::string to_bin(const int nDigit);
};

