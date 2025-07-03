#pragma once
#include<windows.h>
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <graphics.h>
#include <conio.h>
#include <tchar.h>
#include "Create_Maze.h"
#include "Player_Operate.h"
#include "AutoPlay_mode.h"

using namespace std;

#define ULL unsigned long long


const int WINDOW_WIDTH = 1440;
const int WINDOW_HEIGHT = 1440;
const int BUTTON_WIDTH = 270;
const int BUTTON_HEIGHT = 90;

// 游戏状态-----------------------------------
int GameState = 1;
//1:主菜单
//2:难度选择
//3:模式选择 
//4：玩家游戏中
//5：奥托普雷方式（？
//6：奥托普雷游戏中
//7：结算页面
bool running = true;
bool isCreateMaze = false;
bool ResetAPMap = false;
bool isscan = false;
int APmode = 0;
int countvis = 0;
int countpath = 0;
bool printpath = false;
bool isprintpath = false;
bool isprintvis = false;

// 游戏状态-----------------------------------


//调试用变量----------------------------------
bool judge = 0;
bool judge2 = 0;
bool judge3 = 0;
//调试用变量----------------------------------


class Button // 按钮基类
{

public:
	Button(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
    {
		region = rect;// 按钮的位置
		loadimage(&img_idle, path_img_idle);// 按钮的自然状态的图片
		loadimage(&img_hovered, path_img_hovered);// 按钮鼠标悬停状态的图片
		loadimage(&img_pushed, path_img_pushed);// 按钮的触发状态的图片
        visible = false; // 按钮默认不可见
    }
    ~Button() = default;

    void Draw() // 绘制按钮
    {
        if (!visible)
            return; // 按钮不可见时不绘制
		switch (status)// 根据按钮状态绘制不同的图片
        {
        case Status::Idle:
            putimage(region.left, region.top, &img_idle);
            break;
        case Status::Hovered:
            putimage(region.left, region.top, &img_hovered);
            break;
        case Status::Pushed:
            putimage(region.left, region.top, &img_pushed);
            break;
        }
    }

	void ProcessEvent(const ExMessage& msg) // 处理事件
    {
        if (!visible) 
            return; // 按钮不可见时不处理事件
        //cout << "------------" << endl;
        switch (msg.message)
        {
        case WM_MOUSEMOVE: // 鼠标移动事件
            if (status == Status::Idle && CheckMousePos(msg.x, msg.y)) // 鼠标移动到按钮上
                status = Status::Hovered;
            else if (status == Status::Hovered && !CheckMousePos(msg.x, msg.y)) // 鼠标移出按钮
                status = Status::Idle;
            break;
        case WM_LBUTTONDOWN: // 鼠标左键按下事件
            if (status == Status::Hovered) // 鼠标在按钮上按下
                status = Status::Pushed;
            break;
        case WM_LBUTTONUP: // 鼠标左键抬起事件
            if (status == Status::Pushed) // 鼠标在按钮上抬起
            {
                status = Status::Hovered;
                OnClick();
            }
            break;
        default: // 其他非法事件
            break;
        }

    }

	void ResetStatus() // 重置按钮状态
    {
        status = Status::Idle;
    }


	void SetVisible(bool isVisible) // 设置按钮的可见性
    {
        visible = isVisible;
    }

protected:
    virtual void OnClick() = 0; // 纯虚函数，点击按钮时的操作

private:

    bool CheckMousePos(int x, int y) // 检查鼠标是否在按钮上
    {
        return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
    }

    enum class Status // 按钮的状态枚举变量
    {
        Idle = 0,
        Hovered,
        Pushed
    };

private:// 按钮的属性
	RECT region;// 按钮的位置
	IMAGE img_idle;// 按钮的自然状态的图片
	IMAGE img_hovered;// 按钮鼠标悬停状态的图片
	IMAGE img_pushed;// 按钮的触发状态的图片
    Status status = Status::Idle;
    bool visible; // 按钮可见性标志
};

// 继承Button基类，实现各自的OnClick函数
class StartButton : public Button
{
public:
    StartButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
		: Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}// 调用基类构造函数，初始化基类属性，下同
	~StartButton() = default;// 调用基类析构函数，析构函数为空，使用默认析构函数，可以不用手动释放资源，由基类负责释放资源，避免内存泄漏，下同
protected:
	void OnClick()
    {
        GameState = 2;
        SetVisible(false); // 游戏开始时隐藏按钮
        Button::ResetStatus();
    }
};

