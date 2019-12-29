#pragma once
#include "Matrix.h"
#include "ProgramFrame.h"
#include "String.h"

class Equations: public Matrix<double>
{
private:
	int _nVar;
	int _nEquation;
	int _nDigit; //so chu so thap phan

protected:
	bool validData();

	// danh sach chi so phan tu khac 0 dau tien cua moi dong
	int* listIdxFirstVal();

	//round 1 so thuc -> string
	//ex: 3.45672, nDigit = 2 -> 3.45
	string round(double n);

	//chuyen so hang thanh string (ex. nameVar = "x", numVar = 2, factor = 1
	//result: x2
	string termToString(string nameVar, int numVar, double factor);

	//chinh sua ket qua cua 1 bien
	string modify(string str);

	//dau vao la 1 ma tran gom he so cac an va he so tu do
	//tra ve la he phuong trinh da dc giai (chuyen ve, chia he so)
	Equations solveLinearEqua();

public:
	Equations();
	Equations(int nVar, int numEquation);
	Equations(const Equations&);

	void input(istream& in);
	void output(ostream& out, string function = "");
};

