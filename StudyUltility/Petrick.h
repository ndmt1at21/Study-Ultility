#pragma once
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Petrick
{
public:
	string _p; // p co the la P1.P2.P3

public:
	Petrick()
	{
		_p = "";
	}

	Petrick(string p)
	{
		_p = p;
	}

	Petrick(char p)
	{
		_p = p;
	}

	Petrick(const Petrick& pet)
	{
		_p = pet._p;
	}

	//pet smaller than this
	bool isSubPetrick(Petrick pet);

	Petrick operator+(Petrick pet);
	Petrick operator*(Petrick pet);

	//phep nhan 2 list Petrick, trong moi list la 1 so hang
	//ex. list1 {AB, BC} -> hieu la AB+BC
	vector<Petrick> multi(vector<Petrick> list1, vector<Petrick> list2);

	//lam gon bieu thuc cong cac phan tu lai voi nhau
	//voi cac phan tu trong phep cong la 1 vector dau vao
	vector<Petrick> simplify(vector<Petrick>);

	//tham so dau vao la bang tableSimplify, nhung dong chua chi so hang phai co
	//va nhung cot xem nhu loai bo roi
	//tra ve cac te bao optional
	vector<vector<int>> groupOptional(vector<vector<int>> tableSimplify, 
		vector<int> idxRowMustHave, vector<int> idxColRemove);
};