class HelpButton : public Button
{
public:
    HelpButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
        : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~HelpButton() = default;
protected:
    void OnClick()
    {
        MessageBox(GetHWnd(), _T("这是个还没有写的Help，然后就没有然后了\n二更：现在写完了，belike：↓\n这是一个经典玩法的迷宫小游戏，共设置 Easy、Normal、Hard、Lunatic 四个难度（致敬東方Project）\n对于每个难度设置有玩家操作模式和自动演示模式\n自动演示模式分为 DFS、BFS、Astar 三个算法的演示模式\n玩家操作模式中，使用 ↑  ↓  ←  → 四个按键进行移动操作，使用W S A D 四个按键可以进行更快的移动\n红色圆点是玩家，蓝色方格是起点，红色方格是终点\n游戏的目标很简单，从起点开始，到达终点\n"), _T("Help"), MB_OK);
        Button::ResetStatus();
    }
};

class AboutButton : public Button
{
public:
    AboutButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
        : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~AboutButton() = default;
protected:
    void OnClick()
    {
        MessageBox(GetHWnd(), _T("这是一个还没写完的迷宫游戏……的菜单……\n二更：现在也写完了，不止是菜单……\nbelike：↓\n这是数据结构课课程设计大作业——\n基于A*算法的迷宫小游戏开发\n作者：\n然后就没有了……\n灌注NagoriYuuuki谢谢喵~\n小Tips：作者本人并未通关过Lunatic难度（菜\nLunatic难度的首通由软件2302班的WY同学在测试时达成！"), _T("About"), MB_OK);
        Button::ResetStatus();
    }

};

class ExitButton : public Button
{
public:
    ExitButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
        : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~ExitButton() = default;
protected:
    void OnClick()
    {
        running = false;
    }
};

class EasyButton : public Button
{
public:
	EasyButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
		: Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
	~EasyButton() = default;
protected:
	void OnClick()
	{
		diff = 1;
		//MessageBox(GetHWnd(), _T("已选择简单难度"), _T("难度选择"), MB_OK);
        GameState = 3;
        Button::ResetStatus();
	}
};

class NormalButton : public Button
{
public:
	NormalButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
		: Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
	~NormalButton() = default;
protected:
	void OnClick()
	{
		diff = 2;
		//MessageBox(GetHWnd(), _T("已选择普通难度"), _T("难度选择"), MB_OK);
        GameState = 3;
        Button::ResetStatus();
	}
};

class HardButton : public Button
{
public:
	HardButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
		: Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
	~HardButton() = default;
protected:
	void OnClick()
	{
		diff = 3;
		//MessageBox(GetHWnd(), _T("已选择困难难度"), _T("难度选择"), MB_OK);
        GameState = 3;
        Button::ResetStatus();

	}
};

class LunaticButton : public Button
{
public:
	LunaticButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
		: Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
	~LunaticButton() = default;
protected:
	void OnClick()
	{
		diff = 4;
		//MessageBox(GetHWnd(), _T("已选择疯狂难度"), _T("难度选择"), MB_OK);
        GameState = 3;
        Button::ResetStatus();
	}
};

class PlayButton : public Button
{
public:
	PlayButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
		: Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
	~PlayButton() = default;
protected:
	void OnClick()
	{
		GameState = 4;
		//MessageBox(GetHWnd(), _T("Check"), _T("Check"), MB_OK);
        Button::ResetStatus();
	}
};

class AutoPlayButton :public Button
{
public:
    AutoPlayButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
        : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~AutoPlayButton() = default;
protected:
    void OnClick()
    {
        GameState = 5;
        //MessageBox(GetHWnd(), _T("Check"), _T("Check"), MB_OK);
        Button::ResetStatus();
    }
};

