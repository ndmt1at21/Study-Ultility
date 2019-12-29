#include "Petrick.h"
#define TERM_FILLED 1

//pet smaller than this
//tat ca ki tu cua pet nam trong this -> subPetrick
bool Petrick::isSubPetrick(Petrick pet)
{
	//dem cac ki tu trong pet xuat hien o this
	int count = 0;
	for (int i = 0; i < pet._p.length(); i++)
	{
		if (_p.find(pet._p[i]) != string::npos)
			count++;
	}

	if (count == pet._p.length())
		return true;

	return false;
}

Petrick Petrick::operator+(Petrick pet) //ex. X + X = X; X + XY = X
{
	if (_p == pet._p)
		return _p;

	Petrick pet1; //smaller
	Petrick pet2;

	//get smaller and larger between two Petrick var
	if (_p.length() > pet._p.length())
	{
		pet1 = pet;
		pet2 = *this;
	}
	else
	{
		pet1 = *this;
		pet2 = pet;
	}

	Petrick result;
	if (pet2.isSubPetrick(pet1))
		result = pet1;

	return result;
}

//X*X=X
Petrick Petrick::operator*(Petrick pet)
{
	if (_p == pet._p)
		return pet;

	//concat 2 petrick
	Petrick result = *this;
	result._p += pet._p;

	sort(result._p.begin(), result._p.end());

	//remove duplicate (ex: XX = X)
	result._p.erase(unique(result._p.begin(), result._p.end()), result._p.end());
	
	return result;
}

//thuc hien phep nhan bieu thuc (moi bieu thuc la 1 vector kieu Petrick)
//ex. list1: AB, BC
//	  list2: AD, AC -> ham multi dc hieu la se thuc hien 
//    (AB + BC)*(AD+AC)
//tra ve ket qua rut gon cua phep tinh nhan do
vector<Petrick> Petrick::multi(vector<Petrick> list1, vector<Petrick> list2)
{
	if (list1.size() == 0)
		return list2;

	vector<Petrick> result;

	//nhan (X+Y)(B+C) -> XB + XC + YB + YC
	for (int i = 0; i < list1.size(); i++)
	{
		for (int j = 0; j < list2.size(); j++)
			result.push_back(list1[i] * list2[j]);
	}

	result = simplify(result);
	return result;
}

//lam gon bieu thuc cong cac phan tu lai voi nhau
//voi cac phan tu trong phep cong la 1 vector dau vao
vector<Petrick> Petrick::simplify(vector<Petrick> listPet)
{
	vector<bool> isPlus;
	isPlus.resize(listPet.size(), false);

	//tim kiem nhung phan tu cong vs nhau dc trong listPet
	for (int i = 0; i < listPet.size(); i++)
	{
		//bo qua nhung phan tu da dc cong roi
		if (isPlus[i] == true)
			continue;

		for (int j = 0; j < listPet.size(); j++)
		{
			//ko tu cong chinh minh va listPer[j] phai nam trong listpet[i]
			//ex: ABCD va ACD -> cong dc (vi ACD nam trong ABCD)
			if (j != i && listPet[i].isSubPetrick(listPet[j]))
			{
				listPet[i] = listPet[i] + listPet[j];
				isPlus[j] = true;
			}
		}
	}

	vector<Petrick> result;
	for (int i = 0; i < isPlus.size(); i++)
	{
		//chua bi cong vs so nao ca
		if (isPlus[i] == false)
			result.push_back(listPet[i]);
	}

	return result;
}

//tra ve chi so dong cua cac nhom trong bang, moi 1 vector la 1 nhom
vector<vector<int>> Petrick::groupOptional(vector<vector<int>> tableSimplify, 
		vector<int> idxRowMustHave, vector<int> idxColRemove)
{
	//khoi tao chu cai ung vs cac idx con lai
	vector<char> alpha;
	//alpha.resize(tableSimplify.size() - idxRowMustHave.size());

	char ch = 'A';
	for (int i = 0; i < tableSimplify.size(); i++)
	{
		alpha.push_back(ch);
		ch += 1;
	}

	//ex
	//tableSimplify
	//		1	2 
	// A	*
	// B
	// C	*	*
	//listResult = (A+C)*C (di nhien da bo qua cac cot idxColRemove)
	vector<Petrick> listResult;

	int colTable = 0;
	if (tableSimplify.size() > 0)
		colTable = tableSimplify[0].size();

	for (int i = 0; i < colTable; i++)
	{
		//neu cot da dc danh dau thi bo qua
		if (find(idxColRemove.begin(), idxColRemove.end(), i) != idxColRemove.end())
			continue;

		//thuc hien phep cong cac o co danh dau trong 1 cot
		vector<Petrick> listCol;
		for (int j = 0; j < tableSimplify.size(); j++)
		{
			if (tableSimplify[j][i] == TERM_FILLED)
			{
				Petrick tmp(alpha[j]);
				listCol.push_back(tmp);
			}
		}

		listResult = multi(listResult, listCol);
	}

	//find petrick min lengtht
	vector<Petrick> min;
	int minLength = 0;
	if (listResult.size() > 0)
		minLength = listResult[0]._p.length();

	for (int i = 0; i < listResult.size(); i++)
	{
		if (minLength > listResult[i]._p.length())
			minLength = listResult[i]._p.length();
	}

	//tim nhung
	for (int i = 0; i < listResult.size(); i++)
	{
		if (listResult[i]._p.length() == minLength)
			min.push_back(listResult[i]);
	}

	//tim tap hop cac dong trong tableSimlify tao nen bieu thuc bool tu chon.
	//ex. idxRowOptional:
	// 0 2 4 -> x + y + z -> 1 group tu chon
	// 1 3 5 -> a + b + c -> 1 group tu chon . Voi x,y,z,a,b,c la 1 group
	vector<vector<int>> idxRowOptional;
	for (int i = 0; i < min.size(); i++)
	{
		vector<int> idxRowOnes;
		for (int j = 0; j < min[i]._p.length(); j++)
			idxRowOnes.push_back(min[i]._p[j] - alpha[0]);

		idxRowOptional.push_back(idxRowOnes);
	}

	return idxRowOptional;
}
