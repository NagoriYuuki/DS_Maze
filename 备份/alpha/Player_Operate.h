#pragma once

#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<algorithm>
#include "Create_Maze.h"
#include<conio.h>

using namespace std;

class Player
{
	MazeCreate& maze;
	int _x[4] = { 1, -1, 0, 0 };
	int _y[4] = { 0, 0, 1, -1 };
public:
	//int sx = maze.Start.x+1;
	//int sy = maze.Start.y+1;
	Player(MazeCreate& m) : maze(m)
	{
		sx = maze.Start.x;
		sy = maze.Start.y;
		ex = maze.End.x;
		ey = maze.End.y;
		player_pos = { sx, sy };
	}

	int sx;
	int sy;
	int ex;
	int ey;
	bool GameOver = false;
	POINT player_pos;
	void showpos()
	{
		cout << "调用： ";
		cout <<"Start: " << maze.Start.x << " " << maze.Start.y << endl;
		cout <<"End: " << maze.End.x << " " << maze.End.y << endl;
		//cout << "Player Position: " << player_pos.x << " " << player_pos.y << endl;
	}

	void Check()
	{
		cout << "Player: " << player_pos.x << " " << player_pos.y << endl;
	}

	void SetVisible(bool isVisible)
	{
		visible = isVisible;
	}

	void PlayerPosition()
	{
		//PlayerInit();
		if (!visible)
			return;

		if (!GameOver&&player_pos.x == maze.End.x && player_pos.y == maze.End.y)
		{
			cout << "Check Win" << endl;
			GameOver = true;
		}
		if (_kbhit())
		{
			cout << "按下了一个键" << endl;
			char input = _getch();
			if (input == 'w'||input=='W')
			{
				cout << input << endl;
				if (player_pos.x - 1 >= 1 && !maze.MazeMap[player_pos.x - 1][player_pos.y])
				{
					player_pos.x--;
				}
				cout << "player_pos: ";
				cout << player_pos.x << " " << player_pos.y << endl;
			}
			else if (input == 's'||input=='S')
			{
				cout << input << endl;
				if (player_pos.x + 1 <= maze.row+2 && !maze.MazeMap[player_pos.x + 1][player_pos.y])
				{
					player_pos.x++;
				}
				cout << "player_pos: ";
				cout << player_pos.x << " " << player_pos.y << endl;
			}
			else if (input == 'a'||input=='A')
			{
				cout << input << endl;
				if (player_pos.y - 1 >= 1 && !maze.MazeMap[player_pos.x][player_pos.y - 1])
				{
					player_pos.y--;
				}
				cout << "player_pos: ";
				cout << player_pos.x << " " << player_pos.y << endl;
			}
			else if (input == 'd'||input=='D')
			{
				cout << input << endl;
				if (player_pos.y + 1 <= maze.col+2 && !maze.MazeMap[player_pos.x][player_pos.y + 1])
				{
					player_pos.y++;
				}
				cout << "player_pos: ";
				cout << player_pos.x << " " << player_pos.y << endl;
			}
			else
				return;

		}
	}

	bool JudgeGame()
	{
		return GameOver;
	}

	void PlayerInit()
	{
		player_pos = maze.returnStart();
		GameOver = false;
		//cout << "Init ---------- " << endl;
	}

	void EZPlayerDraw()
	{
		setfillcolor(RGB(255, 0, 0));
		fillcircle(400 + player_pos.y * 30 + 13, 100 + player_pos.x * 30 + 13, 10);
	}

	void NMPlayerDraw()
	{
		setfillcolor(RGB(255, 0, 0));
		fillcircle(450 + player_pos.y * 15 + 7, 100 + player_pos.x * 15 + 7, 6);
	}

	void HDPlayerDraw()
	{
		setfillcolor(RGB(255, 0, 0));
		fillcircle(370 + player_pos.y * 11 + 5, 50 + player_pos.x * 11 + 5, 5);
	}

	void LNPlayerDraw()
	{
		setfillcolor(RGB(255, 0, 0));
		fillcircle(350 + player_pos.y * 10 + 5, 10 + player_pos.x * 10 + 5, 5);
	}

	void PrintMap()
	{
		cout << "Maze: ";
		cout << maze.row << " " << maze.col << endl;
		for (int i = 0; i <= 20 + 2; i++)
		{
			for (int j = 0; j <= 20 + 2; j++)
			{
				cout << maze.MazeMap[i][j] << " ";
			}
			cout << endl;
		}
	}

private:
	bool visible = false;

};


class EasyPlayer : public Player
{
public:
	void PlayerDraw()
	{
		setfillcolor(RGB(255, 0, 0));
		fillcircle(400 + player_pos.y * 30 + 10, 100 + player_pos.x * 30 + 10, 10);
	}
};


class NormalPlayer :public Player
{
public:
	void PlayerDraw()
	{
		setfillcolor(RGB(255, 0, 0));
		fillcircle(450 + player_pos.y * 15 + 10, 100 + player_pos.x * 15 + 10, 6);
	}
};

class HardPlayer :public Player
{
public:
	void PlayerDraw()
	{
		setfillcolor(RGB(255, 0, 0));
		fillcircle(370 + player_pos.y * 11 + 10, 50 + player_pos.x * 11 + 10, 5);
	}
};

class LunaticPlayer :public Player
{
public:
	void PlayerDraw()
	{
		setfillcolor(RGB(255, 0, 0));
		fillcircle(350 + player_pos.y * 10 + 10, 10 + player_pos.x * 10 + 10, 5);
	}
};

//else if (diff == 3)
//{
//	sz = 11;
//	luxx = 370;
//	luyy = 50;
//	row = col = 60;
//	Start = { 1,1 };
//	End = { 60,60 };
//}
//else if (diff == 4)
//{
//	sz = 10;
//	luxx = 350;
//	luyy = 10;
//	row = col = 80;
//	Start = { 1,1 };
//	End = { 80,80 };
//}
