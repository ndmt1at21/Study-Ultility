#include "display.h"
#include <conio.h>

enum function
{
	HAM_BOOL,
	CONG_VECTOR,
	NHAN_VECTOR,
	DINH_THUC,
	NGHICH_DAO_MA_TRAN,
	TICH_MA_TRAN,
	RANK_MA_TRAN,
	HE_TUYEN_TINH,
	EXIT
};

void Display::run()
{
	bool toContinue = true;
	COORD cursor{ LEFT_CURSOR, TOP_Y};

	setCursor(0);
	while (toContinue)
	{
		clrscr();
		fileToBuffScr("display.txt", { 10, 0 });

		if (Keyboard::keyPressed())
		{
			int ch = Keyboard::getKey();

			switch (ch)
			{
			case KEY_DOWN:
				if (cursor.Y < BOT_Y)
					cursor.Y++;
				break;

			case KEY_UP:
				if (cursor.Y > TOP_Y)
					cursor.Y--;
				break;

			case KEY_ESC:
				toContinue = false;
				break;

			case KEY_ENTER:
			{
				setCursor(1);
				runFunction(cursor.Y - TOP_Y);
				setCursor(0);
			}
			}
		}

		strToBuffScr("->", cursor);
		output();
	}
}

void Display::runFunction(int numFunction)
{
	init(' ');
	output();
	
	switch (numFunction)
	{
	case HAM_BOOL:
	{
		Karnaugh* kar = new Karnaugh();
		kar->run(cin, cout);
		delete kar;
		break;
	}

	case DINH_THUC:
	{
		Matrix<double>* mtx = new Matrix<double>();
		mtx->run(cin, cout, "det");
		delete mtx;
		break;
	}

	case RANK_MA_TRAN:
	{
		Matrix<double>* mtx = new Matrix<double>();
		mtx->run(cin, cout, "rank");
		delete mtx;
		break;
	}

	case NGHICH_DAO_MA_TRAN:
	{
		Matrix<double>* mtx = new Matrix<double>();
		mtx->run(cin, cout, "inversion");
		delete mtx;
		break;
	}

	case HE_TUYEN_TINH:
	{
		Equations* equa = new Equations();
		equa->run(cin, cout);
		delete equa;
		break;
	}

	case NHAN_VECTOR:
	{
		Vector* vct = new Vector();
		vct->run(cin, cout, "multiAlpha");
		delete vct;
		break;
	}

	case CONG_VECTOR:
	{
		Vector* vct = new Vector();
		vct->run(cin, cout, "plusVct");
		break;
	}

	case TICH_MA_TRAN:
	{
		Matrix<double>* mtx = new Matrix<double>();
		mtx->run(cin, cout, "multiMtx");
		break;
	}
	
	case EXIT:
	{
		exit(0);
	}
	}
}