// HookDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "HookDll.h"

void SetHook(HWND hwnd)
{
    g_hwnd = hwnd;
    assert(hwnd);
    HINSTANCE hModule = GetModuleHandle(TEXT("HookDll"));
    assert(hModule);
    g_hHKey = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hModule, 0);
    g_hHMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, hModule, 0);
    g_hHKeyLL = SetWindowsHookEx(WH_KEYBOARD_LL, LLKeyboardProc, hModule, 0); 
    assert(g_hHKey);
    assert(g_hHMouse);
    g_uTime = 10;
    int cx = GetSystemMetrics(SM_CXSCREEN);
    int cy = GetSystemMetrics(SM_CYSCREEN);
	InitializeCriticalSection(&g_ce);

    g_uF3cnt = 0;

    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0,
        GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

    g_hwndE = CreateWindow(L"Button", L"INFO", WS_POPUP, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
    assert(g_hwndE);
    SetWindowText(g_hwndE, TEXT("Locked."));
    SetWindowPos(g_hwndE, HWND_TOPMOST, cx - 150, 0, 150, 40, SWP_HIDEWINDOW);

    SetWindowLong(g_hwndE, GWL_EXSTYLE, GetWindowLong(g_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(g_hwndE, 0, 150, LWA_ALPHA);

    g_hwndBS = CreateWindow(L"Edit", L"STATUS", 0, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
    assert(g_hwndBS);
    SetWindowText(g_hwndBS, TEXT("OK."));
    SetWindowPos(g_hwndBS, HWND_TOPMOST, 0, 0, 210, 60, SWP_HIDEWINDOW);

    Set0();
    SetF1();

    SetTimer(hwnd, 1, 1000, TimerProc);
}

LRESULT CALLBACK LLKeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    BOOL bEatKey = FALSE;
    if (code == HC_ACTION)
    {
        switch(wParam)
        { 
        case WM_KEYDOWN:    
        case WM_SYSKEYDOWN: 
        case WM_KEYUP: 
        case WM_SYSKEYUP:   
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam; 
            bEatKey =
                ((p->vkCode == VK_TAB)&&((p->flags & LLKHF_ALTDOWN) != 0))||
                ((p->vkCode == VK_ESCAPE)&&((p->flags & LLKHF_ALTDOWN)!=0))||
                ((p->vkCode == VK_ESCAPE)&&((GetKeyState(VK_CONTROL) & 0x8000)!=0))||
                ((p->vkCode == VK_SPACE)&&((GetKeyState(VK_CONTROL) & 0x8000)!=0))||
                (p->vkCode == VK_LWIN)||(p->vkCode == VK_RWIN);
            break;
        }
    }
    if (bEatKey)
    {
        return TRUE;
    }
    else
    {
        return CallNextHookEx(g_hHKeyLL, code, wParam, lParam);
    }
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION)
    {
        if (lParam & 0x80000000)
        {
            switch (wParam)
            {
            case VK_F3:
                if ((GetKeyState(VK_RCONTROL) & 0x8000) && (GetKeyState(VK_LSHIFT) & 0x8000)
                    && g_uCase == 0x0)
                {
                    g_uF3cnt++;
                    if (g_uF3cnt >= 3)
                    {
                        SetF3();
                    }
                }
                break;
            case VK_F4:
                if ((GetKeyState(VK_RCONTROL) & 0x8000) && (GetKeyState(VK_LSHIFT) & 0x8000)
                    && g_uCase == 0x0)
                {
                    g_uTime *= 2;
                }
                break;
            case VK_F5:
                if ((GetKeyState(VK_RCONTROL) & 0x8000) && (GetKeyState(VK_LSHIFT) & 0x8000)
                    && g_uCase == 0x0)
                {
                    g_uTime /= 2;
                }
                break;
            case VK_F6:
                SetF6();
                break;
            case VK_F7:
                {
                    g_uF3cnt = 0;
                    if ((g_uCase & 0x1) == 0x1)
                    {
                        break;
                    }
                    if ((g_uCase & 0x10) == 0x10)
                    {
                        g_uCase = 0;
                        KillTimer(g_hwnd, 2);
                        SendMessage(g_hwnd, WM_COMMAND, 0x1234, 0);
                        SetTimer(g_hwnd, 1, 1000, TimerProc);
                        g_hFindFile = NULL;
                    }
                    else
                    {
                        KillTimer(g_hwnd, 1);
                        g_hFindFile = NULL;
                        g_uCase = 0x10;
                        SetTimer(g_hwnd, 2, 5000, SetPicProc);
                        ShowNextPic();
                    }
                }
                break;
            case VK_F1:
                SetF1();
                break;
            case VK_F8:
                if ((g_uCase & 0x10) == 0x10)
                {
                    ShowNextPic();
                }                
                break;
            case '1':
                Set1();
                break;
            case '2':
                Set2();
                break;
            case '3':
                Set3();
                break;
            case '4':
                Set4();
                break;
            case '5':
                Set5();
                break;
            case '6':
                Set6();
                break;
            case '7':
                Set7();
                break;
            case '0':
                Set0();
                break;
            }            
        }
    }
    return CallNextHookEx(g_hHKey, code, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam)
{
    return 1;
}

VOID CALLBACK TimerProc(HWND hwnd, UINT, UINT_PTR, DWORD)
{
//     if (g_bHid)
//     {
//         ShowWindow(g_hwnd, SW_HIDE);
//         ShowWindow(g_hwndE, SW_HIDE);
//     }
//     else
    if (g_bLockMouse)
    {
        SetWindowPos(g_hwnd, HWND_TOPMOST, 0, 0,
            GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
        ShowWindow(g_hwnd, SW_MAXIMIZE);
    }

//     SendMessage(hwnd, WM_ACTIVATE, 0, 0);

    if (g_bLockMouse)
    {
        ShowCursor(FALSE);
        static const RECT rtC = {0};
        ClipCursor(&rtC);
        const RECT rt = {0};
        SetCursorPos(0, 0);
    }

    HDC hDC = GetDC(hwnd);
    RECT rect = {0};
    GetClientRect(hwnd, &rect);

    HDC hBufDC = CreateCompatibleDC(hDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left, rect.bottom - rect.top);

    SelectObject(hBufDC, hBitmap);

    SetBkMode(hBufDC, OPAQUE);
    SetBkColor(hBufDC, RGB(0, 0, 0));

    HFONT hFont = CreateFont(30, 15, 0, 0, 16, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("楷体"));

    HFONT hTFont = CreateFont(30, 12, 0, 0, 16, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Times New Roman"));

    HFONT hMsgFnt = CreateFont(30, 12, 0, 0, 16, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("微软雅黑"));

    BitBlt(hBufDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
        hDC, 0, 0, SRCCOPY);

    //////////////////////////////////////////////////////////////////////////

    RECT rtText = {rect.left, rect.bottom - 30, rect.left + 500, rect.bottom};

    WCHAR szMessage[30] = {0};
    swprintf_s(szMessage, TEXT("剩余： %03d秒        "), g_uTime);

    SetTextColor(hBufDC, RGB(100, 100, 100));
    SelectObject(hBufDC, hFont);
    DrawText(hBufDC, szMessage, lstrlen(szMessage), &rtText, DT_LEFT);

	//////////////////////////////////////////////////////////////////////////

	EnterCriticalSection(&g_ce);
	if (g_uTimMsg > 0)
	{
		RECT rtTxt = {rect.left + 200, rect.bottom / 2 + 160, rect.right - 200, rect.bottom - 60};
		SetTextColor(hBufDC, RGB(0xff, 0xcc, 0x66));
		HGLOBAL hOld = SelectObject(hBufDC, hMsgFnt);
		DrawText(hBufDC, g_sMsg, g_sMsg.GetLength(), &rtTxt, DT_WORDBREAK | DT_CENTER);
		SelectObject(hBufDC, hOld);
		g_uTimMsg--;
	}
	else if (g_uTimMsg == 0)
	{
		RECT rtTxt = {rect.left + 200, rect.bottom / 2 + 130, rect.right - 200, rect.bottom - 60};
		ExtTextOut(hBufDC, 0, 0, ETO_OPAQUE, &rtTxt, NULL, 0, NULL);
		g_uTimMsg--;
	}
	LeaveCriticalSection(&g_ce);

    time_t currTime = time(NULL);
    tm _tm = {0};
    WCHAR szTime[50] = {0};
    localtime_s(&_tm, &currTime);
    wcsftime(szTime, sizeof(szTime), TEXT("%c"), &_tm);

    RECT rtTime = {rect.left + 200, rect.bottom / 2 + 50, rect.right - 200, rect.bottom / 2 + 120};

    SetTextColor(hBufDC, RGB(0, 0, 250));
    SelectObject(hBufDC, hTFont);
    DrawText(hBufDC, szTime, lstrlen(szTime), &rtTime, DT_CENTER);

    {
//         RECT rtE;
//         GetWindowRect(g_hwndE, &rtE);
//         MoveWindow(g_hwndE, rtE.left, rt.top, rtE.right - rtE.left, rtE.bottom - rtE.top, FALSE);
        SetWindowText(g_hwndE, szTime);
        UpdateWindow(g_hwndE);
        UpdateWindow(g_hwnd);
    }

    //////////////////////////////////////////////////////////////////////////

    BitBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
        hBufDC, 0, 0, SRCCOPY);

    DeleteObject(hFont);
    DeleteObject(hBitmap);
    DeleteDC(hBufDC);
    ReleaseDC(hwnd, hDC);

    if (g_uTime < 0)
    {
        SetF3();
    }
    if (g_bST)
    {
        g_uTime--;
    }
    g_uTBcnt++;

    switch (g_uTBcnt)
    {
    case 720:        
        SetTimer(hwnd, 0x1001, 100, MoveEProc);
        break;
    case 870:
        SetTimer(hwnd, 0x1002, 100, MoveEProc);
        break;
    case 1020:
        SetTimer(hwnd, 0x1003, 100, MoveEProc);
        break;
    case 1170:
        SetTimer(hwnd, 0x1004, 100, MoveEProc);
        break;
    case 1200:
        g_uTBcnt = 0;
        break;
    }
}

BOOL ShowBS(HWND hwnd, LPWSTR szPicture)
{
    HANDLE hFile = NULL;

    hFile = CreateFile(szPicture, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        ShowWindow(g_hwndBS, SW_SHOW);
        SetWindowText(g_hwndBS, TEXT("CreateFile failed."));
        return FALSE;
    }
    else
    {
        BOOL hr = 0;
        DWORD dwSizeH = 0, dwSizeL = 0;
        dwSizeL = GetFileSize(hFile, &dwSizeH);
        if (dwSizeH == 0 && dwSizeL == 0)
        {
            ShowWindow(g_hwndBS, SW_SHOW);
            SetWindowText(g_hwndBS, TEXT("GetFileSize failed."));
            CloseHandle(hFile);
            return FALSE;
        }

        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwSizeL);
        if (hGlobal == INVALID_HANDLE_VALUE)
        {
            ShowWindow(g_hwndBS, SW_SHOW);
            SetWindowText(g_hwndBS, TEXT("GlobalAlloc failed."));
            CloseHandle(hFile);
            return FALSE;
        }

        LPVOID pvData = GlobalLock(hGlobal);
        if (pvData == NULL)
        {
            ShowWindow(g_hwndBS, SW_SHOW);
            SetWindowText(g_hwndBS, TEXT("GlobalLock failed."));
            CloseHandle(hFile);
            return FALSE;
        }
        DWORD dwRead = 0;

        hr = ReadFile(hFile, pvData, dwSizeL, &dwRead, NULL);
        if (!hr)
        {
            ShowWindow(g_hwndBS, SW_SHOW);
            SetWindowText(g_hwndBS, TEXT("ReadFile failed."));
            CloseHandle(hFile);
            return FALSE;
        }

        hr = CloseHandle(hFile);
        if (!hr)
        {
            ShowWindow(g_hwndBS, SW_SHOW);
            SetWindowText(g_hwndBS, TEXT("CloseHandle failed."));
            return FALSE;
        }

        IStream *pStm = NULL;
        CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);

        IPicture *pPic = NULL;

        if (SUCCEEDED(OleLoadPicture(pStm, dwSizeL, TRUE, IID_IPicture, (LPVOID*)&pPic)))
        {
            SAFE_RELEASE(pStm);

            OLE_XSIZE_HIMETRIC mWidth = 0; 
            OLE_YSIZE_HIMETRIC mHeight = 0;

            pPic->get_Width(&mWidth); 
            pPic->get_Height(&mHeight);

            HDC hDC = GetDC(hwnd);
            int mScreenWidth = GetDeviceCaps(hDC, HORZRES);
            int mScreenHeight = GetDeviceCaps(hDC, VERTRES);

            pPic->Render(hDC, 0, 0, mScreenWidth, mScreenHeight,
                0, mHeight, mWidth, -mHeight, NULL);

            SAFE_RELEASE(pPic);

            ShowWindow(g_hwndBS, SW_HIDE);
            return TRUE;
        }
        else
        {
            ShowWindow(g_hwndBS, SW_SHOW);
            SetWindowText(g_hwndBS, TEXT("OleLoadPicture failed."));
            return FALSE;
        }
    }
}

VOID CALLBACK SetPicProc(HWND hwnd, UINT, UINT_PTR, DWORD)
{
    ShowNextPic();
}

VOID ShowNextPic()
{
    LPWSTR szDir = TEXT("I:\\");
    WCHAR szSearch[20] = {0};
    lstrcpy(szSearch, szDir);
    lstrcat(szSearch, TEXT("*.jpg"));
    WIN32_FIND_DATA fd = {0};

    WCHAR szPic[50] = {0};
    if (g_hFindFile == NULL)
    {
        g_hFindFile = FindFirstFile(szSearch, &fd);
        if (g_hFindFile == INVALID_HANDLE_VALUE)
        {
            g_hFindFile = NULL;
            goto END;
        }
    }
    else
    {
        BOOL bRet = FindNextFile(g_hFindFile, &fd);
        if (!bRet)
        {
            goto END;
        }
    }

    lstrcpy(szPic, szDir);
    lstrcat(szPic, fd.cFileName);
    SetWindowText(g_hwndE, szPic);
    g_uCase = 0x10 | ShowBS(g_hwnd, szPic) << 1;
    if (g_uCase == 0x0)
    {
        Sleep(1000);
        goto END;
    }
    return;

END:
    g_uCase = 0x0;
    g_hFindFile = NULL;
    SetTimer(g_hwnd, 1, 1000, TimerProc);
    KillTimer(g_hwnd, 2);
}

VOID CALLBACK MoveEProc(HWND hwnd, UINT, UINT_PTR id, DWORD)
{
    static int cx = GetSystemMetrics(SM_CXSCREEN);
    static int cy = GetSystemMetrics(SM_CYSCREEN);
    static int dx = 150;
    static int dy = 40;
    int pos = 0;
    int px = 0;
    int py = 0;
    switch (id)
    {
    case 0x1001:
        px = (int)((cx - dx) / 300.0 * (300 - g_uTBpos));
        py = (int)((cy - dy) / 300.0 * (g_uTBpos));
        break;
    case 0x1002:
        px = (int)((cx - dx) / 300.0 * (g_uTBpos));
        py = cy - dy;
        break;
    case 0x1003:
        px = (int)((cx - dx) / 300.0 * (300 - g_uTBpos));
        py = (int)((cy - dy) / 300.0 * (300 - g_uTBpos));
        break;
    case 0x1004:
        px = (int)((cx - dx) / 300.0 * (g_uTBpos));
        py = 0;
        break;
    }
    MoveWindow(g_hwndE, px, py, dx, dy, FALSE);
    g_uTBpos++;
    if (g_uTBpos > 300)
    {
        KillTimer(hwnd, id);
        g_uTBpos = 0;
    }
}

void SetF3()
{	
    {
        HWND hFT = FindWindow(_T("_TweakCube_NetMaster_FloatWindow"), _T(""));
        if (hFT)
        {
            SetWindowPos(hFT, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        }
    }

    ShowCursor(TRUE);
    ClipCursor(NULL);
    SendMessage(g_hwnd, WM_CLOSE, 0, 0);
	DeleteCriticalSection(&g_ce);
    UnhookWindowsHookEx(g_hHMouse);
    UnhookWindowsHookEx(g_hHKey);
}

void Set0()
{
    g_bST = !g_bST;
}

void Set1()
{
    g_bHid = FALSE;
    SetLayeredWindowAttributes(g_hwnd, 0, 0, 0);
    SetWindowLong(g_hwnd, GWL_EXSTYLE, GetWindowLong(g_hwnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
}

void Set2()
{
    SetWindowLong(g_hwnd, GWL_EXSTYLE, GetWindowLong(g_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(g_hwnd, 0, 230, LWA_ALPHA);
}
void Set3()
{
    SetWindowLong(g_hwnd, GWL_EXSTYLE, GetWindowLong(g_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(g_hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
}
void Set4()
{
    SetWindowLong(g_hwnd, GWL_EXSTYLE, GetWindowLong(g_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(g_hwnd, RGB(0, 0, 0), 128, LWA_ALPHA | LWA_COLORKEY);
}
void Set5()
{
    g_bHid = TRUE;
    SetWindowLong(g_hwnd, GWL_EXSTYLE, GetWindowLong(g_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(g_hwnd, 0, 0, LWA_ALPHA);
}

void Set6()
{
    g_crTC = RGB(0, 0, 0);
}

void Set7()
{
    g_crTC = RGB(255, 0, 0);
    SendMessage(g_hwnd, WM_COMMAND, 0x1234, 0);
}

void SetF1()
{
    if (IsWindowVisible(g_hwndE))
    {
        ShowWindow(g_hwndE, SW_HIDE);
    } 
    else
    {
        ShowWindow(g_hwndE, SW_SHOW);
    }
}

void SetF6()
{
    g_uF3cnt = 0;
    if ((g_uCase & 0x10) == 0x10)
    {
        return;
    }
    if ((g_uCase & 0x1) == 0x1)
    {
        SendMessage(g_hwnd, WM_COMMAND, 0x1234, 0);
        SetTimer(g_hwnd, 1, 1000, TimerProc);
        g_uCase = 0x0;
    }
    else
    {
        KillTimer(g_hwnd, 1);
        ShowWindow(g_hwndE, SW_HIDE);

        g_uCase = ShowBS(g_hwnd, TEXT("C:\\Windows\\Web\\Wallpaper\\Windows\\img0.jpg"));
        if (g_uCase == 0x0)
        {
            Sleep(3000);
            ShowWindow(g_hwndBS, SW_HIDE);
            ShowWindow(g_hwndE, SW_SHOW);
            SetTimer(g_hwnd, 1, 1000, TimerProc);
        }
    }
    SetWindowPos(g_hwnd, HWND_TOPMOST, 0, 0,
        GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
}

void ShowMsg( LPCTSTR msg )
{
	if (msg == NULL)
	{
		return;
	}
	EnterCriticalSection(&g_ce);
	if (g_uTimMsg <= 0)
	{
		g_uTimMsg = 8;
		g_sMsg = msg;
	}
	LeaveCriticalSection(&g_ce);
}

extern "C" void SetM()
{
    if (g_bLockMouse)
    {
        SetWindowPos(g_hwnd, HWND_BOTTOM, 0, 0,
            GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0);
        UnhookWindowsHookEx(g_hHMouse);
        g_hHMouse = NULL;
        ShowCursor(TRUE);
        ClipCursor(NULL);
        g_bST = TRUE;
    }
    else
    {
        SetWindowPos(g_hwnd, HWND_TOPMOST, 0, 0,
            GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
        ShowWindow(g_hwnd, SW_MAXIMIZE);
        SetFocus(g_hwnd);
        g_hHMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, GetModuleHandle(TEXT("HookDll")), 0);
    }
    g_bLockMouse = !g_bLockMouse;
}
