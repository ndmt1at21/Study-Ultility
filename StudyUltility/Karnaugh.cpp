#include "Karnaugh.h"
#include <algorithm>
#include "Matrix.h"
#include "Petrick.h"
#include <conio.h>
Karnaugh::Karnaugh()
{
	_karMap.resize(0);
	_karMap.resize(0);

	_col = 0;
	_row = 0;
	_nVar = 0;

	_bitRow = bit1;
	_bitCol = bit1;
}

Karnaugh::Karnaugh(int row, int col)
{
	_nVar = row / 2 + col / 2;
	_row = row;
	_col = col;

	_karMap.resize(_row * _col);
}

Karnaugh::Karnaugh(vector<int> valTerm, int nVar)
{
	_nVar = nVar;
	_col = (nVar - nVar / 2) * 2;
	_row = nVar * 2 - _col;

	if (_col == 2)
		_bitCol = bit1;
	else if (_col == 4)
		_bitCol = bit2;

	if (_row == 2)
		_bitRow = bit1;
	else if (_row == 4)
		_bitRow = bit2;

	//allocated karMap
	if (_row < 0 || _col < 0)
		return;

	_karMap.resize(_row * _col);

	for (int i = 0; i < valTerm.size(); i++)
	{
		int idxRow = 0;
		int idxCol = 0;

		Dec a(valTerm[i]);
		string bit = a.to_bin(_nVar);

		//lay chi so cua 1 o term trong karmap
		getIdxKarMapFromTerm(bit, idxRow, idxCol);

		//neu chi so hop le thi dien vao bang karmap
		if (idxRow >= 0 && idxCol >= 0 && idxRow < _row && idxCol < _col)
		{
			_karMap[idxRow * _col + idxCol] = TERM_FILLED;
			_valTerm.push_back(idxRow * _col + idxCol);
		}

	}
}

Karnaugh::Karnaugh(const Karnaugh& kar)
{
	_valTerm = kar._valTerm;
	_karMap = kar._karMap;

	_nVar = kar._nVar;
	_col = kar._col;
	_row = kar._col;

	_bitRow = kar._bitRow;
	_bitCol = kar._bitCol;
}

void Karnaugh::getIdxKarMapFromTerm(string bit, int& idxRow, int& idxCol)
{
	string bitRow, bitCol;
	bitRow = bit.substr(0, _row / 2);
	bitCol = bit.substr(_row / 2, _col / 2);

	vector<string>::iterator it;

	//find bit in _bitRow & _bitCol
	//return idxRow, idxCol in karMap
	it = find(_bitRow.begin(), _bitRow.end(), bitRow);
	idxRow = it - _bitRow.begin();

	it = find(_bitCol.begin(), _bitCol.end(), bitCol);
	idxCol = it - _bitCol.begin();
}

bool Karnaugh::isGroup(const filter groupFilter, int posBegin)
{
	int filterSize = groupFilter.group.size();
	
	//neu filter lon hon karmap dung (VD filter 16 > kamap(2x4))
	if (_karMap.size() < filterSize)
		return false;

	//neu chi so bat dau xet trong karmap + size filter
	// vuot ra ngoai luon khoi xet tiep
	if (posBegin + filterSize > _row * _col)
		return false;

	//te bao ko nhay sang hang moi, nhieu hang so voi hang quy dinh cua te bao
	int iRowBegin = posBegin / _col;
	int iRowEnd = (posBegin + filterSize - 1) / _col;

	if (iRowEnd - iRowBegin + 1 != groupFilter.row)
		return false;

	//count num term in one group
	int nTermGroup = 0; 
	nTermGroup = numberTerm(groupFilter.group);

	//gia tri flag, xem cac vi tri tuong ung cua groupfilter trong bang karmap
	//co nam trong group khac ko?
	int valTermStart = _karMap[posBegin];
	
	//count num term equal valTermStart
	int nTermFound = 0; 

	for (int i = 0; i < filterSize; i++)
	{
		if (groupFilter.group[i] == TERM_FILLED)
		{
			if (_karMap[i + posBegin] == 0)
				return false;

			if (valTermStart != NO_GROUP
				&&_karMap[i + posBegin] == valTermStart)
				nTermFound++;
		}
	}

	//neu tat ca cac o trong group deu cung 1 gia tri trong bang karMap 
	//-> nam trong group khac roi
	if (nTermFound == nTermGroup) 
		return false;

	return true;
}

vector<int> Karnaugh::transform(vector<int> vct)
{
	Matrix<int> mtx(vct, _row, _col);
	mtx = mtx.transform();

	vector<int> result = mtx.toArr1D();
	return result;
}

