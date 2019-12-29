#include "ProgramFrame.h"
#include <conio.h>
#include <algorithm>

void ProgramFrame::startMessage(std::ostream& out)
{
	out << "Xin moi ban nhap du lieu" << std::endl;
}

void ProgramFrame::errorMessage(std::ostream& out)
{
	out << "Du lieu ban nhap khong chinh xac!" << std::endl;
}

bool ProgramFrame::askToContinue(std::istream& in, std::ostream& out)
{
	in.ignore();
	out << "Nhan 'Y' de tiep tuc, phim bat ki de thoat... ";

	char ch = _getch();

	if (ch == 'Y' || ch == 'y')
		return true;
	
	return false;
}

void ProgramFrame::processing(std::istream& in, std::string function) {}

void ProgramFrame::run(std::istream& in, std::ostream& out, std::string function)
{
	bool toContinue = true;

	while (toContinue)
	{
		startMessage(out);
		input(in);

		if (!validData())
		{
			errorMessage(out);
			continue;
		}

		processing(in, function);
		output(out, function);
		toContinue = askToContinue(in, out);

		system("cls");
	}
}