#pragma once
//#ifndef CREATE_MAZE_H__
//#define CREATE_MAZE_H__

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <graphics.h>
using namespace std;

#define create_down 1
#define create_right 2
#define create_left 3
#define create_up 4

int diff = 1;

class MazeCreate
{
public:

    struct Point
    {
        int x, y;
    };

    Point Start, End; // 起点和终点
    int row, col; // 迷宫的行数和列数

    struct InBlock
    {
        int row, col, direct;
    };

    vector<InBlock> Block; // 记录迷宫中的墙

    int now_x = 1, now_y = 1; // 当前位置

    int MazeMap[110][110]; // 迷宫地图

    int luxx = 100, luyy = 100, sz = 10;


    void CreateMaze() // 随机化Prim算法生成迷宫
    {

        if (diff == 1)
        {
            luxx = 400;
            luyy = 100;
            sz = 30;
            row = col = 20;
            Start = { 1,1 };
			End = { 20,20 };
        }
        else if (diff == 2)
        {
            sz = 15;
            luxx = 450;
            luyy = 100;
            row = col = 40;
            Start = { 1,1 };
            End = { 40,40 };
        }
        else if (diff == 3)
        {
            sz = 11;
            luxx = 370;
            luyy = 50;
            row = col = 60;
			Start = { 1,1 };
			End = { 60,60 };
        }
        else if (diff == 4)
        {
            sz = 10;
            luxx = 350;
            luyy = 10;
            row = col = 80;
			Start = { 1,1 };
			End = { 80,80 };
        }

		cout << "diff: " << diff << endl;

        init();
        CountWall();
        //diffchange();
        MazeMap[now_x][now_y] = 1;
        auto seed = chrono::system_clock::now().time_since_epoch().count();
        srand(seed);
        while (Block.size())
        {
            int sz = Block.size();
            int index = rand() % sz;
            int now_direct = Block[index].direct;
            now_x = Block[index].row;
            now_y = Block[index].col;
            switch (now_direct)
            {
            case create_down:
            {
                now_x = Block[index].row + 1;
                now_y = Block[index].col;
                break;
            }
            case create_up:
            {
                now_x = Block[index].row - 1;
                now_y = Block[index].col;
                break;
            }
            case create_right:
            {
                now_x = Block[index].row;
                now_y = Block[index].col + 1;
                break;
            }
            case create_left:
            {
                now_x = Block[index].row;
                now_y = Block[index].col - 1;
                break;
            }

            }

            if (MazeMap[now_x][now_y] == 1)
            {
                MazeMap[Block[index].row][Block[index].col] = 0;
                MazeMap[now_x][now_y] = 0;
                CountWall();
            }
            Block.erase(Block.begin() + index);

        }
    }

    void PrintMaze()
    {
		cout << "col: " << col << " row: " << row << endl;
        cout << "------------------------------------------" << endl;
        for (int i = 0; i <= row + 2; i++)
        {
            for (int z = 0; z <= col + 2; z++)
            {
                if (i == Start.x && z == Start.y)
                    cout << "S ";
                else if (i == End.x && z == End.y)
                    cout << "E ";
                else
                    cout << (MazeMap[i][z] == 1 ? "# " : " @");
            }
            cout << endl;
        }
    }