vector<int> Karnaugh::transform()
{
	Matrix<int> mtx(_karMap, _row, _col);
	mtx = mtx.transform();

	swap(_row, _col);

	vector<int> result = mtx.toArr1D();
	return result;
}

vector<vector<int>> Karnaugh::findGroup(int groupN)
//tra ve danh sach (danh dau TERM_FILLED) cac group (1 group la 1 mang)
{
	vector<filter> groupFilter;
	vector<vector<int>> listFilter;

	switch (groupN)
	{
	case 16:
		groupFilter = groupSixteen;
		break;
	case 8:
		groupFilter = groupEight;
		break;
	case 4:
		groupFilter = groupFour;
		break;
	case 2:
		groupFilter = groupTwo;
		break;
	default:
		groupFilter = groupOne;
		break;
	}

	if (this->numberTerm() == _row * _col)
	{
		listFilter.resize(1);
		listFilter[0].resize(_row * _col, TERM_FILLED);
	}
	else if (groupN >= 2 && groupN <= 16)
		listFilter = findGroup16to2(groupFilter, groupN);
	else if (groupN == 1)
		listFilter = findGroup1(groupFilter);

	return listFilter;
}

vector<vector<int>> Karnaugh::findGroup16to2(const vector<filter> groupFilter, int groupN)
{
	vector<vector<int>> listGroupRow;

	//row
	if (groupN > 2 && groupN <= 16)
		listGroupRow = findGroup16to4ByRow(groupFilter);
	else if (groupN == 2)
		listGroupRow = findGroup2ByRow(groupFilter);

	//lat bang karnaugh lai
	_karMap = transform();

	//col
	vector<vector<int>> listGroupCol;

	if (groupN > 2)
		listGroupCol = findGroup16to4ByRow(groupFilter);
	else if (groupN == 2)
		listGroupCol = findGroup2ByRow(groupFilter);

	for (int i = 0; i < listGroupCol.size(); i++) //xoay nhung group tim kiem lai
		listGroupCol[i] = transform(listGroupCol[i]);

	//update kar1d nhung phan tu nao da tim kiem roi
	_karMap = transform();

	//concat 2 list
	vector<vector<int>> listGroup;

	for (int i = 0; i < listGroupRow.size(); i++)
		listGroup.push_back(listGroupRow[i]);

	for (int i = 0; i < listGroupCol.size(); i++)
		listGroup.push_back(listGroupCol[i]);


	return listGroup;
}

vector<vector<int>> Karnaugh::findGroup16to4ByRow(const vector<filter> groupsFilter)
{
	vector<vector<int>> listGroup;
	int nRes = 0;

	//xet dong
	for (int idxCase = 0; idxCase < groupsFilter.size(); idxCase++)
	{
		for (int i = 0; i < _row; i++)
		{
			for (int j = 0; j < _col - 1; j++)
			{
				int posBegin = i * _col + j;
				if (isGroup(groupsFilter[idxCase], posBegin))
				{
					nRes++;
					listGroup.resize(nRes);

					// khoi tao 1 mang kq toan so 0
					listGroup[nRes - 1].resize(_row * _col, 0); 

					//dien vi tri cac te bao nho 
					//trong 1 te bao lon ra mang ket qua
					for (int k = 0; k < groupsFilter[idxCase].group.size(); k++)
						listGroup[nRes - 1][posBegin + k] = groupsFilter[idxCase].group[k];

	/*				setFound(groupsFilter[idxCase], posBegin);*/
				}
			}
		}
	}

	return listGroup;
}

vector<vector<int>> Karnaugh::findGroup2ByRow(const vector<filter> groupsFilter)
{
	vector<vector<int>> listGroup;
	int nRes = 0;

	for (int idxCase = 0; idxCase < groupsFilter.size(); idxCase++)
	{
		for (int i = 0; i < _row; i++) //xet tung o trong kar
		{
			for (int j = 0; j < _col - 1; j++)
			{
				int posBegin;
				if (idxCase == 0) //th 2 so cach xa nhau
					posBegin = i * _col;
				else //2 so lien tiep
					posBegin = i * _col + j;

				if (isGroup(groupsFilter[idxCase], posBegin))
				{
					nRes++;
					listGroup.resize(nRes);
					listGroup[nRes - 1].resize(_row * _col, 0);

					//dien vao cac TERM_FILLED vo trong 1 mang ket qua
					for (int k = 0; k < groupsFilter[idxCase].group.size(); k++)
						listGroup[nRes - 1][posBegin + k] = groupsFilter[idxCase].group[k];

					//setFound(groupsFilter[idxCase], posBegin);
				}
			}
		}
	}

	return listGroup;
}

