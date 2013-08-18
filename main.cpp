// ��׼windows������
#include <windows.h>
#include "stdafx.h"
#include "resource.h"
#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE            g_hInst;            // ��ǰʵ��

// �˴���ģ���а����ĺ�����ǰ������
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

    //���ƹ�ϣ��ӳ����Ϣ�ʹ��ڣ�������ֻ�������ڼ���ӳ��
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
    TCHAR szTitle[MAX_LOADSTRING];        // �������ı�
    TCHAR szWindowClass[MAX_LOADSTRING];    // ����������

    g_hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_MWIND, szWindowClass, MAX_LOADSTRING);

//��������Ѿ������У��򽫽������ڴ����ϣ�Ȼ���˳�
//#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    hWnd = FindWindow(szWindowClass, szTitle);
    if (hWnd)
    {
        // ������������ǰ����Ӵ���
        // ��| 0x00000001�����ڽ����и�����������ǰ̨��
        // ������Щ���ڡ�
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
	 //���ô�����չ���
	SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)|0x80000);
	typedef BOOL (WINAPI *FSetLayeredWindowAttributes)(HWND,COLORREF,BYTE,DWORD);
	FSetLayeredWindowAttributes SetLayeredWindowAttributes ;
	HINSTANCE hInst = LoadLibrary("User32.DLL");
	SetLayeredWindowAttributes = (FSetLayeredWindowAttributes)GetProcAddress(hInst,"SetLayeredWindowAttributes");
	if (SetLayeredWindowAttributes)
		SetLayeredWindowAttributes(hWnd,RGB(255,255,255),200,LWA_ALPHA);//��������͸���Ⱥ�͸����ʽ1,2����
	FreeLibrary(hInst);
}

void OnPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;

    hdc = BeginPaint(hWnd, &ps);

    // TODO: �ڴ���������ͼ����...

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