class DFSButton :public Button
{
public:
    DFSButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
        :Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~DFSButton() = default;
protected:
    void OnClick()
    {
        //MessageBox(GetHWnd(), _T("Check"), _T("Check"), MB_OK);
        GameState = 6;
        APmode = 1;
        ResetAPMap = false;
        isscan = false;
        judge3 = 0;
		countpath = 0;
		countvis = 0;
		printpath = false;
        isprintpath = false;
        isprintvis = false;
    }
};

class BFSButton :public Button
{
public:
    BFSButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
        :Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~BFSButton() = default;
protected:
    void OnClick()
    {
        //MessageBox(GetHWnd(), _T("Check"), _T("Check"), MB_OK);
        GameState = 6;
        APmode = 2;
        ResetAPMap = false;
        isscan = false;
        judge3 = 0;
        countpath = 0;
        countvis = 0;
        printpath = false;
		isprintpath = false;
		isprintvis = false;
    }
};

class AstarButton :public Button
{
public:
    AstarButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
        :Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~AstarButton() = default;
protected:
    void OnClick()
    {
        //MessageBox(GetHWnd(), _T("Check"), _T("Check"), MB_OK);
        GameState = 6;
        APmode = 3;
        ResetAPMap = false;
        isscan = false;
        judge3 = 0;
        countpath = 0;
        countvis = 0;
        printpath = false;
        isprintpath = false;
        isprintvis = false;
    }
};



class BackButton :public Button
{
public:
    BackButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
        :Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~BackButton() = default;
protected:
    void OnClick()
    {
        //1:主菜单
        //2:难度选择
        //3:模式选择 
        //4：玩家游戏中
        //5：奥托普雷方式（？
        //6：奥托普雷游戏中
        //7：结算页面
        //Theme theme;
        if (GameState == 2)
        {
            GameState = 1;
            isCreateMaze = false;
			//theme.SetAPlayButtonVisibility(true);
            //theme.SetDiffButtonsVisibility(false);
        }
        else if (GameState == 3)
        {
            GameState = 2;
            isCreateMaze = false;
            //theme.SetDiffButtonsVisibility(true);
            //theme.SetAPlayButtonVisibility(true);
        }
        else if (GameState == 4)
        {
            GameState = 3;
            //isCreateMaze = false;
        }
        else if (GameState == 5)
        {
            GameState = 3;
            //isCreateMaze = false;
        }
        else if (GameState == 6)
        {
			isscan = false;
			ResetAPMap = true;
            isscan = true;
            GameState = 5;
            //isCreateMaze = false;
        }
		else if (GameState == 7)
        {
            GameState = 1;
            //isCreateMaze = false;
        }
		Button::ResetStatus();
    }
};


class Theme// 主题类，包含所有按钮
{
public:
	// 按钮的位置
    RECT region_start, region_exit, region_help, region_about;
	RECT region_easy, region_normal, region_hard, region_lunatic;
    RECT region_play, region_autoplay;
	RECT region_dfs, region_bfs, region_astar;
    RECT region_back;
	// 按钮的指针，用于动态创建按钮，可以不用手动释放资源，减少内存泄漏
	StartButton* start_button;
    ExitButton* exit_button;
    HelpButton* help_button;
    AboutButton* about_button;
	EasyButton* easy_button;
	NormalButton* normal_button;
	HardButton* hard_button;
	LunaticButton* lunatic_button;
	PlayButton* play_button;
	AutoPlayButton* autoplay_button;
	BackButton* back_button;
    DFSButton* dfs_button;
    BFSButton* bfs_button;
	AstarButton* astar_button;