vector<vector<int>> Karnaugh::findGroup1(const vector<filter> groupFilter)
{
	vector<vector<int>> listGroup;
	int nRes = 0;

	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _col; j++)
			if (isGroup(groupFilter[0], i * _col + j))
			{
				nRes++;
				listGroup.resize(nRes);
				listGroup[nRes - 1].resize(_row * _col, 0);

				listGroup[nRes - 1][i * _col + j] = TERM_FILLED;
			/*	setFound(groupFilter[0], i * _col + j);*/
			}
	}

	return listGroup;
}


string Karnaugh::toStrBit(int idxRow, int idxCol)
{
	string result;
	result += _bitRow[idxRow] + _bitCol[idxCol];

	return result;
}

string Karnaugh::stringGroup(vector<int> oneGroup) //covert 1 oneGroup array to string (ex: xyzt')
{
	vector<string> listBitTerm;

	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _col; j++)
		{
			if (oneGroup[i * _col + j] == TERM_FILLED)
				listBitTerm.push_back(toStrBit(i, j));
		}
	}

	//tim diem chung trong chuoi bit
	string result;
	string pivot = listBitTerm[0];
	bool isCommon = false; //xem ki tu cua pivot co chung cho tat ca cac chuoi trong list ko
	for (int i = 0; i < pivot.length(); i++) //lay chuoi bit dau lam moc
	{
		for (int j = 0; j < listBitTerm.size(); j++) // so sanh voi cac chuoi bit con lai
		{
			if (listBitTerm[j][i] == pivot[i])
				isCommon = true;
			else
			{
				isCommon = false; //1 phan tu trong list khac voi phan tu pivot ->false
				break;
			}
		}

		if (isCommon) //neu duyet list ma bit chung
		{
			if (pivot[i] == '0') //so 0 thi them dau '. ex: a'
				result = result + alphabet[i] + "\'";
			else
				result = result + alphabet[i]; //alphabet[i]: lay chu cai de bieu dien ham
		}
	}

	if (result == "")
		result = "1";

	return result;
}

int Karnaugh::numberTerm(vector<int> kar) //so cac te bao vector
{
	int count = 0;

	for (int i = 0; i < kar.size(); i++)
		if (kar[i] != 0)
			count++;

	return count;
}

int Karnaugh::numberTerm() //so cac te bao trong karmap
{
	return numberTerm(_karMap);
}

vector<string> Karnaugh::simplify()
{
	//find all groups in karmap
	vector<vector<int>> allGroups;
	for (int i = 16; i >= 1; i /= 2)
	{
		vector<vector<int>> groups = findGroup(i);
		groups = removeDup(groups);

		for (int j = 0; j < groups.size(); j++)
			allGroups.push_back(groups[j]);
	}

	//remove duplicate
	allGroups = removeDup(allGroups);

	//tao bang simplify
	int rowSimplify = allGroups.size();
	int colSimplify = _valTerm.size();
	
	vector<vector<int>> tableSimplify;
	tableSimplify.resize(rowSimplify);
	for (int i = 0; i < rowSimplify; i++)
		tableSimplify[i].resize(colSimplify, 0);

	// dien vao bang neu vi tri cua term xuat hien trong group
	for (int i = 0; i < rowSimplify; i++)
	{
		for (int j = 0; j < colSimplify; j++)
		{
			if (allGroups[i][_valTerm[j]] == TERM_FILLED)
				tableSimplify[i][j] = TERM_FILLED;
		}
	}

	//tim vi tri nhung group bat buoc se co trong cong thuc toi tieu
	vector<int> idxRowMustHave;

	for (int i = 0; i < colSimplify; i++)
	{
		//dem nhung vi tri danh dau trong 1 cot
		int colCount = 0;
		
		int idxRow;
		for (int j = 0; j < rowSimplify; j++)
		{
			if (tableSimplify[j][i] == TERM_FILLED)
			{
				idxRow = j;
				colCount++;
			}
		}

		//neu trong 1 cot chi co 1 danh dau -> chon
		if (colCount == 1 &&
			find(idxRowMustHave.begin(), idxRowMustHave.end(), idxRow) == idxRowMustHave.end())
		{
			idxRowMustHave.push_back(idxRow);
		}
	}

	//tim te bao con lai trong cot ko bi gach di
	// nhung cot bi gach di
	vector<int> idxColRemove;

	//duyet cac cot
	for (int i = 0; i < idxRowMustHave.size(); i++)
	{
		for (int j = 0; j < colSimplify; j++)
		{
			int row = idxRowMustHave[i];

			if (tableSimplify[row][j] == TERM_FILLED)
			{
				if (find(idxColRemove.begin(), idxColRemove.end(), j) == idxColRemove.end())
					idxColRemove.push_back(j);
			}
		}
	}

	//tim cong thuc chung, phai co trong cong thuc
	string grMustHave;
	for (int i = 0; i < idxRowMustHave.size(); i++)
	{
		int row = idxRowMustHave[i];
		string strGr = stringGroup(allGroups[row]);
		
		if (grMustHave == "")
			grMustHave += strGr;
		else
			grMustHave += " + " + strGr;
	}

	//tim cong thu rieng (optional)
	Petrick tmp;
	vector<vector<int>> idxRowGrsOptional;
	idxRowGrsOptional = tmp.groupOptional(tableSimplify, idxRowMustHave, idxColRemove);
	vector<string> optionals;

	for (int i = 0; i < idxRowGrsOptional.size(); i++)
	{
		string oneOptional;
		for (int j = 0; j < idxRowGrsOptional[i].size(); j++)
		{
			int row = idxRowGrsOptional[i][j];
			string oneGroup = stringGroup(allGroups[row]);
			if (oneOptional == "")
				oneOptional += oneGroup;
			else
				oneOptional += " + " + oneGroup;
		}

		optionals.push_back(oneOptional);
	}

	//tim tat ca cong thuc co the
	vector<string> allBool;
	for (int i = 0; i < optionals.size(); i++)
	{
		string oneBool = grMustHave;

		if (oneBool == "")
			oneBool += optionals[i];
		else
			oneBool += " + " + optionals[i];

		allBool.push_back(oneBool);
	}

	if (allBool.size() == 0)
		allBool.push_back(grMustHave);

	//tim cong thuc toi tieu
	int minLength = 0;
	if (allBool.size() > 0)
		minLength = boolLength(allBool[0]);

	//get min length
	for (int i = 0; i < allBool.size(); i++)
	{
		if (minLength > boolLength(allBool[i]))
			minLength = boolLength(allBool[i]);
	}

	//get result (bool length) == min length
	vector<string> result;
	for (int i = 0; i < allBool.size(); i++)
	{
		if (boolLength(allBool[i]) == minLength)
			result.push_back(allBool[i]);
	}

	return result;
}

