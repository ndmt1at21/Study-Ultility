#pragma once
#include <iostream>
#include <vector>
#include "ProgramFrame.h"

using namespace std;

class Vector :public ProgramFrame
{
private:
	vector<double> _data;
	int _dim;

protected:
	bool validData();

public:
	Vector();
	Vector(int);
	Vector(vector<double>);
	Vector(const Vector&);
	~Vector();

	void input(istream&);
	void output(ostream&, string function = "");

	friend istream& operator>>(istream& in, Vector& vct);
	friend ostream& operator<<(ostream& out, const Vector);

	Vector operator=(Vector);
	double operator[](int);
	Vector operator+(Vector) const;
	Vector operator-(Vector) const;
	Vector operator*(double) const;

	void processing(istream& in, string function = "");
};

