#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<algorithm>
#include "Create_Maze.h"
#include<windows.h>

using namespace std;

#define xx first
#define yy second

class AutoPlay
{
public:
	MazeCreate& maze;//引用传参
    int asx;
    int asy;
    int aex;
    int aey;

    int mp[110][110];
    bool vis[110][110];
    int n;
    int m;
    int _x[4] = { 1, -1, 0, 0 };
    int _y[4] = { 0, 0, 1, -1 };

    AutoPlay(MazeCreate& mz) : maze(mz)
    {
		asx = maze.Start.x;
		asy = maze.Start.y;
		aex = maze.End.x;
		aey = maze.End.y;
		n = maze.row;
        m = maze.col;
        init();
    }

    void Check()//调试用
    {
        cout << "scan ";
		cout << "Start :" << maze.Start.x << " " << maze.Start.y << endl;
		cout << "End: " << maze.End.x << " " << maze.End.y << endl;
		//cout << "end:" << aex << " " << aey << endl;
		//cout << "n_m: " << n << " " << m << endl;
        //for (int i = 0; i <= maze.row + 2; i++)
        //{
        //    for (int z = 0; z <= maze.col + 2; z++)
        //        cout << maze.MazeMap[i][z] << " ";
        //    cout << endl;
        //}
        cout << "vismap:--------------------------------------" << endl;
        for (int i = 0; i <= maze.row + 2; i++)
        {
            for (int z = 0; z <= maze.col + 2; z++)
                cout << vis[i][z] << " ";
            cout << endl;
        }
    }

	void checkvvis()//调试用
    {
        cout << vvis.size();
    }

	void scan(int x)//选择不同的搜索方式
    {
        cout << "scanmode: " << x << endl;
        if (x == 1)
            dfs(maze.Start.x, maze.Start.y);
        else if (x == 2)
            bfs(maze.Start.x, maze.Start.y);
        else if (x == 3)
            Astar(maze.Start.x, maze.Start.y);
    }

	void PrintAllVis()//输出搜索过程
    {
        for (auto i : vvis)
        {
            //Sleep(20);
            int xx = i.xx;
            int yy = i.yy;
			ShowVis(xx, yy);
        }
    }

    void PrintVis(int x)
    {
		for (int i = 0; i < x; i++)
		{
			int xx = vvis[i].xx;
			int yy = vvis[i].yy;
			ShowVis(xx, yy);
		}
    }


	void PrintAllPath()//输出可行路径
    {
		for (auto i : path)
		{
			//Sleep(20);
			int xx = i.xx;
			int yy = i.yy;
			ShowPath(xx, yy);
		}
    }

	void PrintPath(int x)
	{
        for(int i=0;i<x;i++)
        {
			int xx = path[i].xx;
			int yy = path[i].yy;
            ShowPath(xx, yy);
        }
	}

	int returnPathSize()//返回路径长度
	{
		return path.size();
	}

	int returnVisSize()//返回搜索过程长度
	{
		return vvis.size();
	}

	void init()//初始化
    {
        cout << "INITSCAN" << endl;
        for (int i = 0; i <= n + 2; i++)
            for (int z = 0; z <= m + 2; z++)
                mp[i][z] = maze.MazeMap[i][z];
        memset(vis, 0, sizeof(vis));
        path.clear();
		vvis.clear();
        dfscheck = false;
    }

private:
	vector<pair<int, int>> path;//存储路径
	vector<pair<int, int>>vvis;//存储搜索过程
    int luxx, luyy, sz;


	bool check(int x, int y)//判断是否越界
    {
        return x >= 1 && x <= maze.row+2 && y >= 1 && y <= maze.col+2;
    }

	bool dfscheck = false;//用于DFS判断是否找到路径
	void dfs(int x, int y)//常规DFS
    {
		cout << "dfs now: " << x << " " << y << endl;
        if (x == maze.End.x && y == maze.End.y)
        {
            dfscheck = true;
            return;
        }
        for (int i = 0; i < 4; i++)
        {
            int xx = x + _x[i], yy = y + _y[i];
            if (!check(xx, yy))
                continue;
            if (vis[xx][yy])
                continue;
            if (maze.MazeMap[xx][yy])
                continue;
            vis[xx][yy] = 1;
            vvis.push_back({ xx, yy });
			//ShowVis(xx, yy);
            dfs(xx, yy);
            if (dfscheck)
            {
				path.push_back({ x, y });
				return;
            }
        }
    }