    void InitTheme()// 初始化按钮
    {
		setbkcolor(WHITE);// 设置背景颜色
		// 按钮位置
        region_start.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
        region_start.right = region_start.left + BUTTON_WIDTH;
        region_start.top = 100;
        region_start.bottom = region_start.top + BUTTON_HEIGHT;

        region_exit.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
        region_exit.right = region_exit.left + BUTTON_WIDTH;
        region_exit.top = 500;
        region_exit.bottom = region_exit.top + BUTTON_HEIGHT;

        region_help.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
        region_help.right = region_help.left + BUTTON_WIDTH;
        region_help.top = 220;
        region_help.bottom = region_help.top + BUTTON_HEIGHT;

        region_about.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
        region_about.right = region_about.left + BUTTON_WIDTH;
        region_about.top = 340;
        region_about.bottom = region_about.top + BUTTON_HEIGHT;

		region_easy.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
		region_easy.right = region_easy.left + BUTTON_WIDTH;
		region_easy.top = 100;
		region_easy.bottom = region_easy.top + BUTTON_HEIGHT;

		region_normal.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
		region_normal.right = region_normal.left + BUTTON_WIDTH;
		region_normal.top = 220;
		region_normal.bottom = region_normal.top + BUTTON_HEIGHT;

		region_hard.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
		region_hard.right = region_hard.left + BUTTON_WIDTH;
		region_hard.top = 340;
		region_hard.bottom = region_hard.top + BUTTON_HEIGHT;

		region_lunatic.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
		region_lunatic.right = region_lunatic.left + BUTTON_WIDTH;
		region_lunatic.top = 460;
		region_lunatic.bottom = region_lunatic.top + BUTTON_HEIGHT;

		region_back.left = (WINDOW_WIDTH) / 6 - BUTTON_WIDTH / 2 - 100;
		region_back.right = region_back.left + BUTTON_WIDTH;
		region_back.top = 50;
		region_back.bottom = region_back.top + BUTTON_HEIGHT;

		region_play.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
		region_play.right = region_play.left + BUTTON_WIDTH;
		region_play.top = 220;
		region_play.bottom = region_play.top + BUTTON_HEIGHT;

		region_autoplay.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
		region_autoplay.right = region_autoplay.left + BUTTON_WIDTH;
		region_autoplay.top = 340;
		region_autoplay.bottom = region_autoplay.top + BUTTON_HEIGHT;

		region_dfs.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
		region_dfs.right = region_dfs.left + BUTTON_WIDTH;
		region_dfs.top = 220;
		region_dfs.bottom = region_dfs.top + BUTTON_HEIGHT;

		region_bfs.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
		region_bfs.right = region_bfs.left + BUTTON_WIDTH;
		region_bfs.top = 340;
		region_bfs.bottom = region_bfs.top + BUTTON_HEIGHT;

		region_astar.left = (WINDOW_WIDTH) / 2 - BUTTON_WIDTH / 2 - 30;
		region_astar.right = region_astar.left + BUTTON_WIDTH;
		region_astar.top = 460;
		region_astar.bottom = region_astar.top + BUTTON_HEIGHT;
        //贴图路径
        start_button = new StartButton(region_start, _T("imgs/start.png"), _T("imgs/start_hovered.png"), _T("imgs/start_pushed.png"));
        exit_button = new ExitButton(region_exit, _T("imgs/exit.png"), _T("imgs/exit_hovered.png"), _T("imgs/exit_pushed.png"));
        help_button = new HelpButton(region_help, _T("imgs/help.png"), _T("imgs/help_hovered.png"), _T("imgs/help_pushed.png"));
        about_button = new AboutButton(region_about, _T("imgs/about.png"), _T("imgs/about_hovered.png"), _T("imgs/about_pushed.png"));

		easy_button = new EasyButton(region_easy, _T("imgs/easy.png"), _T("imgs/easy_hovered.png"), _T("imgs/easy_pushed.png"));
		normal_button = new NormalButton(region_normal, _T("imgs/normal.png"), _T("imgs/normal_hovered.png"), _T("imgs/normal_pushed.png"));
		hard_button = new HardButton(region_hard, _T("imgs/hard.png"), _T("imgs/hard_hovered.png"), _T("imgs/hard_pushed.png"));
		lunatic_button = new LunaticButton(region_lunatic, _T("imgs/lunatic.png"), _T("imgs/lunatic_hovered.png"), _T("imgs/lunatic_pushed.png"));

		back_button = new BackButton(region_back, _T("imgs/back.png"), _T("imgs/back_hovered.png"), _T("imgs/back_pushed.png"));

		play_button = new PlayButton(region_play, _T("imgs/play.png"), _T("imgs/play_hovered.png"), _T("imgs/play_pushed.png"));
		autoplay_button = new AutoPlayButton(region_autoplay, _T("imgs/autoplay.png"), _T("imgs/autoplay_hovered.png"), _T("imgs/autoplay_pushed.png"));

		dfs_button = new DFSButton(region_dfs, _T("imgs/dfs.png"), _T("imgs/dfs_hovered.png"), _T("imgs/dfs_pushed.png"));
		bfs_button = new BFSButton(region_bfs, _T("imgs/bfs.png"), _T("imgs/bfs_hovered.png"), _T("imgs/bfs_pushed.png"));
		astar_button = new AstarButton(region_astar, _T("imgs/astar.png"), _T("imgs/astar_hovered.png"), _T("imgs/astar_pushed.png"));
    }

