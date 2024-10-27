#include <iostream>
#include <ctime>
#include <windows.h>

using namespace std;

struct celldata {
	bool revealed;
	bool mine;
	bool flaged;
	int minesaround;
};

celldata gameboard[9999][9999];

enum bschoices {
	PETIT = 10,
	MOYEN = 18,
	GRAND = 24
};

int celltofind = 0;
int cellfinds = 0;
int boardsize = 0;
int x = 0;
int y = 0;
int nbminemod = 0;
bool ismodded = false;

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

void cheatshowmine() {
	int nbmineonboard = 0;
	for (int y = 0; y < boardsize; y++)
	{
		for (int x = 0; x < boardsize; x++)
		{
			if (gameboard[y][x].mine)
			{
				color(1);
				cout << "[" << "*" << "]";
				color(4);
				nbmineonboard++;
			}
			else if (gameboard[y][x].minesaround > 0) {
				color(2);
				cout << "[" << gameboard[y][x].minesaround << "]";
				color(4);
			}
			else
			{
				cout << "[" << " " << "]";
			}
		}
		cout << endl;
	}
	cout << nbmineonboard << " mines" << endl;
}

void iniboard() {
	int totalmine = 0;

	if (ismodded) {
		totalmine = nbminemod;
	}
	else
	{
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

	for (int y = 0; y < boardsize; y++)
	{
		for (int x = 0; x < boardsize; x++)
		{
			for (int j = -1; j <= 1; ++j) {
				for (int k = -1; k <= 1; ++k) {
					if (y + j >= 0 && y + j < boardsize && x + k >= 0 && x + k < boardsize) {
						if (gameboard[y + j][x + k].mine) {
							gameboard[y][x].minesaround++;
						}
					}
				}
			}
		}
	}
}

void afboard() {
	cellfinds = 0;
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
					cellfinds++;
				}
				else {
					cout << "[" << " " << "]";
					cellfinds++;
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

void checkcell(int x, int y) {
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			int combi = i * 10 + j;
			if (y + i >= 0 && y + i < boardsize && x + j >= 0 && x + j < boardsize) {
				if (gameboard[y][x].minesaround == 0) {
					if (gameboard[y + i][x + j].minesaround == 0 && !gameboard[y + i][x + j].revealed)
					{
						gameboard[y + i][x + j].revealed = true;
						checkcell(x + j, y + i);
						/*switch (combi) {
						case 11:
							if (gameboard[y + 1][x].minesaround == 0 || gameboard[y][x + 1].minesaround == 0) {
								gameboard[y + i][x + j].revealed = true;
								checkcell(x + j, y + i);
							}
							break;
						case 9:
							if (gameboard[y + 1][x].minesaround == 0 || gameboard[y][x - 1].minesaround == 0) {
								gameboard[y + i][x + j].revealed = true;
								checkcell(x + j, y + i);
							}
							break;
						case -9:
							if (gameboard[y - 1][x].minesaround == 0 || gameboard[y][x + 1].minesaround == 0) {
								gameboard[y + i][x + j].revealed = true;
								checkcell(x + j, y + i);
							}
							break;
						case -11:
							if (gameboard[y - 1][x].minesaround == 0 || gameboard[y][x - 1].minesaround == 0) {
								gameboard[y + i][x + j].revealed = true;
								checkcell(x + j, y + i);
							}
							break;
						default:
							gameboard[y + i][x + j].revealed = true;
							checkcell(x + j, y + i);
							break;
						}*/
					}
					else if (!gameboard[y + i][x + j].mine) {
						gameboard[y + i][x + j].revealed = true;
						//switch (combi) {
						//case 11: //1, 1
						//	if (!gameboard[y + 1][x].mine && !gameboard[y][x + 1].mine) {
						//		gameboard[y + i][x + j].revealed = true;
						//	}
						//	break;
						//case 9: //1, -1
						//	if (!gameboard[y + 1][x].mine && !gameboard[y][x - 1].mine) {
						//		gameboard[y + i][x + j].revealed = true;
						//	}
						//	break;
						//case -9://-1, 1
						//	if (!gameboard[y - 1][x].mine && !gameboard[y][x + 1].mine) {
						//		gameboard[y + i][x + j].revealed = true;
						//	}
						//	break;
						//case -11://-1, -1
						//	if (!gameboard[y - 1][x].mine && !gameboard[y][x - 1].mine) {
						//		gameboard[y + i][x + j].revealed = true;
						//	}
						//	break;
						//default:
						//	gameboard[y + i][x + j].revealed = true;
						//	break;
						//}
					}
				}
				else
				{
					gameboard[y][x].revealed = true;
				}
			}
		}
	}
}

