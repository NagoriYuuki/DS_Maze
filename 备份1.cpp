#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <graphics.h>
#include <conio.h>
#include <tchar.h>
#include "Create_Maze.h"

using namespace std;

#define ULL unsigned long long


const int WINDOW_WIDTH = 1440;
const int WINDOW_HEIGHT = 1440;
const int BUTTON_WIDTH = 192;
const int BUTTON_HEIGHT = 75;

// ��Ϸ״̬-----------------------------------
int GameState = 1;
//1:���˵�
//2:�Ѷ�ѡ��
//3:ģʽѡ�� 
//4�������Ϸ��
//5���������׷�ʽ����
//6������������Ϸ��
//7������ҳ��
bool running = true;
bool isCreateMaze = false;
// ��Ϸ״̬-----------------------------------


//�����ñ���----------------------------------
bool judge = 0;
//�����ñ���----------------------------------


class Button // ��ť����
{

public:
    Button(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
    {
        region = rect;
        loadimage(&img_idle, path_img_idle);
        loadimage(&img_hovered, path_img_hovered);
        loadimage(&img_pushed, path_img_pushed);
        visible = false; // ��ťĬ�ϲ��ɼ�
    }
    ~Button() = default;

    void Draw() // ���ư�ť
    {
        if (!visible)
            return; // ��ť���ɼ�ʱ������
        switch (status)
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

    void ProcessEvent(const ExMessage& msg)
    {
        if (!visible) 
            return; // ��ť���ɼ�ʱ�������¼�
        //cout << "------------" << endl;
        switch (msg.message)
        {
        case WM_MOUSEMOVE: // ����ƶ��¼�
            if (status == Status::Idle && CheckMousePos(msg.x, msg.y)) // ����ƶ�����ť��
                status = Status::Hovered;
            else if (status == Status::Hovered && !CheckMousePos(msg.x, msg.y)) // ����Ƴ���ť
                status = Status::Idle;
            break;
        case WM_LBUTTONDOWN: // �����������¼�
            if (status == Status::Hovered) // ����ڰ�ť�ϰ���
                status = Status::Pushed;
            break;
        case WM_LBUTTONUP: // ������̧���¼�
            if (status == Status::Pushed) // ����ڰ�ť��̧��
            {
                status = Status::Hovered;
                OnClick();
            }
            break;
        default: // �����Ƿ��¼�
            break;
        }

    }

    void ResetStatus()
    {
        status = Status::Idle;
    }


    void SetVisible(bool isVisible)
    {
        visible = isVisible;
    }

protected:
    virtual void OnClick() = 0; // ���麯���������ťʱ�Ĳ���

private:

    bool CheckMousePos(int x, int y) // �������Ƿ��ڰ�ť��
    {
        return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
    }

    enum class Status // ��ť��״̬ö�ٱ���
    {
        Idle = 0,
        Hovered,
        Pushed
    };

private:
    RECT region;
    IMAGE img_idle;
    IMAGE img_hovered;
    IMAGE img_pushed;
    Status status = Status::Idle;
    bool visible; // ��ť�ɼ��Ա�־
};

// �̳�Button���࣬ʵ�ָ��Ե�OnClick����
class StartButton : public Button
{
public:
    StartButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
        : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~StartButton() = default;
protected:
    void OnClick()
    {
        GameState = 2;
        SetVisible(false); // ��Ϸ��ʼʱ���ذ�ť
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
        MessageBox(GetHWnd(), _T("���Ǹ���û��д��Help��Ȼ���û��Ȼ����"), _T("Help"), MB_OK);
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
        MessageBox(GetHWnd(), _T("����һ����ûд����Թ���Ϸ�����Ĳ˵�����"), _T("About"), MB_OK);
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
		//MessageBox(GetHWnd(), _T("��ѡ����Ѷ�"), _T("�Ѷ�ѡ��"), MB_OK);
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
		//MessageBox(GetHWnd(), _T("��ѡ����ͨ�Ѷ�"), _T("�Ѷ�ѡ��"), MB_OK);
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
		//MessageBox(GetHWnd(), _T("��ѡ�������Ѷ�"), _T("�Ѷ�ѡ��"), MB_OK);
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
		//MessageBox(GetHWnd(), _T("��ѡ�����Ѷ�"), _T("�Ѷ�ѡ��"), MB_OK);
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
        //1:���˵�
        //2:�Ѷ�ѡ��
        //3:ģʽѡ�� 
        //4�������Ϸ��
        //5���������׷�ʽ����
        //6������������Ϸ��
        //7������ҳ��
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


class Theme// �����࣬�������а�ť
{
public:
    RECT region_start, region_exit, region_help, region_about;
	RECT region_easy, region_normal, region_hard, region_lunatic;
    RECT region_play, region_autoplay;
	RECT region_dfs, region_bfs, region_astar;
    RECT region_back;

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



    void InitTheme()// ��ʼ����ť
    {
        setbkcolor(WHITE);
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

		region_back.left = (WINDOW_WIDTH) / 6 - BUTTON_WIDTH / 2 - 150;
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

        start_button = new StartButton(region_start, _T("imgs/start.png"), _T("imgs/start_hovered.png"), _T("imgs/start_pushed.png"));
        exit_button = new ExitButton(region_exit, _T("imgs/exit.png"), _T("imgs/exit_hovered.png"), _T("imgs/exit_pushed.png"));
        help_button = new HelpButton(region_help, _T("imgs/help.png"), _T("imgs/help_hovered.png"), _T("imgs/help_pushed.png"));
        about_button = new AboutButton(region_about, _T("imgs/about.png"), _T("imgs/about_hovered.png"), _T("imgs/about_pushed.png"));

		easy_button = new EasyButton(region_easy, _T("imgs/diff/easy.png"), _T("imgs/diff/easy_hovered.png"), _T("imgs/diff/easy_pushed.png"));
		normal_button = new NormalButton(region_normal, _T("imgs/diff/normal.png"), _T("imgs/diff/normal_hovered.png"), _T("imgs/diff/normal_pushed.png"));
		hard_button = new HardButton(region_hard, _T("imgs/diff/hard.png"), _T("imgs/diff/hard_hovered.png"), _T("imgs/diff/hard_pushed.png"));
		lunatic_button = new LunaticButton(region_lunatic, _T("imgs/diff/lunatic.png"), _T("imgs/diff/lunatic_hovered.png"), _T("imgs/diff/lunatic_pushed.png"));

		back_button = new BackButton(region_back, _T("imgs/back.png"), _T("imgs/back_hovered.png"), _T("imgs/back_pushed.png"));

		play_button = new PlayButton(region_play, _T("imgs/mode/play.png"), _T("imgs/mode/play_hovered.png"), _T("imgs/mode/play_pushed.png"));
		autoplay_button = new AutoPlayButton(region_autoplay, _T("imgs/mode/autoplay.png"), _T("imgs/mode/autoplay_hovered.png"), _T("imgs/mode/autoplay_pushed.png"));

		dfs_button = new DFSButton(region_dfs, _T("imgs/mode/dfs.png"), _T("imgs/mode/dfs_hovered.png"), _T("imgs/mode/dfs_pushed.png"));
		bfs_button = new BFSButton(region_bfs, _T("imgs/mode/bfs.png"), _T("imgs/mode/bfs_hovered.png"), _T("imgs/mode/bfs_pushed.png"));
		astar_button = new AstarButton(region_astar, _T("imgs/mode/astar.png"), _T("imgs/mode/astar_hovered.png"), _T("imgs/mode/astar_pushed.png"));
    }

    ~Theme()
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

    void ThemeDraw()// ���ư�ť
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

    void ProcessEvent(const ExMessage& msg)// �����¼�
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

    void SetThemeButtonsVisibility(bool isVisible) // �������а�ť�Ŀɼ���
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
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    //int x = 300;
    //int y = 300;
    ExMessage msg;

    Theme theme;
    theme.InitTheme();

    BeginBatchDraw();  // ����˫����
    MazeCreate maze;
    while (running)
    {
        DWORD start_time = GetTickCount();
        while (peekmessage(&msg))
        {
            theme.ProcessEvent(msg);
            //if (msg.message == WM_MOUSEMOVE)
            //{
            //    x = msg.x;
            //    y = msg.y;
            //}
        }
        cleardevice();

        if (GameState==1)
        {
			//cout << "���˵�" << endl;
            theme.SetThemeButtonsVisibility(true); // ����Ϸ״̬ʱ��ʾ���˵���ť
            theme.SetDiffButtonsVisibility(false);
            theme.SetBackButtonVisibility(false);
            theme.SetAPlayButtonVisibility(false);
            theme.SetModeButtonVisibility(false);

            theme.ThemeDraw();  // ���ư�ť
        }
        else if (GameState == 2)
        {
			//cout << "ѡ���Ѷ�" << endl;
            theme.SetThemeButtonsVisibility(false);
            theme.SetDiffButtonsVisibility(true);
            theme.SetBackButtonVisibility(true);
            theme.SetAPlayButtonVisibility(false);
            theme.SetModeButtonVisibility(false);

            theme.DiffDraw();
            theme.BackDraw();
            if (diff == 1)
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
            //cout << "ѡ��ģʽ" << endl;
            theme.SetThemeButtonsVisibility(false);
            theme.SetDiffButtonsVisibility(false);
            theme.SetBackButtonVisibility(true);
            theme.SetAPlayButtonVisibility(true);
            theme.SetModeButtonVisibility(false);

            theme.APlayDraw();
            theme.BackDraw();
            if (!isCreateMaze)
            {

                maze.CreateMaze();
                isCreateMaze = true;
            }

            //maze.EasyxPrintMaze();
        }
        else if (GameState == 4)
        {
			//cout << "�����Ϸ��" << endl;
            theme.SetThemeButtonsVisibility(false);
            theme.SetDiffButtonsVisibility(false);
            theme.SetBackButtonVisibility(true);
            theme.SetAPlayButtonVisibility(false);
            theme.SetModeButtonVisibility(false);

			theme.BackDraw();
            maze.EasyxPrintMaze();
            if (!judge)
            {
                maze.PrintMaze();
                judge = 1;
            }

        }
        else if (GameState == 5)
        {
			//cout << "�������׷�ʽ" << endl;
            theme.SetThemeButtonsVisibility(false);
            theme.SetDiffButtonsVisibility(false);
            theme.SetBackButtonVisibility(true);
            theme.SetAPlayButtonVisibility(false);
            theme.SetModeButtonVisibility(true);

            theme.ModeDraw();
            theme.BackDraw();
 /*           maze.EasyxPrintMaze();*/
        }
        else if (GameState == 6)
        {
            theme.SetThemeButtonsVisibility(false);
            theme.SetDiffButtonsVisibility(false);
            theme.SetBackButtonVisibility(true);
            theme.SetAPlayButtonVisibility(false);
            theme.SetModeButtonVisibility(false);

			theme.BackDraw();
            maze.EasyxPrintMaze();
        }
        //1:���˵�
        //2:�Ѷ�ѡ��
        //3:ģʽѡ�� 
        //4�������Ϸ��
		//5���������׷�ʽ����
		//6������������Ϸ��
        //7������ҳ��
        //solidcircle(x, y, 50);
        FlushBatchDraw();  // ˫�������

        DWORD end_time = GetTickCount();
        DWORD delta_time = end_time - start_time;

        if (delta_time < 1000 / 60) // ����֡��,����CPUռ��
            Sleep(1000 / 60 - delta_time);
    }
    EndBatchDraw();  // ����˫����

    return 0;
}
