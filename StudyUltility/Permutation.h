#pragma once
#include <iostream>
#include <vector>

using namespace std;

template <class T>
class Permutation
{
private:
	vector<T> _arrToPermute;

	void permuted(int l, int h, vector<vector<T>>& listPermuted);
	void numberOfPermute(const vector<T> src, vector<T>& numOfPermute, int l, int h);

public:
	Permutation();
	Permutation(vector<T> src);
	Permutation(int begin, int end);

	vector<vector<T>> permuted();
	vector<T> numberOfPermute();
};

template <class T>
Permutation<T>::Permutation() {}

template <class T>
Permutation<T>::Permutation(vector<T> src)
{
	_arrToPermute = src;
}

template <class T>
Permutation<T>::Permutation(int begin, int end)
{
	for (int i = begin; i <= end; i++)
		_arrToPermute.push_back(i);
}

//dau vao la idx left cua day, height (cuoi) cua day, listPermuted la ket qua
// sau khi da hoan vi
template <class T>
void Permutation<T>::permuted(int l, int h, vector<vector<T>>& listPermuted)
{
	if (l == h)
		listPermuted.push_back(_arrToPermute);
	else
	{
		for (int i = l; i < _arrToPermute.size(); i++)
		{
			swap(_arrToPermute[l], _arrToPermute[i]);
			permuted(l + 1, h, listPermuted);
			swap(_arrToPermute[l], _arrToPermute[i]);
		}
	}
}

//dau vao la chuoi goc (chua hoan vi), ket qua so lan hoan vi, left, height cua chuoi 
template <class T>
void Permutation<T>::numberOfPermute(const vector<T> src, vector<T>& numOfPermute, int l, int h)
{
	static int count = 0;
	if (l == h)
		numOfPermute.push_back(count);
	else
	{
		for (int i = l; i < _arrToPermute.size(); i++)
		{
			if (_arrToPermute[l] != _arrToPermute[i])
				count++;

			numberOfPermute(src, numOfPermute, l + 1, h);

			if (_arrToPermute[l] != _arrToPermute[i])
				count--;
		}
	}
}

//ham cho ra ket qua sau khi hoan vi
template <class T>
vector<vector<T>> Permutation<T>::permuted()
{
	vector<vector<int>> result;
	permuted(0, _arrToPermute.size() - 1, result);

	return result;
}

//so lan hoan vi cua 1 so trong day
template <class T>
vector<T> Permutation<T>::numberOfPermute()
{
	vector<int> result;
	numberOfPermute(_arrToPermute, result, 0, _arrToPermute.size() - 1);

	return result;
}
