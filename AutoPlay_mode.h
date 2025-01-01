#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<algorithm>
#include "Create_Maze.h"

#define xx first
#define yy second

class AutoPlay
{
public:
	MazeCreate maze;
	int asx = maze.Start.x;
	int asy = maze.Start.y;
	int aex = maze.End.x;
	int aey = maze.End.y;

	int mp[110][110];
	bool vis[110][110];
	int n = maze.row, m = maze.col;
	int _x[4] = { 1,-1,0,0 };
	int _y[4] = { 0,0,1,-1 };
	

private:
	vector<pair<int, int>> path;
	void init() // ≥ı ºªØ
	{
		for (int i = 0; i <= n + 2; i++)
			for (int z = 0; z <= m + 2; z++)
				mp[i][z] = maze.MazeMap[i][z];
		memset(vis, 0x3f, sizeof(vis));
		path.clear();
	}

	bool check(int x, int y)
	{
		return x >= 1 && x <= n && y >= 1 && y <= m;
	}



	void dfs(int x, int y)
	{
		if (x == aex && y == aey)
			return;
		for (int i = 0; i < 4; i++)
		{
			int xx = x + _x[i], yy = y + _y[i];
			if (check(xx, yy))
				continue;
			if (vis[xx][yy])
				continue;
			if (mp[xx][yy])
				continue;
			vis[xx][yy] = 1;
			path.push_back({ xx,yy });
			dfs(xx, yy);
		}
	}

	void bfs(int x, int y)
	{
		queue<pair<int,int>>que;
		que.push({ x,y });
		vis[x][y] = 1;
		while (!que.empty())
		{
			int ux = que.front().xx;
			int uy = que.front().yy;
			que.pop();
			for (int i = 0; i < 4; i++)
			{
				int vx = ux + _x[i];
				int vy = uy + _y[i];
				if (!check(vx, vy))
					continue;
				if (vis[vx][vy])
					continue;
				if (mp[vx][vy])
					continue;
				vis[vx][vy] = 1;
				que.push({ vx,vy });
			}
		}
	}
};


int main()
{

}

