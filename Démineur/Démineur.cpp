#include <iostream>
#include <ctime>

using namespace std;

struct celldata{
	bool revealed;
	bool mine;
	bool flaged;
};

celldata gameboard[100][100];

enum bschoices {
	PETIT = 10,
	MOYEN = 18,
	GRAND = 24
};

void iniboard() {

}

void afboard(int boardsize) {
	for (int y = 0; y < boardsize; y++)
	{
		for (int x = 0; x < boardsize; x++)
		{
			if (!gameboard[y][x].revealed && !gameboard[y][x].flaged) {
				cout << "[" << " " << "]";
			}
			else if (gameboard[y][x].flaged) {
				cout << "[" << "F" << "]";
			}
		}
		cout << endl;
	}
}

float gennbbt(int nb1, int nb2) {
	return floor(nb1 + ((rand() / (float)RAND_MAX) * nb2));
}

void addflag(int boardsize, int x, int y) {
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
	gameboard[y][x].flaged = true;
}

int main()
{
	srand(time(NULL));

	int boardsize = 0;
	int taille = 0;
	int select = 0;
	int x = 0;
	int y = 0;

	cout << "Entrer une taille de tableau\n1. PETIT\n2. MOYEN\n3. GRAND\n> ";
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
		cout << endl << "Demineur\n1. Afficher le tableau\n2. Poser un drapeau\n> ";
		cin >> select;

		switch (select)
		{
		case 1:
			afboard(boardsize);
			break;
		case 2:
			cout << "Entrer une coordonnee x et y\nEntrer x\n> ";
			cin >> x;
			cout << "Entrer y\n> ";
			cin >> y;
			addflag(boardsize, x - 1, y - 1);
			break;
		case 3:
			break;
		default:
			cout << "Erreur, saissie incorrect";
			break;
		}
	}
}
