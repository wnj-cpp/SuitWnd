// 标准windows程序框架
#include <windows.h>
#include "stdafx.h"
#include "resource.h"
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE            g_hInst;            // 当前实例

// 此代码模块中包含的函数的前向声明
BOOL                InitInstance(HINSTANCE, int);
ATOM                MyRegisterClass(HINSTANCE, LPTSTR);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void                OnCreate(HWND);
void                OnPaint(HWND);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    //类似哈希表，映射消息和窗口，本程序只对主窗口加了映射
    HACCEL hAccelTable;
    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MWIND));

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    TCHAR szTitle[MAX_LOADSTRING];        // 标题栏文本
    TCHAR szWindowClass[MAX_LOADSTRING];    // 主窗口类名

    g_hInst = hInstance; // 将实例句柄存储在全局变量中

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_MWIND, szWindowClass, MAX_LOADSTRING);

//如果程序已经在运行，则将焦点置于窗口上，然后退出
//#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    hWnd = FindWindow(szWindowClass, szTitle);
    if (hWnd)
    {
        // 将焦点置于最前面的子窗口
        // “| 0x00000001”用于将所有附属窗口置于前台并
        // 激活这些窗口。
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
        return 0;
    }
//#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP

    if (!MyRegisterClass(hInstance, szWindowClass))
    {
        return FALSE;
    }

    UINT width = GetSystemMetrics(SM_CXSCREEN);
    UINT height = GetSystemMetrics(SM_CYSCREEN);

    hWnd = CreateWindowEx(WS_EX_TOPMOST,
                        szWindowClass,
                        szTitle,
                        SH_WS_PANEL,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        width,
                        height,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
    WNDCLASSEX wc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szWindowClass;
    wc.lpfnWndProc = WndProc;
    wc.style = CS_HREDRAW|CS_VREDRAW;
    //wc.style = CS_DBLCLKS;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MWIND));
//    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.lpszMenuName = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    wc.cbSize = sizeof (WNDCLASSEX);

    return RegisterClassEx(&wc);
}

void OnCreate(HWND hWnd)
{
	 //设置窗口扩展风格
	SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)|0x80000);
	typedef BOOL (WINAPI *FSetLayeredWindowAttributes)(HWND,COLORREF,BYTE,DWORD);
	FSetLayeredWindowAttributes SetLayeredWindowAttributes ;
	HINSTANCE hInst = LoadLibrary("User32.DLL");
	SetLayeredWindowAttributes = (FSetLayeredWindowAttributes)GetProcAddress(hInst,"SetLayeredWindowAttributes");
	if (SetLayeredWindowAttributes)
		SetLayeredWindowAttributes(hWnd,RGB(255,255,255),200,LWA_ALPHA);//这里设置透明度和透明方式1,2两种
	FreeLibrary(hInst);
}

void OnPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;

    hdc = BeginPaint(hWnd, &ps);

    // TODO: 在此添加任意绘图代码...

    GetClientRect(hWnd,&rect);
    //DrawText(hdc,TEXT("Hello!"),-1,&rect,DT_CENTER);


    EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
            OnCreate(hWnd);
            break;
        case WM_PAINT:
            OnPaint(hWnd);
            break;
        case WM_LBUTTONDOWN:
            SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
