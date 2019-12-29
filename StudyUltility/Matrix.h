#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include "Permutation.h"
#include "ProgramFrame.h"

#define sigma 0.00000001

using namespace std;

template <class T>
class Matrix: public ProgramFrame
{
protected:
	vector<vector<T>> _data;
	int _row;
	int _col;

protected:
	bool validData();
	bool rowAllZero(int);

public:
	Matrix();
	Matrix(const T** arr, int row, int col);
	Matrix(const vector<vector<T>> arr);
	Matrix(const vector<T> arr, int row, int col);
	Matrix(int, int);
	Matrix(const Matrix<T>&);
	~Matrix();

	void input(istream&);
	void output(ostream&, string function = "");

	friend istream& operator>>(istream& in, Matrix& mtx)
	{
		cout << "Nhap so hang, so cot: ";
		in >> mtx._row >> mtx._col;

		if (mtx._row > 0)
			mtx._data.resize(mtx._row);

		for (int i = 0; i < mtx._row; i++)
		{
			cout << "Nhap du lieu hang " << i + 1 << ": ";

			if (mtx._col > 0)
				mtx._data[i].resize(mtx._col);

			for (int j = 0; j < mtx._col; j++)
				in >> mtx._data[i][j];
		}

		return in;
	}

	friend ostream& operator<<(ostream& out, const Matrix mtx)
	{
		for (int i = 0; i < mtx._row; i++)
		{
			for (int j = 0; j < mtx._col; j++)
				out << setprecision(3) << mtx._data[i][j] << "\t";

			cout << endl;
		}

		return out;
	}

	template <class U>
	void init(U value);

	//copy bo hang i, cot j ra, i<0 j<0 -> copy nguyen matrix
	Matrix<T> copy(int iRemove, int jRemove); 
	Matrix<T> join(Matrix mtx);
	Matrix<T> split(int);
	vector<T> toArr1D();

	template <class U>
	Matrix operator/(U k) const;
	Matrix operator*(Matrix<T>) const;

	Matrix<T> transform();
	int rowEchelon(int idxRow, int idxCol);
	double detLeibniz();
	double detGauss();
	Matrix<T> inversion();
	int rank();

	void processing(istream& in, string function);
};

template<class T>
Matrix<T>::Matrix()
{
	_row = 0;
	_col = 0;

	_data.resize(0);
}

template <class T>
Matrix<T>::Matrix(const T** arr, int row, int col)
{
	_row = row;
	_col = col;

	_data.resize(row);
	for (int i = 0; i < _row; i++)
		_data[i].resize(col);


	for (int i = 0; i < _row; i++)
		for (int j = 0; j < _col; j++)
			_data[i][j] = arr[i][j];
}

template <class T>
Matrix<T>::Matrix(const vector<vector<T>> arr)
{
	_row = arr.size();
	_col = 0;

	if (arr.size() > 0)
		_col = arr.size();

	_data.resize(_row);
	for (int i = 0; i < _row; i++)
		_data[i].resize(_col);

	for (int i = 0; i < _row; i++)
		for (int j = 0; j < _col; j++)
			_data[i][j] = arr[i][j];
}

template <class T>
Matrix<T>::Matrix(int row, int col)
{
	_row = row;
	_col = col;

	if (_row > 0)
		_data.resize(_row);

	for (int i = 0; i < _row; i++)
		if (_col > 0)
			_data[i].resize(_col);
}

template <class T>
Matrix<T>::Matrix(vector<T> arr, int row, int col)
{
	_row = row;
	_col = col;

	if (_row > 0)
		_data.resize(_row);

	for (int i = 0; i < _row; i++)
		if (_col > 0)
			_data[i].resize(_col);

	int arrSize = arr.size();
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _col; j++)
		{
			if (i * _col + j < arrSize)
				_data[i][j] = arr[i * _col + j];
		}
	}
}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& mtx)
{
	_data = mtx._data;
	_row = mtx._row;
	_col = mtx._col;
}

template <class T>
Matrix<T>::~Matrix() {}

template <class T>
bool Matrix<T>::validData()
{
	if (_row < 0 || _col < 0)
		return false;

	return true;
}

template <class T>
void Matrix<T>::input(istream& in)
{
	in >> *this;
}

template <class T>
void Matrix<T>::output(ostream& out, string function)
{
	if (function == "echelon")
	{
		rowEchelon(0, 0);
		out << *this;
	}
	else if (function == "det")
	{
		double detA = detGauss();
		out << "Dinh thuc ma tran: " << detA << endl;
	}
	else if (function == "rank")
	{
		int rankA = rank();
		out << "Hang ma tran: " << rankA << endl;
	}
	else if (function == "transform")
	{
		Matrix mtx = transform();
		out << "Ma tran chuyen vi: " << endl;
		out << mtx;
	}
	else if (function == "inversion")
	{
		Matrix mtx = inversion();

		out << "Ma tran kha nghich: " << endl;
		out << mtx;
	}
	else
		out << *this;
}

