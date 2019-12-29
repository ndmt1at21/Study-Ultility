#include "Equations.h"
#include <string>
#include <sstream>

Equations::Equations()
{
	_nVar = 0;
	_nEquation = 0;
	_nDigit = 0;
}

Equations::Equations(int nVar, int numEquation) 
{
	_nVar = nVar;
	_nEquation = numEquation;
	_nDigit = 0;

	int col = 0;
	int row = 0;

	if (_nVar < _nEquation) //so bien nho hon so pt 
	{
		row = _nEquation;
		col = _nVar + 1;
	}
	else
	{
		row = _nVar;
		col = _nVar + 1; //tinh hstd
	}

	_col = col;
	_row = row;

	_data.resize(_row);
	for (int i = 0; i < _row; i++)
		_data[i].resize(_col);
	
	init(0);
}

Equations::Equations(const Equations& equa) 
{
	_data = equa._data;
	_row = equa._row;
	_col = equa._col;

	_nVar = equa._nVar;
}

void Equations::input(istream& in)
{
	cout << "Nhap so bien cua he: ";
	in >> _nVar;

	cout << "Nhap so phuong trinh: ";
	in >> _nEquation;

	*this = Equations(_nVar, _nEquation);

	cout << endl << "----Nhap du lieu he phuong trinh----" << endl;
	for (int i = 0; i < _nEquation; i++)
	{
		cout << "Nhap lan luot he so cua phuong trinh thu " << i + 1 << ": ";
		for (int j = 0; j < _nVar + 1; j++) //tinh hstd nua
			in >> _data[i][j];
	}

	cout << "So chu so thap phan hien thi: ";
	cin >> _nDigit;
}

void Equations::output(ostream& out, string function)
{
	if (function != "")
		return;

	Equations result = this->solveLinearEqua();

	if (result._row == 0)
	{
		cout << "No solution" << endl;
		return;
	}

	cout << "Solution: " << endl;
	for (int i = 0; i < _nVar; i++)
	{
		cout << "x" << i + 1 << " = ";

		string rootEquation;
		for (int j = 0; j < _col; j++) //ko tinh he so tu do
		{
			if (j == _col - 1) //he so tu do
				rootEquation += "+" + termToString("", 0, result._data[i][j]);
			else
				rootEquation += "+" + termToString("x", j + 1, result._data[i][j]);
		}

		rootEquation = modify(rootEquation);

		cout << rootEquation << endl;
	}
	cout << endl;
}

//chua mang cac gia tri dau tien khac 0 tai moi dong, neu dong toan bo = 0 -> -1
int* Equations::listIdxFirstVal()
{
	int* varHasValue = new int[_col]; //danh dau bien do co gia tri hay khong?
	int* firstIdxRow = new int[_row]; //danh dau vi tri dau tien != 0 moi dong

	for (int i = 0; i < _col; i++)
		varHasValue[i] = firstIdxRow[i] = -1;

	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _col; j++)
			if (_data[i][j] != 0 && varHasValue[j] != 1) //neu bien do co gt roi thi bo qua
			{
				firstIdxRow[i] = j;
				varHasValue[j] = 1; //neu vi tri dau tien != 0 roi vao cot j=2 -> x3 se co gia tri -> danh dau
				break;
			}
	}

	delete[] varHasValue;
	return firstIdxRow;
}

string Equations::round(double n)
{
	string n_str = to_string(n);
	string n_round;

	//round to n digit
	for (int i = 0; i < n_str.find('.') + _nDigit + 1; i++)
		n_round.push_back(n_str[i]);

	return n_round;
}


string Equations::termToString(string nameVar, int numVar, double factor)
{
	string term = "";

	//he so truoc x = 0 -> kq rong
	if (factor == 0)
		return term;

	string factor_str = round(factor);
	string numVar_str = to_string(numVar);

	string result;
	if (nameVar == "" && numVar == 0)
		result = factor_str;
	else
	{
		if (factor == 1)
			result = nameVar + numVar_str;
		else
			result = factor_str + "*" + nameVar + numVar_str;
	}

	return result;
}

