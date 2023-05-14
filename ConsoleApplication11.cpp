#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <time.h>
using namespace std;

HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);

void FieldCreation(vector<vector<char>>& field, int& n)
{
	for (int i = 0; i < n; i++)
	{
		vector<char> walls;

		for (int j = 0; j < n; j++)
		{
			if (i == 0 && j == 0)
			{
				walls.push_back(char(201));
			}
			else if (i == 0 && j == n - 1)
			{
				walls.push_back(char(187));
			}
			else if (i == n - 1 && j == 0)
			{
				walls.push_back(char(200));
			}
			else if (i == n - 1 && j == n - 1)
			{
				walls.push_back(char(188));
			}
			else if (i != 0 && (j == 0 || j == n - 1))
			{
				walls.push_back(char(186));
			}
			else if ((i == 0 || i == n - 1) && j != 0)
			{
				walls.push_back(char(205));
			}
			else
			{
				walls.push_back(' ');
			}
		}

		field.push_back(walls);
	}

	return;
}

void FieldOutput(vector<vector<char>>& field, int& n, int& count, int& speed)
{
	COORD cord;
	cord.X = 0;
	cord.Y = 0;

	SetConsoleCursorPosition(hand, cord);

	SetConsoleTextAttribute(hand, 15);

	cout << "field side: " << n << endl;
	cout << "speed: " << speed << endl;
	cout << "score: " << count << endl;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (field[i][j] == char(201) || field[i][j] == char(187) || field[i][j] == char(200) || field[i][j] == char(188) || field[i][j] == char(186) || field[i][j] == char(205))
			{
				SetConsoleTextAttribute(hand, 11);
			}
			else if (field[i][j] == char(248))
			{
				SetConsoleTextAttribute(hand, 12);
			}
			else if (field[i][j] == char(2) || field[i][j] == 'o')
			{
				SetConsoleTextAttribute(hand, 14);
			}

			if (field[i][j] == char(205) || field[i][j] == char(201) || field[i][j] == char(200))
			{
				cout << field[i][j] << char(205);
			}
			else
			{
				cout << field[i][j] << " ";
			}
		}

		cout << endl;
	}

	return;
}

void Object(vector<vector<char>>& field, int& n, vector<int>& Xcords, vector<int>& Ycords, int& objectX, int& objectY, int& count)
{
	bool place = false;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (field[i][j] == ' ')
			{
				place = true;
			}
		}
	}

	if (place)
	{
		while (field[objectY][objectX] == char(2) || field[objectY][objectX] == 'o')
		{
			objectX = 1 + rand() % (n - 2);
			objectY = 1 + rand() % (n - 2);
		}

		field[objectY][objectX] = char(248);
	}

	return;
}

bool ObjectCollision(vector<int>& Xcords, vector<int>& Ycords, int& objectX, int& objectY)
{
	return Xcords[0] == objectX && Ycords[0] == objectY;
}

bool Collision(vector<vector<char>>& field, int& n, vector<int>& Xcords, vector<int>& Ycords, bool& alive, int& count)
{
	bool snakeCollision = false;

	for (int i = Xcords.size() - count - 1; i < Xcords.size() - 1; i++)
	{
		if (field[Ycords[i]][Xcords[i]] == char(2))
		{
			snakeCollision = true;
		}
	}

	alive = !(Xcords[0] == 0 || Ycords[0] == 0 || Xcords[0] == n - 1 || Ycords[0] == n - 1 || snakeCollision || count == pow((n - 2), 2) - 1);
	return alive;
}

void Length(vector<vector<char>>& field, int n, vector<int>& Xcords, vector<int>& Ycords, int& count)
{
	Xcords.push_back(Xcords[0]);
	Ycords.push_back(Ycords[0]);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (field[i][j] == char(2) || field[i][j] == 'o')
			{
				field[i][j] = ' ';
			}
		}
	}

	for (int i = Xcords.size() - count - 1; i < Xcords.size() - 1; i++)
	{
		field[Ycords[i]][Xcords[i]] = 'o';
	}

	field[Ycords[0]][Xcords[0]] = char(2);
}