template <class T>
template <class U>
void Matrix<T>::init(U value)
{
	for (int i = 0; i < _row; i++)
		for (int j = 0; j < _col; j++)
			_data[i][j] = value;
}

//dau vao la hang va cot can bo di
//ra la ma tran da bi bo di row & colRemove 
template <class T>
Matrix<T> Matrix<T>::copy(int iRowRemove, int iColRemove)
{
	int rowTmp = 0;
	int colTmp = 0;

	//kiem tra dau vao
	if (iRowRemove < 0 || iRowRemove > _row)
		rowTmp = _row;

	if (iColRemove < 0 || iColRemove > _col)
		colTmp = _col;

	if (iColRemove >= 0 && iRowRemove >= 0 && iColRemove < _col && iRowRemove < _row)
	{
		rowTmp = _row - 1;
		colTmp = _col - 1;
	}

	Matrix<T> result(rowTmp, colTmp);

	int iRowResult = 0;
	int jColResult = 0;

	for (int i = 0; i < _row; i++)
		if (i != iRowRemove) //ko tinh iRowRemove nen khi chay bo qua th nay
		{
			for (int j = 0; j < _col; j++)
				if (j != iColRemove) //tuong tu ko tinh iColRemove
				{
					result._data[iRowResult][jColResult] = _data[i][j];
					jColResult++;
				}
			iRowResult++;
			jColResult = 0;
		}

	return result;
}

//noi lai 2 ma tran cung so dong
//dau vao la 1 matran
//dau ra la ma tran da duoc noi vs this (this + mtx)
template <class T>
Matrix<T> Matrix<T>::join(Matrix<T> mtx)
{
	Matrix<T> result = *this;
	
	//kiem tra dau vao
	if (_row != mtx._row)
		return result;

	for (int i = 0; i < mtx._row; i++)
		for (int j = 0; j < mtx._col; j++)
			result._data[i].push_back(mtx._data[i][j]);

	result._col += mtx._col;
	return result;
}

//ket qua sau khi split la phan ben phai matrix ke tu idxCol
template <class T>
Matrix<T> Matrix<T>::split(int idxCol)
{
	//result la phan ben phai cua ma tran bi cat tu idxCol
	Matrix<T> result;
	result._data.resize(_row);
	result._row = _row;

	for (int i = 0; i < _row; i++)
	{
		//copy phan ke tu idxCol ve sau vao ma tran ressult
		for (int j = idxCol; j < this->_col; j++)
			result._data[i].push_back(this->_data[i][j]);
		_data[i].resize(idxCol);
	}

	result._col = _col - idxCol;
	_col = idxCol;

	return result;
}

//chuyen ma tran thanh mang 1 chieu
template <class T>
vector<T> Matrix<T>::toArr1D()
{
	vector<T> result;
	result.resize(_row * _col);

	for (int i = 0; i < _data.size(); i++)
		for (int j = 0; j < _data[i].size(); j++)
			//i, j trong matrix -> i*_col+j trong arr1d
			result[i * _col + j] = _data[i][j];  

	return result;
}

//phep nhan ma tran
template <class T>
Matrix<T> Matrix<T>::operator*(Matrix<T> mtx) const
{
	if (_col != mtx._row)
		return *this;

	//2 ma tran mxn va nxp -> nhan lai -> mxp
	Matrix<T> result(_row, mtx._col);
	result.init(0);
	int n = _col;

	//ct [AB]i, j = sigma([A]i, r * [B]r, i) 0<=r<n,  [i, j]: moi phan tu cua result
	for (int i = 0; i < result._row; i++)
		for (int j = 0; j < result._col; j++)
			for (int r = 0; r < n; r++)
				result._data[i][j] += _data[i][r] * mtx._data[r][j];

	return result;
}

//pjep chia voi 1 so k
template <class T>
template <class U>
Matrix<T> Matrix<T>::operator/(U k) const
{
	Matrix<T> result = *this;
	for (int i = 0; i < _row; i++)
		for (int j = 0; j < _col; j++)
			result._data[i][j] /= k;

	return result;
}

//chuyen vi ma tran
template <class T>
Matrix<T> Matrix<T>::transform()
{
	Matrix<T> result(_col, _row);

	//phan tu tai i, j chuyen sang j, i va nguoc lai
	for (int i = 0; i < _row; i++)
		for (int j = 0; j < _col; j++)
			result._data[j][i] = _data[i][j];

	return result;
}