	~Theme()// 析构函数，释放资源
    {
        delete start_button;
        delete exit_button;
        delete help_button;
        delete about_button;
		delete easy_button;
		delete normal_button;
		delete hard_button;
		delete lunatic_button;
		delete back_button;
		delete play_button;
		delete autoplay_button;
		delete dfs_button;
		delete bfs_button;
		delete astar_button;

    }

    void ThemeDraw()// 绘制按钮
    {
        start_button->Draw();
        exit_button->Draw();
        help_button->Draw();
        about_button->Draw();
    }

    void DiffDraw()
    {
		easy_button->Draw();
		normal_button->Draw();
		hard_button->Draw();
		lunatic_button->Draw();
    }

    void BackDraw()
    {
        back_button->Draw();
    }

    void APlayDraw()
    {
        autoplay_button->Draw();
		play_button->Draw();
    }

    void ModeDraw()
    {
        dfs_button->Draw();
		bfs_button->Draw();
		astar_button->Draw();
    }

    void ProcessEvent(const ExMessage& msg)// 处理事件
    {
        start_button->ProcessEvent(msg);
        exit_button->ProcessEvent(msg);
        help_button->ProcessEvent(msg);
        about_button->ProcessEvent(msg);

        easy_button->ProcessEvent(msg);
        normal_button->ProcessEvent(msg);
        hard_button->ProcessEvent(msg);
        lunatic_button->ProcessEvent(msg);

        back_button->ProcessEvent(msg);

        play_button->ProcessEvent(msg);
        autoplay_button->ProcessEvent(msg);

		dfs_button->ProcessEvent(msg);
		bfs_button->ProcessEvent(msg);
		astar_button->ProcessEvent(msg);
    }

    void SetThemeButtonsVisibility(bool isVisible) // 设置各个情况下的按钮的可见性
    {
        start_button->SetVisible(isVisible);
        exit_button->SetVisible(isVisible);
        help_button->SetVisible(isVisible);
        about_button->SetVisible(isVisible);
    }

	void SetDiffButtonsVisibility(bool isVisible)
	{
		easy_button->SetVisible(isVisible);
		normal_button->SetVisible(isVisible);
		hard_button->SetVisible(isVisible);
		lunatic_button->SetVisible(isVisible);
	}

    void SetBackButtonVisibility(bool isVisible)
    {
		back_button->SetVisible(isVisible);
    }

    void SetAPlayButtonVisibility(bool isVisible)
    {
		autoplay_button->SetVisible(isVisible);
		play_button->SetVisible(isVisible);
    }

    void SetModeButtonVisibility(bool isVisible)
    {
		dfs_button->SetVisible(isVisible);
		bfs_button->SetVisible(isVisible);
		astar_button->SetVisible(isVisible);
    }
};

