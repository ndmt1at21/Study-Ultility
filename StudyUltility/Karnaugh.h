#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "ProgramFrame.h"
#include "dec.h"

#define TERM_FILLED 1
#define NO_GROUP 1
#define TERM_FOUND -1
#define EMPTY 0

using namespace std;

struct filter
{
	vector<int> group;
	int row;
};

const vector<filter> groupSixteen{
	{{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  4}
};

const vector<filter> groupEight{
	{{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}, 4},
	{{1, 1, 1, 1, 1, 1, 1, 1}, 2}
};

const vector<filter> groupFour{
	{{1, 1, 0, 0, 1, 1}, 2},
	{{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, 4},
	{{1, 1, 1, 1}, 1},
	{{1, 0, 0, 1, 1, 0, 0, 1}, 2}
};

const vector<filter> groupTwo{
	{{1, 0, 0, 1}, 1},
	{{1, 1}, 1 }
};

const vector<filter> groupOne{
	{{1}, 1},
};

const vector<char> alphabet{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' , 'i', 'j', 'k' };

const vector<string> bit2 = { "00", "01", "11", "10" };
const vector<string> bit1 = { "0", "1" };

class Karnaugh :public ProgramFrame
{
private:
	vector<int> _karMap;
	vector<int> _valTerm;
	int _nVar;
	int _col;
	int _row;

	vector<string> _bitRow;
	vector<string> _bitCol;

protected:
	bool validData();

	//get idxRow & col from value term
	void getIdxKarMapFromTerm(string bit, int& idxRow, int& idxCol);

	//convert idx in kar to bit
	string toStrBit(int idxRow, int idxCol);

	//covert group array to string (ex: xyzt')
	string stringGroup(vector<int> oneGroup);

	//xem group Filter co la 1 group trong karMap ko
	//posBegin la vi tri bat dau kiem tra trong karMap (1D)
	bool isGroup(const filter groupFilter, int posBegin);

	//xoay karmap & vector (chuyen thanh matrix roi xoay)
	vector<int> transform(vector<int> vct);
	vector<int> transform();
	
	//lengthcua ham bool (bo cac dau ' di)
	int boolLength(string boolean);

	//so te bao trong 1 kar ( o day la so cac TERM_FILLED (1))
	int numberTerm(vector<int> kar);
	int numberTerm();

	//dau vao cac cac group filter
	//ra la 1 danh sach cac group, moi group gom row*col phan tu
	//va chua groupN te bao (dc danh dau TERM_FILLED)
	//tim kiem nhung te bao tu 16->4 (tim tat ca)
	vector<vector<int>> findGroup16to2(const vector<filter> group, int groupN);
	vector<vector<int>> findGroup1(const vector<filter> group);

	//tim kiem nhung te bao tu 16->4 (duyet theo dong)
	//dau ra vao tuong tu tren
	vector<vector<int>> findGroup16to4ByRow(const vector<filter> groupsFilter);
	vector<vector<int>> findGroup2ByRow(const vector<filter> groupsFilter);

	//group nay co la con group khac ko
	bool isSubGroup(vector<int> group1, vector<int> group2);

	//loai bo cac group giong nhau, chua nhau trong list cac group
	vector<vector<int>> removeDup(vector<vector<int>> listGroups);

public:
	Karnaugh();
	Karnaugh(int row, int col); //init karnaugh with all value 0
	Karnaugh(vector<int>, int);
	Karnaugh(const Karnaugh&);

	void input(istream& in);
	void output(ostream& out, string function = "");

	//tim tat ca cac group co the
	vector<vector<int>> findGroup(int groupN);
	
	//tim cong thuc toi tieu
	vector<string> simplify();
};
