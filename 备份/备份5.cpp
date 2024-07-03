#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <graphics.h>
#include <conio.h>
#include <tchar.h>

using namespace std;

#define ULL unsigned long long

#define create_down 1
#define create_right 2
#define create_left 3
#define create_up 4
const int WINDOW_WIDTH = 1440;
const int WINDOW_HEIGHT = 1440;
const int BUTTON_WIDTH = 192;
const int BUTTON_HEIGHT = 75;

// ��Ϸ״̬-----------------------------------
int GameState = 1;
//1:���˵�
//2:�Ѷ�ѡ��
//3:ģʽѡ�� 
//4���������׷�ʽ����
//5������������Ϸ��
//6�������Ϸ��
//7������ҳ��
bool running = true;
bool isCreateMaze = false;
// ��Ϸ״̬-----------------------------------

int diff = 4;

class MazeCreate
{
public:

    struct Point
    {
        int x, y;
    };

    Point Start, End; // �����յ�
    int row, col; // �Թ�������������

    struct InBlock
    {
        int row, col, direct;
    };

    vector<InBlock> Block; // ��¼�Թ��е�ǽ

    int now_x = 1, now_y = 1; // ��ǰλ��

    int MazeMap[110][110]; // �Թ���ͼ



    void CreateMaze() // �����Prim�㷨�����Թ�
    {
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
        for (int i = 0; i <= row + 2; i++)
        {
            for (int z = 0; z <= col + 2; z++)
            {
                if (i == Start.x && z == Start.y)
                    cout << "S ";
                else if (i == End.x && z == End.y)
                    cout << "E ";
                else
                    cout << (MazeMap[i][z] == 1 ? "# " : "  ");
            }
            cout << endl;
        }
    }



    void EasyxPrintMaze()
    {
        //diffchange();
        //cout << "�Թ�������ϣ����ڻ����Թ�����" << endl;
        //settextcolor(BLACK);
        int luxx = 100, luyy = 100, sz = 10;
        if (diff == 1)
        {
			luxx = 400;
            luyy = 100;
            sz = 30;
        }
        else if (diff == 2)
        {
            sz = 15;
            luxx = 350;
            luyy = 100;
        }
        else if (diff == 3)
        {
            sz = 11;
            luxx = 270;
            luyy = 50;
        }
        else if (diff == 4)
        {
            sz = 10;
            luxx = 250;
            luyy = 10;
        }

        cout << sz << endl;
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

private:
    void init() // ��ʼ���Թ���Ĭ��Ϊȫǽ
    {
        for (int i = 0; i <= col + 2; i++)
            for (int z = 0; z <= row + 2; z++)
                MazeMap[z][i] = 1;
    }

    void push(int x, int y, int dir) // ���ǽ
    {
        Block.push_back({ x, y, dir });
    }

    int CountWall() // ���㵱ǰλ����Χ��ǽ������
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

class Button // ��ť����
{

public:
    Button(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
    {
        region = rect;
        loadimage(&img_idle, path_img_idle);
        loadimage(&img_hovered, path_img_hovered);
        loadimage(&img_pushed, path_img_pushed);
        visible = true; // ��ťĬ�Ͽɼ�
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
        cout << "------------" << endl;
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
		GameState = 3;
		MessageBox(GetHWnd(), _T("Check"), _T("Check"), MB_OK);
	}
};

class AutoButton :public Button
{
public:
    AutoButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
        : Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
    ~AutoButton() = default;
protected:
    void OnClick()
    {
        GameState = 4;
        MessageBox(GetHWnd(), _T("Check"), _T("Check"), MB_OK);
    }
};


class Theme// �����࣬�������а�ť
{
public:
    RECT region_start, region_exit, region_help, region_about;
	RECT region_easy, region_normal, region_hard, region_lunatic;
    RECT region_play, autoplay;
	RECT region_dfs, region_bfs, region_astar;

    StartButton* start_button;
    ExitButton* exit_button;
    HelpButton* help_button;
    AboutButton* about_button;
	EasyButton* easy_button;
	NormalButton* normal_button;
	HardButton* hard_button;
	LunaticButton* lunatic_button;


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


        start_button = new StartButton(region_start, _T("imgs/start.png"), _T("imgs/start_hovered.png"), _T("imgs/start_pushed.png"));
        exit_button = new ExitButton(region_exit, _T("imgs/exit.png"), _T("imgs/exit_hovered.png"), _T("imgs/exit_pushed.png"));
        help_button = new HelpButton(region_help, _T("imgs/help.png"), _T("imgs/help_hovered.png"), _T("imgs/help_pushed.png"));
        about_button = new AboutButton(region_about, _T("imgs/about.png"), _T("imgs/about_hovered.png"), _T("imgs/about_pushed.png"));

		easy_button = new EasyButton(region_easy, _T("imgs/diff/easy.png"), _T("imgs/diff/easy_hovered.png"), _T("imgs/diff/easy_pushed.png"));
		normal_button = new NormalButton(region_normal, _T("imgs/diff/normal.png"), _T("imgs/diff/normal_hovered.png"), _T("imgs/diff/normal_pushed.png"));
		hard_button = new HardButton(region_hard, _T("imgs/diff/hard.png"), _T("imgs/diff/hard_hovered.png"), _T("imgs/diff/hard_pushed.png"));
		lunatic_button = new LunaticButton(region_lunatic, _T("imgs/diff/lunatic.png"), _T("imgs/diff/lunatic_hovered.png"), _T("imgs/diff/lunatic_pushed.png"));

    }

    ~Theme()
    {
        delete start_button;
        delete exit_button;
        delete help_button;
        delete about_button;
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
};

int main()
{
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    int x = 300;
    int y = 300;
    ExMessage msg;

    Theme theme;
    theme.InitTheme();

    BeginBatchDraw();  // ����˫����
    MazeCreate maze;
    if(diff==1)
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
    while (running)
    {
        DWORD start_time = GetTickCount();
        while (peekmessage(&msg))
        {
            theme.ProcessEvent(msg);
            if (msg.message == WM_MOUSEMOVE)
            {
                x = msg.x;
                y = msg.y;
            }
        }
        cleardevice();

        if (GameState==1)
        {

            theme.SetThemeButtonsVisibility(true); // ����Ϸ״̬ʱ��ʾ���˵���ť
            theme.ThemeDraw();  // ���ư�ť
        }
        else if (GameState == 2)
        {
			theme.SetThemeButtonsVisibility(false);//�����Ѷ�ѡ��ҳ�棬�������˵���ť
			theme.SetDiffButtonsVisibility(true);
			theme.DiffDraw();
        }
        else if(GameState==3)
        {
            cout << "��Ϸ��ʼ" << endl;
            theme.SetThemeButtonsVisibility(false); // ��Ϸ��ʼʱ�������а�ť
            if (!isCreateMaze)
            {
                maze.CreateMaze();
                isCreateMaze = true;
            }
            maze.EasyxPrintMaze();
        }

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
