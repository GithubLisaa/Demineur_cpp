#include <iostream>
#include <ctime>
#include <windows.h>

using namespace std;

struct celldata{
	bool revealed;
	bool mine;
	bool flaged;
	int minesaround;
};

celldata gameboard[100][100];

enum bschoices {
	PETIT = 10,
	MOYEN = 18,
	GRAND = 24
};


float gennbint(int nb1, int nb2) {
	return floor(nb1 + ((rand() / (float)RAND_MAX) * nb2));
}

void color(int color) {
	switch (color)
	{
	case 1:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		break;
	case 2:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
		break;
	case 3:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
		break;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	}
}

void tempshowmine(int boardsize) {
	int nbmineonboard = 0;
	for (int y = 0; y < boardsize; y++)
	{
		for (int x = 0; x < boardsize; x++)
		{
			if (gameboard[y][x].mine)
			{
				cout << "[" << "*" << "]";
				nbmineonboard++;
			}
			else {
				cout << "[" << " " << "]";
			}
		}
		cout << endl;
	}
	cout << nbmineonboard << " mines" << endl;
}

void iniboard(int boardsize) { 
	int totalmine = 0;

	switch (boardsize)
	{
	case 10:
		totalmine = 10;
		break;
	case 18:
		totalmine = 40;
		break;
	case 24:
		totalmine = 99;
		break;
	default:
		break;
	}

	for (int i = 0; i < totalmine; i++)
	{
		int x = 0;
		int y = 0;
		bool ok = true;
		bool breakoutfor = false;

		x = gennbint(0, boardsize);
		y = gennbint(0, boardsize);
		
		for (int j = -2; j <= 2 && !breakoutfor; ++j) {
			for (int k = -2; k <= 2 && !breakoutfor; ++k) {
				if (y + j >= 0 && y + j < boardsize && x + k >= 0 && x + k < boardsize) {
					if (gameboard[y + j][x + k].revealed || gameboard[y][x].mine) {
						ok = false;
						breakoutfor = true;
					}
				}
			}
		}
		if (ok)
		{
			gameboard[y][x].mine = true;
		}
		else
		{
			i--;
		}
	}
}

void afboard(int boardsize) {
	for (int y = 0; y < boardsize; y++)
	{
		for (int x = 0; x < boardsize; x++)
		{
			if (gameboard[y][x].revealed)
			{
				if (gameboard[y][x].minesaround > 0) {
					color(2);
					cout << "[" << gameboard[y][x].minesaround << "]";
					color(4);
				}
				else {
					cout << "[" << " " << "]";
				}
			}
			else if (gameboard[y][x].flaged) {
				color(1);
				cout << "[" << "F" << "]";
				color(4);
			}
			else {
				cout << "[" << "?" << "]";
			}
		}
		cout << endl;
	}
}

void editcell(int boardsize, int type, int x, int y) {
	if (x > boardsize) {
		x = boardsize - 1;
	}
	else if (x < 0) {
		x = 0;
	}
	if (y > boardsize) {
		y = boardsize - 1;
	}
	else if (y < 0) {
		y = 0;
	}
	switch (type)
	{
	case 1:
		if (!gameboard[y][x].flaged)
		{
			gameboard[y][x].flaged = true;
		}
		else
		{
			gameboard[y][x].flaged = false;
		}
		break;
	case 2:
		gameboard[y][x].revealed = true;
	default:
		break;
	}
}

int main()
{
	srand(time(NULL));

	int boardsize = 0;
	int taille = 0;
	int select = 0;
	int x = 0;
	int y = 0;
	bool firstsele = true;

	gennbint(0, 0);
	gennbint(0, 0);
	gennbint(0, 0);

	cout << "Entrer une taille de tableau:\n1. PETIT\n2. MOYEN\n3. GRAND\n> ";
	cin >> taille;

	switch (taille)
	{
	case 1:
		boardsize = PETIT;
		break;
	case 2:
		boardsize = MOYEN;
		break;
	case 3:
		boardsize = GRAND;
		break;
	default:
		cout << "Erreur, element choisi incorrect";
		break;
	}

	while (true)
	{
		cout << endl << "Demineur\n1. Afficher le tableau\n2. Decouvrire une case\n3. Poser un drapeau\n>  ";
		cin >> select;

		switch (select)
		{
		case 1:
			afboard(boardsize);
			break;
		case 2:
			cout << "Entrer une coordonnee x et y\nEntrer x:\n> ";
			cin >> x;
			cout << "Entrer y:\n> ";
			cin >> y;

			if (firstsele) {
				editcell(boardsize, 2, x - 1, y - 1);
				iniboard(boardsize);
				firstsele = false;
			}
			else
			{
				editcell(boardsize, 2, x - 1, y - 1);
			}
			break;
		case 3:
			cout << "Entrer une coordonnee x et y\nEntrer x:\n> ";
			cin >> x;
			cout << "Entrer y:\n> ";
			cin >> y;
			editcell(boardsize, 1, x - 1, y - 1);
			break;
		case 4:
			tempshowmine(boardsize);
			break;
		default:
			cout << "Erreur, saissie incorrect";
			break;
		}
	}
}
