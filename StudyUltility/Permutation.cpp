//#include "Permutation.h"
//
//
//Permutation::Permutation() {}
//
//Permutation::Permutation(vector<int> src)
//{
//	_arrToPermute = src;
//}
//
//Permutation::Permutation(int begin, int end)
//{
//	for (int i = begin; i <= end; i++)
//		_arrToPermute.push_back(i);
//}
//
//void Permutation::permuted(int l, int h, vector<vector<int>>& listPermuted)
//{
//	if (l == h)
//		listPermuted.push_back(_arrToPermute);
//	else
//	{
//		for (int i = l; i < _arrToPermute.size(); i++)
//		{
//			swap(_arrToPermute[l], _arrToPermute[i]);
//			permuted(l + 1, h, listPermuted);
//			swap(_arrToPermute[l], _arrToPermute[i]);
//		}
//	}
//}
//
//void Permutation::numberOfPermute(const vector<int> src, vector<int>& numOfPermute, int l, int h)
//{
//	static int count = 0;
//	if (l == h)
//		numOfPermute.push_back(count);
//	else
//	{
//		for (int i = l; i < _arrToPermute.size(); i++)
//		{
//			if (_arrToPermute[l] != _arrToPermute[i])
//				count++;
//
//			numberOfPermute(src, numOfPermute, l + 1, h);
//
//			if (_arrToPermute[l] != _arrToPermute[i])
//				count--;
//		}
//	}
//}
//
//vector<vector<int>> Permutation::permuted()
//{
//	vector<vector<int>> result;
//	permuted(0, _arrToPermute.size() - 1, result);
//
//	return result;
//}
//
//vector<int> Permutation::numberOfPermute()
//{
//	vector<int> result;
//	numberOfPermute(_arrToPermute, result, 0, _arrToPermute.size() - 1);
//
//	return result;
//}
