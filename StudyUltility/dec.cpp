#include "dec.h"

Dec::Dec()
{
	_dec = 0;
}

Dec::Dec(int a)
{
	_dec = a;
}

std::string Dec::to_bin(const int nDigit)
{
	if (_dec < 0)
		return "0";

	unsigned int decClone = _dec;
	std::string result;
	result.resize(nDigit, '0');

	int i = result.size() - 1;
	while (decClone != 0)
	{
		char tmp = decClone % 2 + '0';
		result[i] = tmp;
		decClone /= 2;
		
		i--;

		//de phong loi 
		if (i < 0)
			break;
	}

	return result;
}