bool editcell(int type) {
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
		if (gameboard[y][x].mine == true) {
			color(1);
			cout << "Game Over" << endl;
			color(4);
			for (int y = 0; y < boardsize; y++)
			{
				for (int x = 0; x < boardsize; x++)
				{
					if (gameboard[y][x].mine)
					{
						color(1);
						cout << "[" << "*" << "]";
						color(4);
					}
					else
					{
						cout << "[" << " " << "]";
					}
				}
				cout << endl;
			}
			return true;
			break;
		}
		else if (gameboard[y][x].revealed == true)
		{
			color(1);
			cout << "Erreur, case deja decouverte" << endl;
			color(4);
		}
		else {
			checkcell(x, y);
		}
		return false;
		break;
	case 3:
		gameboard[y][x].revealed = true;
		break;
	default:
		break;
	}
}

int main()
{
	srand(time(NULL));

	int taille = 0;
	int select = 0;
	bool end = false;
	bool firstsele = true;

	gennbint(0, 0);
	gennbint(0, 0);
	gennbint(0, 0);

	for (int i = 0; i < 1; i++) {
		cout << "Entrer une taille de tableau:\n1. Petit (10*10)\n2. Moyen (18*18)\n3. Grand (24*24)\n4. Moddee\n> ";
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
		case 4:
			char seeda;
			int seedmod;
			int maxmines;
			ismodded = true;
			cout << "Entrer votre taille de tableau personnalisee (max 9999*9999, min 6*6)\n> ";
			cin >> boardsize;
			if (boardsize > 9999) {
				boardsize = 9999;
			}
			else if (boardsize < 6) {
				boardsize = 6;
			}
			maxmines = boardsize * boardsize - 25;
			cout << "Entrer votre nombre de mines, max " << maxmines << "\n> ";
			cin >> nbminemod;
			if (nbminemod > maxmines) {
				nbminemod = maxmines;
			}
			cout << "Voulez vous rentrer une seed custom ? (y/n)\n> ";
			cin >> seeda;
			if (seeda == 'y') {
				cout << "Entrer votre seed\n> ";
				cin >> seedmod;
				srand(seedmod);
			}
			break;
		default:
			color(1);
			cout << endl << "Erreur, saissie incorrect" << endl << endl;
			color(4);
			i--;
			break;
		}
	}

	switch (boardsize)
	{
	case 10:
		celltofind = boardsize * boardsize - 10;
		break;
	case 18:
		celltofind = boardsize * boardsize - 40;
		break;
	case 24:
		celltofind = boardsize * boardsize - 99;
		break;
	default:
		celltofind = boardsize * boardsize - nbminemod;
		break;
	}

	while (!end)
	{
		cout << endl;
		afboard();
		if (!firstsele) {
			if (celltofind - cellfinds == 0) {
				color(3);
				cout << "Bravo!\nfin de partie";
				color(4);
				end = true;
			}
			else
			{
				cout << celltofind - cellfinds << " cases restantes a trouver\nVotre derniere entree etait en x: " << x + 1 << ", y: " << y + 1 << endl;
			}
		}
		if (!end)
		{
			color(3);
			cout << endl << "Demineur" << endl;
			color(4);
			cout << "1. Decouvrire une case\n2. Poser un drapeau\n> ";
			cin >> select;

			switch (select)
			{
			case 1:
				cout << "Entrer une coordonnee x et y:\nEntrer x:\n> ";
				cin >> x;
				cout << "Entrer y:\n> ";
				cin >> y;
				x--;
				y--;
				if (firstsele) {
					editcell(3);
					iniboard();
					checkcell(x, y);
					firstsele = false;
				}
				else
				{
					end = editcell(2);
				}
				break;
			case 2:
				cout << "Entrer une coordonnee x et y:\nEntrer x:\n> ";
				cin >> x;
				cout << "Entrer y:\n> ";
				cin >> y;
				x--;
				y--;
				editcell(1);
				break;
			case 3:
				cheatshowmine();
				break;
			default:
				color(1);
				cout << endl << "Erreur, saissie incorrect" << endl;
				color(4);
				break;
			}
		}
	}
}