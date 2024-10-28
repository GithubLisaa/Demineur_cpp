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
int x = -1;
int y = -1;
int nbminemod = 0;
bool ismodded = false;

float gennbint(int nb1, int nb2) {
	return floor(nb1 + ((rand() / (float)RAND_MAX) * nb2));
}

//void color(int color) {
//	switch (color)
//	{
//	case 1:
//		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
//		break;
//	case 2:
//		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
//		break;
//	case 3:
//		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
//		break;
//	default:
//		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
//		break;
//	}
//}

//std::cout << "\033[<style>;<couleurtexte>;<background>mTexte\033[0m" << std::endl;


void cheatshowmine() {
	int nbmineonboard = 0;
	for (int y = 0; y < boardsize; y++)
	{
		for (int x = 0; x < boardsize; x++)
		{
			if (gameboard[y][x].mine)
			{
				cout << "\033[1;31;102m[" << "*" << "]\033[0m";
				nbmineonboard++;
			}
			else if (gameboard[y][x].revealed) {
				if (gameboard[y][x].minesaround > 0) {
					switch (gameboard[y][x].minesaround)
					{
					case 1:
						cout << "\033[1;34;103m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					case 2:
						cout << "\033[1;32;103m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					case 3:
						cout << "\033[1;31;103m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					case 4:
						cout << "\033[1;95;103m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					case 5:
						cout << "\033[1;33;103m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					case 6:
						cout << "\033[1;36;103m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					case 7:
						cout << "\033[1;30;103m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					default:
						cout << "\033[1;90;103m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					}
				}
				else
				{
					cout << "\033[1;30;103m[" << " " << "]\033[0m";
				}
			}
			else
			{
				if (gameboard[y][x].minesaround > 0) {
					switch (gameboard[y][x].minesaround)
					{
					case 1:
						cout << "\033[1;34;102m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					case 2:
						cout << "\033[1;32;102m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					case 3:
						cout << "\033[1;31;102m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					case 4:
						cout << "\033[1;95;102m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					case 5:
						cout << "\033[1;33;102m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					case 6:
						cout << "\033[1;36;102m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					case 7:
						cout << "\033[1;30;102m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					default:
						cout << "\033[1;90;102m[" << gameboard[y][x].minesaround << "]\033[0m";
						break;
					}
				}
				else
				{
					cout << "\033[1;30;102m[" << " " << "]\033[0m";
				}
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
	for (int y1 = 0; y1 < boardsize; y1++)
	{
		for (int x1 = 0; x1 < boardsize; x1++)
		{
			if (gameboard[y1][x1].revealed)
			{
				if (gameboard[y1][x1].minesaround > 0) {
					if (x1 == x && y1 == y)
					{
						switch (gameboard[y1][x1].minesaround)
						{
						case 1:
							cout << "\033[1;34;47m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						case 2:
							cout << "\033[1;32;47m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						case 3:
							cout << "\033[1;31;47m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						case 4:
							cout << "\033[1;95;47m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						case 5:
							cout << "\033[1;37;47m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						case 6:
							cout << "\033[1;36;47m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						case 7:
							cout << "\033[1;30;47m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						default:
							cout << "\033[1;90;47m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						}
					}
					else
					{
						switch (gameboard[y1][x1].minesaround)
						{
						case 1:
							cout << "\033[1;34;103m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						case 2:
							cout << "\033[1;32;103m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						case 3:
							cout << "\033[1;31;103m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						case 4:
							cout << "\033[1;95;103m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						case 5:
							cout << "\033[1;37;103m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						case 6:
							cout << "\033[1;36;103m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						case 7:
							cout << "\033[1;30;103m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						default:
							cout << "\033[1;90;103m[" << gameboard[y1][x1].minesaround << "]\033[0m";
							break;
						}
					}
					cellfinds++;
				}
				else if (x1 == x && y1 == y)
				{
					cout << "\033[1;30;47m[" << " " << "]\033[0m";
					cellfinds++;
				} 
				else
				{
					cout << "\033[1;30;103m[" << " " << "]\033[0m";
					cellfinds++;
				}
			}
			else if (gameboard[y1][x1].flaged) {
				if (x1 == x && y1 == y)
				{
					cout << "\033[1;31;47m[" << "F" << "]\033[0m";
				}
				else
				{
					cout << "\033[1;31;102m[" << "F" << "]\033[0m";
				}
			} 
			else if (x1 == x && y1 == y)
			{
				cout << "\033[1;30;47m[" << "?" << "]\033[0m";
			}
			else
			{
				cout << "\033[1;30;102m[" << "?" << "]\033[0m";
			}
		}
		cout << endl;
	}
}

void checkcell(int x, int y) {
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			//int combi = i * 10 + j;
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
		if (!gameboard[y][x].flaged && !gameboard[y][x].revealed)
		{
			gameboard[y][x].flaged = true;
		}
		else if (!gameboard[y][x].revealed)
		{

			gameboard[y][x].flaged = false;
		}
		else
		{
			cout << endl << "\033[1;31mErreur, case deja decouverte\033[0m" << endl;
		}
		break;
	case 2:
		if (gameboard[y][x].mine == true) {
			cout << "\033[1;31mGame Over\033[0m" << endl;
			for (int y1 = 0; y1 < boardsize; y1++)
			{
				for (int x1 = 0; x1 < boardsize; x1++)
				{
					if (y1 == y && x1 == x)
					{
						if (gameboard[y1][x1].mine)
						{
							cout << "\033[1;31;47m[" << "*" << "]\033[0m";
						}
					}
					else
					{
						if (gameboard[y1][x1].mine)
						{
							cout << "\033[1;31;103m[" << "*" << "]\033[0m";
						}
						else if (gameboard[y1][x1].revealed)
						{
							cout << "\033[1;30;103m[" << " " << "]\033[0m";
						}
						else
						{
							cout << "\033[1;30;42m[" << " " << "]\033[0m";
						}
					}
				}
				cout << endl;
			}
			return true;
			break;
		}
		else if (gameboard[y][x].revealed == true)
		{
			cout << endl << "\033[1;31mErreur, case deja decouverte\033[0m" << endl;
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
			cout << endl << "\033[1;31mErreur, saissie incorrect\033[0m" << endl << endl;
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
		afboard();
		if (!firstsele) {
			if (celltofind - cellfinds == 0) {
				cout << "\033[1;34mBravo!\nfin de partie\033[0m";
				end = true;
			}
			else
			{
				cout << "Votre derniere entree est en: \033[1;34;47m[x: " << x + 1 << ", y: " << y + 1 << "]\033[0m\n" << celltofind - cellfinds << " cases restantes a trouver" << endl;
			}
		}
		if (!end)
		{
			cout << endl << "\033[1;34mDemineur\033[0m" << endl;
			cout << "1. Decouvrir une case\n2. Poser/enlever un drapeau\n> ";
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
				cout << endl << "\033[1;31mErreur, saissie incorrect\033[0m" << endl;
				break;
			}
		}
	}
}