    void EasyxPrintMaze()
    {
        //diffchange();
        //cout << "迷宫生成完毕，正在绘制迷宫……" << endl;
        //settextcolor(BLACK);

 /*       if (diff == 1)
        {
            luxx = 400;
            luyy = 100;
            sz = 30;
			row = col = 20;
        }
        else if (diff == 2)
        {
            sz = 15;
            luxx = 350;
            luyy = 100;
            row = col = 40;
        }
        else if (diff == 3)
        {
            sz = 11;
            luxx = 270;
            luyy = 50;
            row = col = 60;
        }
        else if (diff == 4)
        {
            sz = 10;
            luxx = 250;
            luyy = 10;
            row = col = 80;
        }*/

        //cout << sz << endl;
		cout << "----------------------------------EasyxPrintMaze----------------------------------" << endl;
        cout << "col: " << col << " row: " << row << endl;
        cout << "------------------------------------------" << endl;

        for (int i = 0; i <= row + 2; i++)
        {
            for (int z = 0; z <= col + 2; z++)
            {
                int nowposx = luxx + sz * z;
                int nowposy = luyy + sz * i;
                int lux = nowposx, luy = nowposy, ldx = nowposx + sz, ldy = nowposy, rux = nowposx, ruy = nowposy + sz, rdx = nowposx + sz, rdy = nowposy + sz;
                if (i == Start.x && z == Start.y)
                    setStart(lux, luy, ldx, ldy, rux, ruy, rdx, rdy);
                else if (i == End.x && z == End.y)
                    setEnd(lux, luy, ldx, ldy, rux, ruy, rdx, rdy);
                else
                {
                    if (MazeMap[i][z] == 1)
                        setWall(lux, luy, ldx, ldy, rux, ruy, rdx, rdy);
                }
            }
        }
    }


    //1:墙
    //0:空地


private:
    void init() // 初始化迷宫，默认为全墙
    {
        for (int i = 0; i <= col + 2; i++)
            for (int z = 0; z <= row + 2; z++)
                MazeMap[z][i] = 1;
        Block.clear();
        now_x = 1;
        now_y = 1;

    }

    void push(int x, int y, int dir) // 存放墙
    {
        Block.push_back({ x, y, dir });
    }

    int CountWall() // 计算当前位置周围的墙的数量
    {
        int count = 0;
        if (now_x + 1 <= row)
        {
            push(now_x + 1, now_y, create_down);
            count++;
        }
        if (now_x - 1 >= 1)
        {
            push(now_x - 1, now_y, create_up);
            count++;
        }
        if (now_y + 1 <= col)
        {
            push(now_x, now_y + 1, create_right);
            count++;
        }
        if (now_y - 1 >= 1)
        {
            push(now_x, now_y - 1, create_left);
            count++;
        }
        return count;
    }


    void setWall(int lux, int luy, int ldx, int ldy, int rux, int ruy, int rdx, int rdy)
    {
        setfillcolor(BLACK);
        POINT p[] = { {lux,luy},{ldx,ldy},{rdx,rdy},{rux,ruy},{lux,luy} };
        solidpolygon(p, 4);
    }
    void setStart(int lux, int luy, int ldx, int ldy, int rux, int ruy, int rdx, int rdy)
    {
        setfillcolor(RGB(69, 200, 251));
        POINT p[] = { {lux,luy},{ldx,ldy},{rdx,rdy},{rux,ruy},{lux,luy} };
        solidpolygon(p, 4);
    }
    void setEnd(int lux, int luy, int ldx, int ldy, int rux, int ruy, int rdx, int rdy)
    {
        setfillcolor(RGB(252, 61, 73));
        POINT p[] = { {lux,luy},{ldx,ldy},{rdx,rdy},{rux,ruy},{lux,luy} };
        solidpolygon(p, 4);
    }

    /*  void diffchange()
      {
          if (diff == 1)
          {
              luxx = 100;
              luyy = 100;
              sz = 30;
              col = row == 20;
              Start = { 1,1 };
              End = { 20,20 };
          }
          else if (diff == 2)
          {
              luxx = 75;
              luyy = 75;
              sz = 25;
              col = row = 40;
              Start = { 1,1 };
              End = { 40,40 };
          }
          else if (diff == 3)
          {
              luxx = 50;
              luyy = 50;
              sz = 15;
              col = row = 60;
              Start = { 1,1 };
              End = { 60,60 };
          }
          else if (diff == 3)
          {
              luxx = 30;
              luyy = 30;
              sz = 10;
              col = row = 80;
              Start = { 1,1 };
              End = { 80,80 };
          }
      }*/

};

//#endif
