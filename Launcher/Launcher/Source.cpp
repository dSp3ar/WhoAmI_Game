#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND hEditIP;
HWND hEditPORT;
HWND hEditTitle;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"myWindowClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"myWindowClass", L"Launcher", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    CreateWindow(L"button", L"Client", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 200, 100, 80, 30, hwnd, (HMENU)10001, hInstance, NULL);
    CreateWindow(L"button", L"Server", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 320, 100, 80, 30, hwnd, (HMENU)10002, hInstance, NULL);

    CreateWindow(L"button", L"OK", WS_CHILD | WS_VISIBLE | BS_USERBUTTON, 200, 300, 80, 30, hwnd, (HMENU)10003, hInstance, NULL);
    CreateWindow(L"button", L"Cancel", WS_CHILD | WS_VISIBLE | BS_USERBUTTON, 300, 300, 80, 30, hwnd, (HMENU)10004, hInstance, NULL);

    hEditIP = CreateWindow(L"Edit", NULL, WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 250, 150, 150, 20, hwnd, (HMENU)10005, hInstance, NULL);
    hEditPORT = CreateWindow(L"Edit", NULL, WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 250, 200, 150, 20, hwnd, (HMENU)10006, hInstance, NULL);
    hEditTitle = CreateWindow(L"Edit", NULL, WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 250, 250, 150, 20, hwnd, (HMENU)10007, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT psMess;
    RECT rectMess;
    COLORREF colorTextMess = RGB(0, 0, 0);

    RECT rectIP;
    COLORREF colorTextIP = RGB(255, 0, 0);

    RECT rectPORT;
    COLORREF colorTextPORT = RGB(255, 0, 0);

    RECT rectTITLE;
    COLORREF colorTextTITLE = RGB(255, 0, 0);

    static TCHAR type[1024] = { 0 };
    SIZE_T type_len = 0;

    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:

        if (LOWORD(wParam) == 10001)
        {
            _stprintf(type, L"client");
            SetWindowPos(hEditIP, NULL, 250, 150, 150, 20, SWP_SHOWWINDOW);
        }

        if (LOWORD(wParam) == 10002)
        {
            _stprintf(type, L"server");
            SetWindowPos(hEditIP, NULL, 250, 150, 150, 20, SWP_HIDEWINDOW);
        }

        if (LOWORD(wParam) == 10003)
        {
            TCHAR buff1[1024];
            TCHAR buff2[1024];
            TCHAR buff3[1024];
            TCHAR buff4[10] =  L"\n";
            SIZE_T buff1_len = 0;
            SIZE_T buff2_len = 0;
            SIZE_T buff3_len = 0;
            SIZE_T buff4_len = 0;

            HANDLE hFile = CreateFile(LR"(..\File\data.txt)", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

            GetWindowText(hEditIP, buff1, 1024);
            GetWindowText(hEditPORT, buff2, 1024);
            GetWindowText(hEditTitle, buff3, 1024);
            buff1_len = _tcslen(buff1);
            buff2_len = _tcslen(buff2);
            buff3_len = _tcslen(buff3);
            type_len = _tcslen(type);
            buff4_len = _tcslen(buff4);

            WriteFile(hFile, type, type_len * 2, &type_len, NULL);
            WriteFile(hFile, buff4, buff4_len * 2, &buff4_len, NULL);
            WriteFile(hFile, buff1, buff1_len * 2, &buff1_len, NULL);
            WriteFile(hFile, buff4, buff4_len * 2, &buff4_len, NULL);
            WriteFile(hFile, buff2, buff2_len * 2, &buff2_len, NULL);
            WriteFile(hFile, buff4, buff4_len * 2, &buff4_len, NULL);
            WriteFile(hFile, buff3, buff3_len * 2, &buff3_len, NULL);
            WriteFile(hFile, buff4, buff4_len * 2, &buff4_len, NULL);
            CloseHandle(hFile);
            DestroyWindow(hwnd);
        }

        if (LOWORD(wParam) == 10004)
        {
            DestroyWindow(hwnd);
        }

        break;

    case WM_PAINT:

        hdc = BeginPaint(hwnd, &psMess);


        GetClientRect(hwnd, &rectMess);
        rectMess.top = 70;
        SetTextColor(hdc, colorTextMess);
        DrawText(hdc, L"Please, select who you are:", -1, &rectMess, DT_SINGLELINE | DT_CENTER | DT_TOP);



        GetClientRect(hwnd, &rectIP);
        rectIP.top = 150;
        rectIP.left = -110;
        SetTextColor(hdc, colorTextIP);
        DrawText(hdc, L"IP:", -1, &rectIP, DT_SINGLELINE | DT_CENTER | DT_TOP);



        GetClientRect(hwnd, &rectPORT);
        rectPORT.top = 200;
        rectPORT.left = 200;
        SetTextColor(hdc, colorTextPORT);
        DrawText(hdc, L"PORT:", -1, &rectPORT, DT_SINGLELINE | DT_TOP);



        GetClientRect(hwnd, &rectTITLE);
        rectTITLE.top = 250;
        rectTITLE.left = 140;
        SetTextColor(hdc, colorTextTITLE);
        DrawText(hdc, L"The enemy title:", -1, &rectTITLE, DT_SINGLELINE | DT_TOP);


        EndPaint(hwnd, &psMess);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