	void bfs(int x, int y)//常规BFS
    {
		vector<vector<pair<int, int>>> last(200, vector<pair<int, int>>(200, { -1, -1 }));//存储前驱节点，用于输出路径
        queue<pair<int, int>> que;
        que.push({ x, y });
        vis[x][y] = 1;
        last[x][y] = { x,y };
        while (!que.empty())
        {
            int ux = que.front().xx;
            int uy = que.front().yy;

            if (ux != maze.Start.x || uy != maze.Start.y)
            {
                //ShowVis(ux, uy);
                cout << "bfsnow:" << ux << " " << uy << endl;
                vvis.push_back({ ux, uy });
            }
            if (ux == maze.End.x && uy == maze.End.y)
                break;
            que.pop();
            for (int i = 0; i < 4; i++)
            {
                int vx = ux + _x[i];
                int vy = uy + _y[i];
                if (!check(vx, vy))
                    continue;
                if (vis[vx][vy])
                    continue;
                if (maze.MazeMap[vx][vy])
                    continue;
                vis[vx][vy] = 1;
				last[vx][vy] = { ux, uy };
                que.push({ vx, vy });
            }
        }
		pair<int, int>now = { maze.End.x,maze.End.y };
        while (now != last[now.xx][now.yy])
        {
			path.push_back(now);
			now = last[now.xx][now.yy];
        }
		path.push_back({ maze.Start.x,maze.Start.y });
    }

	int h(int x, int y)//A*算法的启发函数，计算当前位置到终点的曼哈顿距离
    {
        return abs(x - maze.End.x) + abs(y - maze.End.y);
    }

	int cal(int pay, int x, int y)//A*算法的启发函数，计算当前位置到终点的曼哈顿距离+当前位置到起点的距离，F(x)=G(x)+H(x)
    {
        return h(x, y) + pay;
    }

    struct node
    {
        pair<int, int>now;
        int cost;
        int spd;
		bool operator < (const node& a) const//优先选择F(x)小的节点进行扩展
        {
            return cost > a.cost;
        }
    };

#define xx first
#define yy second

    void Astar(int x, int y)//
    {
		vector<vector<pair<int, int>>> last(200, vector<pair<int, int>>(200, { -1, -1 }));//存储前驱节点，用于输出路径
        priority_queue<node>que;
        que.push({ {x, y}, cal(0, x, y), 0 });
        vis[x][y] = 1;
		last[x][y] = { x,y };
        while (!que.empty())
        {
            int ux = que.top().now.xx;
            int uy = que.top().now.yy;
            int spd = que.top().spd;
            if (ux != maze.Start.x || uy != maze.Start.y)
            {
                //ShowVis(ux, uy);
                cout << "astarnow:" << ux << " " << uy << endl;
                vvis.push_back({ ux, uy });
            }
            if (ux == maze.End.x && uy == maze.End.y)
                break;
            que.pop();
            for (int i = 0; i < 4; i++)
            {
                int vx = ux + _x[i];
                int vy = uy + _y[i];
                if (!check(vx, vy))
                    continue;
                if (vis[vx][vy])
                    continue;
                if (maze.MazeMap[vx][vy])
                    continue;
                vis[vx][vy] = 1;
				last[vx][vy] = { ux, uy };
                que.push({ {vx, vy}, cal(spd + 1, vx, vy), spd + 1 });
            }
        }
		pair<int, int>now = { maze.End.x,maze.End.y };
		while (now != last[now.xx][now.yy])//根据前驱节点输出路径
		{
			path.push_back(now);
			now = last[now.xx][now.yy];
		}
        path.push_back({ maze.Start.x,maze.Start.y });
    }


	void ShowVis(int x, int y)//显示搜索过程
    {
        setfillcolor(YELLOW);
        if (diff == 1)
        {
            luxx = 400;
            luyy = 100;
            sz = 30;
        }
        else if (diff == 2)
        {
            sz = 15;
            luxx = 450;
            luyy = 100;
        }
        else if (diff == 3)
        {
            sz = 11;
            luxx = 370;
            luyy = 50;
        }
        else if (diff == 4)
        {
            sz = 10;
            luxx = 350;
            luyy = 10;
        }
        int nowposx = luxx + sz * y;
        int nowposy = luyy + sz * x;
        int lux = nowposx, luy = nowposy, ldx = nowposx + sz, ldy = nowposy, rux = nowposx, ruy = nowposy + sz, rdx = nowposx + sz, rdy = nowposy + sz;
        POINT pt[4] = { {lux, luy}, {ldx, ldy}, {rdx, rdy}, {rux, ruy} };
		fillpolygon(pt, 4);
    }

	void ShowPath(int x, int y)//显示路径
    {
        setfillcolor(RGB(255, 65, 129));
        if (diff == 1)
        {
            luxx = 400;
            luyy = 100;
            sz = 30;
        }
        else if (diff == 2)
        {
            sz = 15;
            luxx = 450;
            luyy = 100;
        }
        else if (diff == 3)
        {
            sz = 11;
            luxx = 370;
            luyy = 50;
        }
        else if (diff == 4)
        {
            sz = 10;
            luxx = 350;
            luyy = 10;
        }
        int nowposx = luxx + sz * y;
        int nowposy = luyy + sz * x;
        int lux = nowposx, luy = nowposy, ldx = nowposx + sz, ldy = nowposy, rux = nowposx, ruy = nowposy + sz, rdx = nowposx + sz, rdy = nowposy + sz;
        POINT pt[4] = { {lux, luy}, {ldx, ldy}, {rdx, rdy}, {rux, ruy} };
        fillpolygon(pt, 4);
    }

};
