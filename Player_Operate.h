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
	MazeCreate& maze;//引用传参
	int _x[4] = { 1, -1, 0, 0 };
	int _y[4] = { 0, 0, 1, -1 };
public:
	//int sx = maze.Start.x+1;
	//int sy = maze.Start.y+1;
	Player(MazeCreate& m) : maze(m)//构造函数
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
	void showpos()//显示当前位置，调试用
	{
		cout << "调用： ";
		cout <<"Start: " << maze.Start.x << " " << maze.Start.y << endl;
		cout <<"End: " << maze.End.x << " " << maze.End.y << endl;
		//cout << "Player Position: " << player_pos.x << " " << player_pos.y << endl;
	}

	void Check()//显示玩家当前位置，调试用
	{
		cout << "Player: " << player_pos.x << " " << player_pos.y << endl;
	}

	void SetVisible(bool isVisible)
	{
		visible = isVisible;
	}


	void PlayerPosition(const ExMessage& msg)//玩家移动
	{
		//Sleep(100);
		if (!GameOver && player_pos.x == maze.End.x && player_pos.y == maze.End.y)
		{
			cout << "Check Win" << endl;
			GameOver = true;
		}
		if (msg.message == WM_KEYDOWN)//按键按下
		{
			cout << "CheckDOWN" << endl;
			auto key = msg.vkcode;//虚拟键码映射
			if (key == 'W' || key == VK_UP)
				isMove[1] = true;
			else if (key == 'S' || key == VK_DOWN)
				isMove[2] = true;
			else if (key == 'A' || key == VK_LEFT)
				isMove[3] = true;
			else if (key == 'D' || key == VK_RIGHT)
				isMove[4] = true;
			else
				return;
		}
		else if (msg.message == WM_KEYUP)//按键抬起
		{
			cout << "CheckUP" << endl;
			auto key = msg.vkcode;
			if (key == 'W' || key == VK_UP)
				isMove[1] = false;
			else if (key == 'S' || key == VK_DOWN)
				isMove[2] = false;
			else if (key == 'A' || key == VK_LEFT)
				isMove[3] = false;
			else if (key == 'D' || key == VK_RIGHT)
				isMove[4] = false;
			else
				return;
		}
		if (isMove[1])
		{
			cout << "W" << endl;
			if (player_pos.x - 1 >= 1 && !maze.MazeMap[player_pos.x - 1][player_pos.y])
				player_pos.x--;
			cout << "player_pos: ";
			cout << player_pos.x << " " << player_pos.y << endl;
		}
		else if (isMove[2])
		{
			cout << "S" << endl;
			if (player_pos.x + 1 <= maze.row + 2 && !maze.MazeMap[player_pos.x + 1][player_pos.y])
				player_pos.x++;
			cout << "player_pos: ";
			cout << player_pos.x << " " << player_pos.y << endl;
		}
		else if (isMove[3])
		{
			cout << "A" << endl;
			if (player_pos.y - 1 >= 1 && !maze.MazeMap[player_pos.x][player_pos.y - 1])
				player_pos.y--;
			cout << "player_pos: ";
			cout << player_pos.x << " " << player_pos.y << endl;
		}
		else if (isMove[4])
		{
			cout << "D" << endl;
			if (player_pos.y + 1 <= maze.col + 2 && !maze.MazeMap[player_pos.x][player_pos.y + 1])
				player_pos.y++;
			cout << "player_pos: ";
			cout << player_pos.x << " " << player_pos.y << endl;
		}

	}
	/*----------------------以控制台为窗口，基于kbhit()函数的移动，废案---------------------------*/

	//void PlayerPosition()
	//{
	//	//PlayerInit();
	//	if (!visible)
	//		return;

		//if (!GameOver&&player_pos.x == maze.End.x && player_pos.y == maze.End.y)
		//{
		//	cout << "Check Win" << endl;
		//	GameOver = true;
		//}
	//	if (_kbhit())
	//	{
	//		cout << "按下了一个键" << endl;
	//		char input = _getch();
	//		if (input == 'w'||input=='W')
	//		{
	//			cout << input << endl;
				//if (player_pos.x - 1 >= 1 && !maze.MazeMap[player_pos.x - 1][player_pos.y])
				//{
				//	player_pos.x--;
				//}
				//cout << "player_pos: ";
				//cout << player_pos.x << " " << player_pos.y << endl;
	//		}
	//		else if (input == 's'||input=='S')
	//		{
	//			cout << input << endl;
	//			if (player_pos.x + 1 <= maze.row+2 && !maze.MazeMap[player_pos.x + 1][player_pos.y])
	//			{
	//				player_pos.x++;
	//			}
	//			cout << "player_pos: ";
	//			cout << player_pos.x << " " << player_pos.y << endl;
	//		}
	//		else if (input == 'a'||input=='A')
	//		{
	//			cout << input << endl;
	//			if (player_pos.y - 1 >= 1 && !maze.MazeMap[player_pos.x][player_pos.y - 1])
	//			{
	//				player_pos.y--;
	//			}
	//			cout << "player_pos: ";
	//			cout << player_pos.x << " " << player_pos.y << endl;
	//		}
	//		else if (input == 'd'||input=='D')
	//		{
	//			cout << input << endl;
	//			if (player_pos.y + 1 <= maze.col+2 && !maze.MazeMap[player_pos.x][player_pos.y + 1])
	//			{
	//				player_pos.y++;
	//			}
	//			cout << "player_pos: ";
	//			cout << player_pos.x << " " << player_pos.y << endl;
	//		}
	//		else
	//			return;

	//	}
	//}

	bool JudgeGame()
	{
		return GameOver;//返回游戏是否结束
	}

	void PlayerInit()//初始化玩家位置
	{
		player_pos = maze.returnStart();
		GameOver = false;
		//cout << "Init ---------- " << endl;
	}
	//对于不同难度，因为缩放所以位置不同，所以分别绘制
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

	void PrintMap()//打印迷宫，调试用
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
	bool visible = false;//是否可见
	bool isMove[5] = { false };//是否被触发，防止重复触发

};

/*---------------------不同玩家类，继承自Player类，废案----------------------*/

//class EasyPlayer : public Player
//{
//public:
//	void PlayerDraw()
//	{
//		setfillcolor(RGB(255, 0, 0));
//		fillcircle(400 + player_pos.y * 30 + 10, 100 + player_pos.x * 30 + 10, 10);
//	}
//};
//
//
//class NormalPlayer :public Player
//{
//public:
//	void PlayerDraw()
//	{
//		setfillcolor(RGB(255, 0, 0));
//		fillcircle(450 + player_pos.y * 15 + 10, 100 + player_pos.x * 15 + 10, 6);
//	}
//};
//
//class HardPlayer :public Player
//{
//public:
//	void PlayerDraw()
//	{
//		setfillcolor(RGB(255, 0, 0));
//		fillcircle(370 + player_pos.y * 11 + 10, 50 + player_pos.x * 11 + 10, 5);
//	}
//};
//
//class LunaticPlayer :public Player
//{
//public:
//	void PlayerDraw()
//	{
//		setfillcolor(RGB(255, 0, 0));
//		fillcircle(350 + player_pos.y * 10 + 10, 10 + player_pos.x * 10 + 10, 5);
//	}
//};