string Equations::modify(string str)
{
	string result = str;

	if (result == "")
		result = round(0);
	
	//+- -> -
	int i = 0;
	while ((i = result.find("+-")) >= 0)
		result.replace(i, 2, "-");

	// ++ -> +
	i = 0;
	while ((i = result.find("++")) >= 0)
		result.replace(i, 2, "+");

	//+ daau, duoi bo
	i = 0;
	while ((i = result.find("+")) == 0 || (i = result.find("+")) == result.size() - 1)
	{
		if (result.size() == 0)
			break;
		result.replace(i, 1, "");
	}

	string tmp;
	for (int i = 0; i < result.length(); i++)
	{
		int ch = result[i];
		
		if (ch == '+')
			tmp += " + ";
		else if (ch == '-')
			tmp += " - ";
		else
			tmp += ch;
	}
	result = tmp;

	if (result.length() == 0)
		result = round(0);

	return result;
}


Equations Equations::solveLinearEqua() 
{
	//kiem tra co vo nghiem ko
	Equations tmp = *this;
	Matrix factor = tmp.split(tmp._col - 1); //cat he so tu do di
	
	//init result
	Equations result(0, 0);

	//rank A != A' -> vo nghiem
	if (tmp.rank() != this->rank())
		return result;
	
	//tra ve 1 ma tran gom n cot, m dong. n cot = (n - 1) cot la bien + 1 cot la he so tu do
	// m dong tuong ung voi kq x1 = ..., x2 =....
	Equations mtx = *this;
	mtx.rowEchelon(0, 0); //bien doi ma tran bac thang

	//tim kiem vi tri dau tien moi dong !=0
	int* firstIdxRow = mtx.listIdxFirstVal();

	result = Equations(_nVar, _nEquation);

	//
	for (int i = 0; i < _row; i++)
	{
		if (firstIdxRow[i] >= 0)
			result._data[firstIdxRow[i]] = mtx._data[i];
	}
	firstIdxRow = result.listIdxFirstVal(); //cap nhap lai theo result
	
	//chuyen ve
	for (int i = 0; i < result._row; i++)
	{
		if (firstIdxRow[i] >= 0)
		{
			for (int j = 0; j < result._col - 1; j++)  //ko tinh he so tu do o cot cuoi
			{
				if (j != firstIdxRow[i]) //ko chuyen ve xi .VD: x1+2x2=0 -> x1 = -2x2 (ko chuyen x1)
					result._data[i][j] = -result._data[i][j];
			}
		}
	}

	//chia he so lan 1
	for (int i = 0; i < result._row; i++)
	{
		if (firstIdxRow[i] >= 0)
		{
			double k = result._data[i][firstIdxRow[i]];
			for (int j = firstIdxRow[i]; j < result._col; j++)
   				result._data[i][j] /= k;

			result._data[i][firstIdxRow[i]] = 0; //cac bien kq se = 0 tai do
		}
	}

	//set cac bien co vo so nghiem -> he so 1 tai cot he so cua bien do
	for (int i = 0; i < result._row; i++)
	{
		if (firstIdxRow[i] < 0)
			result._data[i][i] = 1;
	}
	cout << result << endl;

	// thay cac bien vao VD 2 pt: x1 + x2 = 2; x2 = 4 -> thay x2 vao pt1
	for (int i = 0; i < result._row; i++)
	{
		if (firstIdxRow[i] < 0)
			continue;

		//chay cac bien moi hang VD _col = 4 -> so bien la 3, con lai hstd
		for (int j = 0; j < result._col - 1; j++) 
		{
			if (result._data[i][j] == 0 || j == firstIdxRow[i])
				continue;

			for (int r = 0; r < result._col; r++)
			{
				//moi phan tu se nhan vs cai x cua no (thay the vao)
				//VD x1 = -x2 + x3, x2 = 0 + x3, x3 = 10 -> thay vao x1
				if (r != j)
					result._data[i][r] += result._data[i][j] * result._data[j][r];
			}

			result._data[i][j] = result._data[i][j] * result._data[j][j];
		}
	}

	//chia he so lan 2
	for (int i = 0; i < result._row; i++)
	{
		if (firstIdxRow[i] >= 0) //ko can chia cho cac bien co vo so no
		{
			double k = 1 - result._data[i][firstIdxRow[i]];
			for (int j = firstIdxRow[i]; j < result._col; j++)
				result._data[i][j] /= k;

			result._data[i][firstIdxRow[i]] = 0; //cac bien kq se = 0 tai do
		}
	}

	return result;
}

bool Equations::validData()
{
	if (_nVar <= 0 || _nEquation <= 0 || _nDigit < 0 || _nDigit > 15) //double 15 digits decimal
		return false;

	return true;
}