#include "Vector.h"

Vector::Vector()
{
	_dim = 0;
	_data.resize(0);
}

Vector::Vector(int dim)
{
	_dim = dim;

	if (dim > 0)
		_data.resize(_dim);

	for (int i = 0; i < _dim; i++)
		_data[i] = 0;
}

Vector::Vector(vector<double> vct)
{
	_dim = vct.size();
	_data = vct;
}

//copy Vector
Vector::Vector(const Vector& vct)
{
	_dim = vct._dim;

	if (_dim > 0)
		_data.resize(_dim);
	
	for (int i = 0; i < _dim; i++)
		_data[i] = vct._data[i];
}

Vector::~Vector() {}

//kiem tra du lieu vector
bool Vector::validData()
{
	if (_dim <= 0)
		return false;

	return true;
}

void Vector::input(istream& in)
{
	cout << "Nhap vector: " << endl;
	in >> *this;
}

void Vector::output(ostream& out, string function)
{
	out << *this << endl;
}

istream& operator>>(istream& in, Vector& vct)
{
	cout << "Nhap so chieu vector: ";
	in >> vct._dim;

	if (vct._dim > 0)
	{
		vct._data.resize(vct._dim);

		cout << "Nhap du lieu vector: ";
		for (int i = 0; i < vct._dim; i++)
			in >> vct._data[i];
	}

	return in;
}

ostream& operator<<(ostream& out, const Vector vct)
{
	out << "(";
	for (int i = 0; i < vct._dim; i++)
	{
		if (i == vct._dim - 1)
			out << vct._data[vct._dim - 1] << ")";
		else
			out << vct._data[i] << "; ";
	}
	
	return out;
}

Vector Vector::operator=(Vector vct) 
{
	_dim = vct._dim;
	_data = vct._data;

	return *this;
}

double Vector::operator[](int idx)
{
	if (idx < 0 || idx >= _dim)
		return -1;

	return _data[idx];
}

Vector Vector::operator+(const Vector vct) const
{
	Vector result = *this;

	if (_dim == vct._dim)
	{
		result._data.resize(_dim);
		for (int i = 0; i < _dim; i++)
			result._data[i] = _data[i] + vct._data[i];
	}

	return result;
}

Vector Vector::operator-(const Vector vct) const
{
	Vector result = *this;

	//lay tung so trong vector _data - cho so vct
	if (_dim == vct._dim)
	{
		result._data.resize(_dim);

		for (int i = 0; i < _dim; i++)
			result._data[i] = _data[i] - vct._data[i];
	}

	return result;
}

Vector Vector::operator*(double alpha) const
{
	//lay tung so trong vector _data * cho he so alpha
	Vector result(_dim);

	for (int i = 0; i < _dim; i++)
		result._data[i] = _data[i] * alpha;

	return result;
}

//xu li cong va nhan, muc dich de cho ham run co cach su dung chung
void Vector::processing(istream& in, string function)
{
	if (function == "multiAlpha")
	{
		cout << "Nhap alpha: ";
		int alpha;
		in >> alpha;

		*this = *this * alpha;
	}
	else if (function == "plusVct")
	{
		Vector* vct = new Vector();
		vct->input(in);

		if (!vct->validData())
		{
			errorMessage(cout);
			return;
		}

		*this = *this + *vct;
		delete vct;
	}
}