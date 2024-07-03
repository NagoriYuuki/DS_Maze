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

// 游戏状态-----------------------------------
bool isStartGame = false;
bool running = true;
bool isCreateMaze = false;
// 游戏状态-----------------------------------

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

    void CreateMaze() // 随机化Prim算法生成迷宫
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
        cout << "迷宫生成完毕，正在绘制迷宫……" << endl;
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
    void init() // 初始化迷宫，默认为全墙
    {
        for (int i = 0; i <= col + 2; i++)
            for (int z = 0; z <= row + 2; z++)
                MazeMap[z][i] = 1;
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

};

class Button // 按钮基类
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

    void Draw() // 绘制按钮
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

private:
    RECT region;
    IMAGE img_idle;
    IMAGE img_hovered;
    IMAGE img_pushed;
    Status status = Status::Idle;

};

// 继承Button基类，实现各自的OnClick函数
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
        MessageBox(GetHWnd(), _T("这是个还没有写的Help，然后就没有然后了"), _T("Help"), MB_OK);
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
        MessageBox(GetHWnd(), _T("这是一个还没写完的迷宫游戏……的菜单……"), _T("About"), MB_OK);
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

class Theme// 主题类，包含所有按钮
{
public:
    RECT region_start, region_exit, region_help, region_about;
    StartButton* start_button;
    ExitButton* exit_button;
    HelpButton* help_button;
    AboutButton* about_button;

    void InitTheme()// 初始化按钮
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

    void Draw()// 绘制按钮
    {
        start_button->Draw();
        exit_button->Draw();
        help_button->Draw();
        about_button->Draw();
    }

    void ProcessEvent(const ExMessage& msg)// 处理事件
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
            cout << "游戏开始" << endl;
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
        cleardevice();//清屏
        if (!isStartGame)
            theme.Draw();//绘制按钮

        //solidcircle(x, y, 50);
        FlushBatchDraw();

        DWORD end_time = GetTickCount();
        DWORD delta_time = end_time - start_time;

        if (delta_time < 1000 / 60) // 控制帧率,减少CPU占用
            Sleep(1000 / 60 - delta_time);
    }
    EndBatchDraw();

    return 0;
}