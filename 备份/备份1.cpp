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
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int BUTTON_WIDTH = 192;
const int BUTTON_HEIGHT = 75;

// ��Ϸ״̬-----------------------------------
bool isStartGame = false;
bool running = true;
bool isCreateMaze = false;
// ��Ϸ״̬-----------------------------------

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
        cout << "�Թ�������ϣ����ڻ����Թ�����" << endl;
        settextcolor(BLACK);
        int lux = 150, luy = 150;
        for (int i = 0; i <= row + 2; i++)
        {
            for (int z = 0; z <= col + 2; z++)
            {
                int nowposx = lux + 20 * z;
                int nowposy = luy + 20 * i;
                if (i == Start.x && z == Start.y)
                    outtextxy(nowposx, nowposy, L"S");
                else if (i == End.x && z == End.y)
                    outtextxy(nowposx, nowposy, L"E");
                else
                    outtextxy(nowposx, nowposy, MazeMap[i][z] == 1 ? L"#" : L" ");
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
    }
    ~Button() = default;

    void Draw() // ���ư�ť
    {
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
        isStartGame = true;
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

class Theme// �����࣬�������а�ť
{
public:
    RECT region_start, region_exit, region_help, region_about;
    StartButton* start_button;
    ExitButton* exit_button;
    HelpButton* help_button;
    AboutButton* about_button;

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


        start_button = new StartButton(region_start, _T("imgs/start.png"), _T("imgs/start_hovered.png"), _T("imgs/start_pushed.png"));
        exit_button = new ExitButton(region_exit, _T("imgs/exit.png"), _T("imgs/exit_hovered.png"), _T("imgs/exit_pushed.png"));
        help_button = new HelpButton(region_help, _T("imgs/help.png"), _T("imgs/help_hovered.png"), _T("imgs/help_pushed.png"));
        about_button = new AboutButton(region_about, _T("imgs/about.png"), _T("imgs/about_hovered.png"), _T("imgs/about_pushed.png"));

    }

    ~Theme()
    {
        delete start_button;
        delete exit_button;
        delete help_button;
        delete about_button;
    }

    void Draw()// ���ư�ť
    {
        start_button->Draw();
        exit_button->Draw();
        help_button->Draw();
        about_button->Draw();
    }

    void ProcessEvent(const ExMessage& msg)// �����¼�
    {
        start_button->ProcessEvent(msg);
        exit_button->ProcessEvent(msg);
        help_button->ProcessEvent(msg);
        about_button->ProcessEvent(msg);
    }
};

int main()
{
    initgraph(1280, 720);
    int x = 300;
    int y = 300;
    ExMessage msg;

    Theme theme;
    theme.InitTheme();

    BeginBatchDraw();
    MazeCreate maze;
    maze.row = 20;
    maze.col = 20;
    maze.Start = { 1,1 };
    maze.End = { 20,20 };
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
        if (isStartGame)
        {
            cout << "��Ϸ��ʼ" << endl;
            cleardevice();

            if (!isCreateMaze)
            {
                maze.CreateMaze();
                isCreateMaze = true;
            }
            maze.EasyxPrintMaze();
            //maze.PrintMaze();
            //isStartGame = false;
        }
        cleardevice();//����
        if (!isStartGame)
            theme.Draw();//���ư�ť

        //solidcircle(x, y, 50);
        FlushBatchDraw();

        DWORD end_time = GetTickCount();
        DWORD delta_time = end_time - start_time;

        if (delta_time < 1000 / 60) // ����֡��,����CPUռ��
            Sleep(1000 / 60 - delta_time);
    }
    EndBatchDraw();

    return 0;
}