bool Karnaugh::isSubGroup(vector<int> grLarge, vector<int> grSmall)
{
	int count = 0;
	for (int i = 0; i < grLarge.size(); i++)
	{
		if (grSmall[i] == TERM_FILLED && grLarge[i] != TERM_FILLED)
			return false;
	}
	
	return true;
}

//loai bo cac group giong nhau trong list
vector<vector<int>> Karnaugh::removeDup(vector<vector<int>> listGroups)
{
	vector<vector<int>> result;

	if (listGroups.size() == 0)
		return result;

	//danh dau group do da dc xu li qua chua
	//neu xu li roi bo qua ko xet lai
	vector<bool> checked;
	checked.resize(listGroups.size(), false);

	int nGroup = listGroups.size();
	for (int i = 0; i < nGroup - 1; i++)
	{
		if (checked[i] == false)
		{
			result.push_back(listGroups[i]);
			for (int j = i + 1; j < nGroup; j++)
			{
				if (isSubGroup(listGroups[i], listGroups[j]))
					checked[j] = true;
			}
		}
	}

	if (checked[nGroup - 1] == false)
		result.push_back(listGroups[nGroup - 1]);

	return result;
}

int Karnaugh::boolLength(string boolean)
{
	int len = 0;
	for (int i = 0; i < boolean.length(); i++)
	{
		if (boolean[i] != '\'')
			len++;
	}

	return len;
}

bool Karnaugh::validData()
{
	if (_nVar < 1 && _nVar>4)
		return false;

	if (_col < 0 || _row < 0 || _col > 4 || _row > 4)
		return false;

	for (int i = 0; i < _valTerm.size(); i++)
		if (_valTerm[i] > pow(2, _nVar) || _valTerm[i] < 0)
			return false;

	return true;
}

void Karnaugh::input(istream& in)
{
	cout << "Nhap so bien: ";
	int nVar;
	in >> nVar;

	vector<int> valTerm;

	cout << "Nhap vi tri term 1 (-1 de ngung): ";
	int ch;
	cin >> ch;

	int i = 2;
	while (ch != -1)
	{
		cout << "Nhap vi tri term " << i << ": ";
		valTerm.push_back(ch);
		cin >> ch;
		i++;
	}

	*this = Karnaugh(valTerm, nVar);
}

void Karnaugh::output(ostream& out, string function)
{
	if (function != "")
		return;

	vector<string> result = simplify();

	for (int i = 0; i < result.size(); i++)
		cout << result[i] << endl;
}