//row0, col0 la vi tri bat dau bien doi, thuong la 0, 0
//tra ve so lan doi cho cua 2 hang (de xem dinh thuc co bi doi dau ko)
template <class T>
int Matrix<T>::rowEchelon(int row0, int col0) 
{
	int num;
	if (row0 == _row || col0 == _col) //khi chi so ra ngoai mang nxn
		return 0;

	//neu phan tu [0][0] == 0
	bool isSwap = false;
	if (_data[row0][col0] == 0)
	{
		bool colAllZero = false;

		//tim kiem 1 hang trong mang co phan tu dau = 0
		for (int i = row0 + 1; i < _row; i++)
		{
			if (_data[i][col0] != 0)
			{
				swap(_data[row0], _data[i]);
				isSwap = true;
				break;
			}

			//nguyen hang = 0 thi chuyen sang buoc ke tiep luon
			if (i == _row - 1)
				colAllZero = true;
		}

		//neu trong 1 cot toan phan tu = 0, chuyen sang mang con
		if (colAllZero == true)
		{
			num = rowEchelon(row0, col0 + 1);
			return num;
		}
	}

	//khu cac phan tu duoi hang row0 bang cach - cac phan tu hang tren * k
	//k = phan tu dau tien hang hien tai/phan tu dau tien row0
	for (int row = row0 + 1; row < _row; row++)
	{
		double k = _data[row][col0] / _data[row0][col0];
		for (int col = col0; col < _col; col++)
		{
			_data[row][col] = _data[row][col] - k * _data[row0][col];
			
			//lam tron sai so ~ 0 (vi sd double)
			if (abs(_data[row][col]) < sigma) 
				_data[row][col] = 0;
		}
	}

	num = rowEchelon(row0 + 1, col0 + 1);

	if (isSwap == true) //neu co doi thi so lan doi se +1, else ko lam gi
		return num + 1;
	return num;
}

//tinh det theo cong thuc Leibniz
template <class T>
double Matrix<T>::detLeibniz()
{
	if (_row != _col)
		return -INT_MAX;

	Permutation<T> per(0, _row - 1);

	vector<vector<T>> listPermuted = per.permuted();
	vector<int> arrSign = per.numberOfPermute();

	double det = 0;
	for (int i = 0; i < listPermuted.size(); i++) // VD: 1 hang: 0 2 1
	{
		int sign = 0;
		if (arrSign[i] % 2 == 0)
			sign = 1;
		else
			sign = -1;

		double tmp = 1;
		for (int j = 0; j < _col; j++) //ket hop mac dinh vs 0 1 2 -> cac cap (0, 0) (2,1) (1,2)
		{
			int idx = listPermuted[i][j];
			tmp *= _data[idx][j];
		}
		det += tmp * sign;
	}

	return det;
}

//tinh det theo cach bien doi gauss, tra ve 1 det
template <class T>
double Matrix<T>::detGauss()
{
	if (_row != _col)
		return -INT_MAX;

	Matrix mtx = *this; //ko thay doi du lieu trong _data
	
	//doi cho hai hang, 2 cot cho nhau -> dinh thuc doi dau
	int sign = mtx.rowEchelon(0, 0); 

	double det = 1;
	for (int i = 0; i < mtx._row; i++)
		det *= mtx._data[i][i];

	if (sign % 2 != 0)
		det = -det;

	return det;
}

//tra ve ma tran nghich dao
//A^-1 = 1/abs(detA) * A
template <class T>
Matrix<T> Matrix<T>::inversion()
{
	if (_row != _col)
		return *this;

	Matrix<T> result;

	//tinh gia tri tuyen doi cua det
	double det = this->detGauss();

	if (abs(det) < sigma)
		return result;

	result = Matrix(_row, _col);
	for (int i = 0; i < result._row; i++)
		for (int j = 0; j < result._col; j++)
		{
			//copy bo hang i, j ra
			Matrix<T> tmp = this->copy(i, j); 

			//phan tu tai hang i, j = -1^(i+j) nhan cho det (ma tran con bo hang i cot j)
			// roi dem chia cho det cua ma tran goc
			result._data[i][j] = pow(-1, i + j) * tmp.detGauss() / det;
		}

	result = result.transform();

	return result;
}

//tinh hang ma tran
template <class T>
int Matrix<T>::rank()
{
	//bien ve ma tran bac thang
	Matrix<T> mtx = *this;
	mtx.rowEchelon(0, 0);

	int nRowAllZero = 0; //dem so hang toan la so 0
	for (int i = _row - 1; i >= 0; i--)
	{
		//dem soo 0 cua 1 hang
		int nZero = 0;
		for (int j = 0; j < _col; j++)
		{
			if (mtx._data[i][j] == 0)
				nZero++;
			else
				break;
		}

		//so cac so 0 = so phan tu cua hang -> hang toan = 0
		if (nZero == _col)
		{
			nZero = 0;
			nRowAllZero++;
		}
	}

	return (_row - nRowAllZero); //hang ma tran = so hang ma tran - so hang toan la 0
}

//kiem tra hang co toan so 0 ko?
template <class T>
bool Matrix<T>::rowAllZero(int idxRow)
{
	for (int i = 0; i < _col; i++)
		if (_data[idxRow][i] != 0)
			return false;

	return true;
}

template <class T>
void Matrix<T>::processing(istream& in, string function)
{
	if (function == "multiMtx")
	{
		cout << "Nhap ma tran thu hai: " << endl;
		Matrix<T>* mtx = new Matrix<T>();
		mtx->input(cin);

		if (!mtx->validData())
		{
			errorMessage(cout);
			return;
		}

		*this = (*this) * (*mtx);
		delete mtx;
	}
}