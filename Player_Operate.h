#pragma once

#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<algorithm>
#include "Create_Maze.h"

using namespace std;

class Player
{
	MazeCreate maze;
public:

	int sx = maze.Start.x;
	int sy = maze.Start.y;
	int ex = maze.End.x;
	int ey = maze.End.y;
	POINT player_pos = { sx,sy };




};


int main()
{

}