int main()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);// 初始化图形窗口
    //int x = 300;
    //int y = 300;
	ExMessage msg;// 消息
	ExMessage Move;// 移动消息

	Theme theme;// 主题
	theme.InitTheme();// 初始化主题

    BeginBatchDraw();  // 启用双缓冲
    MazeCreate maze;
    Player player(maze);
	AutoPlay ap(maze);
    while (running)
    {
		DWORD start_time = GetTickCount();// 记录开始时间，结尾记录结束时间，计算时间差，控制帧率，减少CPU占用，15.9%→0.1%！
        while (peekmessage(&msg))//接收消息队列
        {
			theme.ProcessEvent(msg);// 处理按钮事件
			player.PlayerPosition(msg);// 处理玩家移动
            //if (msg.message == WM_MOUSEMOVE)
            //{
            //    x = msg.x;
            //    y = msg.y;
            //}
        }
        cleardevice();//主循环每次清屏，实现画面刷新

        if (GameState==1)
        {
			//cout << "主菜单" << endl;
            theme.SetThemeButtonsVisibility(true); // 非游戏状态时显示主菜单按钮
            theme.SetDiffButtonsVisibility(false);
            theme.SetBackButtonVisibility(false);
            theme.SetAPlayButtonVisibility(false);
            theme.SetModeButtonVisibility(false);
			player.SetVisible(false);
            //player.PlayerInit();

            theme.ThemeDraw();  // 绘制按钮
        }
        else if (GameState == 2)
        {
			//cout << "选择难度" << endl;
            theme.SetThemeButtonsVisibility(false);
            theme.SetDiffButtonsVisibility(true);
            theme.SetBackButtonVisibility(true);
            theme.SetAPlayButtonVisibility(false);
            theme.SetModeButtonVisibility(false);
            player.SetVisible(false);

            theme.DiffDraw();
            theme.BackDraw();
            isCreateMaze = false;
			if (diff == 1)//根据不同难度设置地图尺寸，起始位置，终点位置
            {
                maze.row = 20;
                maze.col = 20;
                maze.Start = { 1,1 };
                maze.End = { 20,20 };
            }
            else if (diff == 2)
            {
                maze.row = 40;
                maze.col = 40;
                maze.Start = { 1,1 };
                maze.End = { 40,40 };
                //cout << "diff=2" << endl;
            }
            else if (diff == 3)
            {
                maze.row = 60;
                maze.col = 60;
                maze.Start = { 1,1 };
                maze.End = { 60,60 };
            }
            else if (diff == 4)
            {
                maze.row = 80;
                maze.col = 80;
                maze.Start = { 1,1 };
                maze.End = { 80,80 };
            }

            //if (!isCreateMaze)
            //{

            //    maze.CreateMaze();
            //    isCreateMaze = true;
            //}
            judge = 0;

        }
        else if(GameState==3)
        {
            //cout << "选择模式" << endl;
            theme.SetThemeButtonsVisibility(false);
            theme.SetDiffButtonsVisibility(false);
            theme.SetBackButtonVisibility(true);
            theme.SetAPlayButtonVisibility(true);
            theme.SetModeButtonVisibility(false);
            player.SetVisible(false);


            player.PlayerInit();
            theme.APlayDraw();
            theme.BackDraw();
			if (!isCreateMaze)//保证对于一次难度选择只生成一次迷宫
            {

                maze.CreateMaze();
                isCreateMaze = true;
				cout << "Maze| :" << maze.row << " " << maze.col << endl;
                player.PlayerInit();
            }
            if (!judge)//同上
            {
                maze.PrintMaze();
                judge = 1;
            }

            //maze.EasyxPrintMaze();
        }
        else if (GameState == 4)
        {
			//cout << "玩家游戏中" << endl;
            theme.SetThemeButtonsVisibility(false);
            theme.SetDiffButtonsVisibility(false);
            theme.SetBackButtonVisibility(true);
            theme.SetAPlayButtonVisibility(false);
            theme.SetModeButtonVisibility(false);
       
			theme.BackDraw();
            //cout << "Step1" << endl;
            maze.EasyxPrintMaze();
            //cout << "Step2" << endl;
            player.SetVisible(true);
   //         cout << "Step3" << endl;

            //peekmessage(&msg);
			//player.PlayerPosition(msg);
            if(!judge2)
            {
				player.PrintMap();//调试用
                judge2 = 1;
				maze.check1();
				cout << "Maze| :" << maze.row << " " << maze.col << endl;//调试用
				player.showpos();//调试用
            }
            //maze.PrintMaze();
			//cout << "Step4" << endl;
			if (diff == 1)//对于不同难度的玩家位置的绘制，即便在迷宫 数组中的位置相同，难度不同缩放不同，映射到屏幕上的位置也不同
            {
				//EasyPlayer* easyplayer = new EasyPlayer();
    //            easyplayer->PlayerDraw();
                player.EZPlayerDraw();
            }
			else if (diff == 2)
            {
				//NormalPlayer* normalplayer = new NormalPlayer();
				//normalplayer->PlayerDraw();
                player.NMPlayerDraw();
            }
            else if (diff == 3)
            {
				//HardPlayer* hardplayer = new HardPlayer();
				//hardplayer->PlayerDraw();
				player.HDPlayerDraw();
            }
            else if (diff == 4)
            {
				//LunaticPlayer* lunaticplayer = new LunaticPlayer();
				//lunaticplayer->PlayerDraw();
                player.LNPlayerDraw();
            }

			if (player.JudgeGame())//判断游戏是否结束
            {
                //GameState = 7;
                if(diff!=4)//常规难度通关
			        MessageBox(GetHWnd(), _T("You Win!\n Ciallo~"), _T("Congratulations!"), MB_OK);
				else//Lunatic难度特殊通关
                    MessageBox(GetHWnd(), _T("YOU BEAT The Lunatic Map！\nOrz"), _T("Congratulations!"), MB_OK);
				player.GameOver = false;//游戏结束判定复位
				GameState = 1;//返回主菜单
            }


        }
        else if (GameState == 5)
        {
			//cout << "奥托普雷方式" << endl;
            theme.SetThemeButtonsVisibility(false);
            theme.SetDiffButtonsVisibility(false);
            theme.SetBackButtonVisibility(true);
            theme.SetAPlayButtonVisibility(false);
            theme.SetModeButtonVisibility(true);
            player.SetVisible(false);

            theme.ModeDraw();
            theme.BackDraw();
            //maze.EasyxPrintMaze();
        }
        else if (GameState == 6)
        {
			//cout << "奥托普雷游戏中" << endl;
            theme.SetThemeButtonsVisibility(false);
            theme.SetDiffButtonsVisibility(false);
            theme.SetBackButtonVisibility(true);
            theme.SetAPlayButtonVisibility(false);
            theme.SetModeButtonVisibility(false);
            player.SetVisible(false);
			if (!ResetAPMap)//重置地图
            {
                ResetAPMap = true;
				ap.init();//初始化
            }

            //if (!judge3)
            //{
            //    ap.Check();
            //    player.Check();
            //    judge3 = 1;
            //}
			if (!isscan)//判断触发搜索（DFS/BFS/A*
            {
                isscan = true;
                ap.scan(APmode);
            }
            //ap.PinrtVis();
            //ap.PirntPath();
            int pathsize = ap.returnPathSize();
            int vissize = ap.returnVisSize();
            if(countvis<vissize)
            {
                ap.PrintVis(countvis);
				countvis++;
            }
            if(countvis==vissize-1)
            {
                printpath = true;
				isprintvis = true;
            }
            if (isprintvis)
                ap.PrintAllVis();
            if (printpath)
            {
                if (countpath < pathsize)
				{
					ap.PrintPath(countpath);
					countpath++;
				}
            }
            if(countpath==pathsize-1)
				isprintpath = true;
            if(isprintpath)
                ap.PrintAllPath();


            
			if (!judge3)//调试用
            {
                judge3 = 1;
                ap.checkvvis();
                ap.Check();
                cout << "source scan" << endl;
            }

			theme.BackDraw();
            maze.EasyxPrintMaze();

        }
		else if (GameState == 7)//结算页面，废案/可扩展
        {
            theme.SetThemeButtonsVisibility(false);
            theme.SetDiffButtonsVisibility(false);
            theme.SetBackButtonVisibility(true);
            theme.SetAPlayButtonVisibility(false);
            theme.SetModeButtonVisibility(false);
            player.SetVisible(false);
			//MessageBox(GetHWnd(), _T("You Win!"), _T("Congratulations!"), MB_OK);

            theme.BackDraw();
        }


        //1:主菜单
        //2:难度选择
        //3:模式选择 
        //4：玩家游戏中
		//5：奥托普雷方式（？
		//6：奥托普雷游戏中
        //7：结算页面
        //solidcircle(x, y, 50);
        FlushBatchDraw();  // 双缓冲绘制

        DWORD end_time = GetTickCount();
        DWORD delta_time = end_time - start_time;

        if (delta_time < 1000 / 60) // 控制帧率,减少CPU占用
            Sleep(1000 / 60 - delta_time);
    }
    EndBatchDraw();  // 结束双缓冲

    return 0;
}