void GameOver(vector<vector<char>>& field, int& n, int& count, int& speed)
{
	system("cls");

	if (count == pow((n - 2), 2) - 1)
	{
		FieldOutput(field, n, count, speed);
		Sleep(1000);
		system("cls");

		SetConsoleTextAttribute(hand, 10);
		cout << "victory" << endl;
		cout << "score: " << count << endl;
	}
	else
	{
		SetConsoleTextAttribute(hand, 12);
		cout << "game over" << endl;
		cout << "score: " << count << endl;
	}

	return;
}

void AfterKey(vector<vector<char>>& field, int& n, vector<int>& Xcords, vector<int>& Ycords, int& objectX, int& objectY, int& count, int& speed, int& delay, bool& alive)
{
	if (ObjectCollision(Xcords, Ycords, objectX, objectY))
	{
		count++;
		Length(field, n, Xcords, Ycords, count);
		Object(field, n, Xcords, Ycords, objectX, objectY, count);
	}
	else
	{
		Length(field, n, Xcords, Ycords, count);
	}

	if (Collision(field, n, Xcords, Ycords, alive, count))
	{
		FieldOutput(field, n, count, speed);
		Sleep(delay);
	}
}

int main()
{
	srand(time(NULL));

	vector<vector<char>> field;
	vector<int> Xcords, Ycords;
	int n, count = 0, speed, delay;
	bool alive = true;
	char key = '0', move;

	SetConsoleTextAttribute(hand, 15);

	cout << "field side: ";
	cin >> n;

	if (n < 4)
	{
		system("cls");
		n = 4;
		cout << "field side: " << n << endl;
	}

	cout << "speed: ";
	cin >> speed;

	if (speed < 1)
	{
		system("cls");
		speed = 1;
		cout << "speed: " << speed << endl;
	}
	else if (speed > 10)
	{
		system("cls");
		speed = 10;
		cout << "speed: " << speed << endl;
	}

	delay = 100 * (11 - speed);

	cout << "score: " << count << endl;

	int objectX = 1 + rand() % (n - 2), objectY = 1 + rand() % (n - 2);

	Xcords.push_back(1 + rand() % (n - 2));
	Ycords.push_back(1 + rand() % (n - 2));

	FieldCreation(field, n);

	field[Ycords[0]][Xcords[0]] = char(2);
	Length(field, n, Xcords, Ycords, count);
	Object(field, n, Xcords, Ycords, objectX, objectY, count);

	FieldOutput(field, n, count, speed);

	while (key != 'w' && key != 'a' && key != 's' && key != 'd' && key != 'W' && key != 'A' && key != 'S' && key != 'D')
	{
		if (_kbhit())
		{
			key = _getch();
			move = key;
		}
	}

	while (alive)
	{
		if (_kbhit())
		{
			key = _getch();
		}

		if (key == 'w' || key == 'W')
		{
			if (move == 's' || move == 'S')
			{
				key = move;
			}
			else
			{
				Ycords[0]--;
				AfterKey(field, n, Xcords, Ycords, objectX, objectY, count, speed, delay, alive);
				move = key;
			}
		}
		else if (key == 'a' || key == 'A')
		{
			if (move == 'd' || move == 'D')
			{
				key = move;
			}
			else
			{
				Xcords[0]--;
				AfterKey(field, n, Xcords, Ycords, objectX, objectY, count, speed, delay, alive);
				move = key;
			}
		}
		else if (key == 's' || key == 'S')
		{
			if (move == 'w' || move == 'W')
			{
				key = move;
			}
			else
			{
				Ycords[0]++;
				AfterKey(field, n, Xcords, Ycords, objectX, objectY, count, speed, delay, alive);
				move = key;
			}
		}
		else if (key == 'd' || key == 'D')
		{
			if (move == 'a' || move == 'A')
			{
				key = move;
			}
			else
			{
				Xcords[0]++;
				AfterKey(field, n, Xcords, Ycords, objectX, objectY, count, speed, delay, alive);
				move = key;
			}
		}
		else
		{
			key = move;
		}
	}

	GameOver(field, n, count, speed);
}