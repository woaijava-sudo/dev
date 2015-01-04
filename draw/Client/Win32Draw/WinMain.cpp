#include <Windows.h>
#include <assert.h>
#include <dwmapi.h>
#include <atlbase.h>
#include <atlstr.h>
#pragma comment(lib, "dwmapi.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI DrawProc(LPVOID);
DWORD WINAPI DispatchMsgTxt(LPVOID lParam);
HMODULE g_hDll = NULL;
CStringA* g_pStrm = NULL;
CRITICAL_SECTION g_ce;
typedef void (*lpfnShowMsg)(LPCTSTR);
lpfnShowMsg ShowMsg = NULL;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    LPCWSTR ClassName = TEXT("Win32DrawWnd");
    LPCWSTR WindowName = TEXT("Win32DrawWnd");

    HANDLE hMutex = CreateMutex(NULL, FALSE, TEXT("Win32DrawWnd MUTEX"));

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return 0;
    }

    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = ClassName;

    RegisterClass(&wndclass);

    HWND hwnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, ClassName, WindowName, WS_POPUP, 0, 0,
        GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    FreeLibrary(g_hDll);
    UnregisterClass(ClassName, hInstance);

    CloseHandle(hMutex);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HANDLE hThread;
    typedef void (*lpfnSetHook)(HWND);
    static lpfnSetHook SetHook = NULL;
    const RECT rt = {0, 0, 0, 0};
    typedef void (*lpfnSet)();
    static lpfnSet vSet[11]= {NULL};
    static const char *pVfn[] = {
        "SetF3",
        "Set0",
        "Set1",
        "Set2",
        "Set3",
        "Set4",
        "Set5",
        "Set6",
        "Set7",
        "SetF1",
        "SetF6",
    };

    static DWM_BLURBEHIND bb = {
        DWM_BB_ENABLE | DWM_BB_BLURREGION | DWM_BB_TRANSITIONONMAXIMIZED,
        TRUE,
        NULL,
        FALSE
    };

    switch(message)
    {
    case WM_CREATE:
		g_pStrm = new CStringA();
		InitializeCriticalSection(&g_ce);
        ShowCursor(FALSE);
        SetCursorPos(0, 0);
        hThread = CreateThread(NULL, NULL, DrawProc, (LPVOID)hwnd, NULL, NULL);
        Sleep(100);
        g_hDll = LoadLibrary(TEXT("HookDll.dll"));
        assert(g_hDll);
        SetHook = (lpfnSetHook)GetProcAddress(g_hDll, "SetHook");
        assert(SetHook);
        for (int i = 0; i <= 10; i++)
        {
            vSet[i] = (lpfnSet)GetProcAddress(g_hDll, pVfn[i]);
            assert(vSet[i]);
        }
		ShowMsg = (lpfnShowMsg)GetProcAddress(g_hDll, "ShowMsg");
		assert(ShowMsg);
        SetHook(hwnd);
        {
            BOOL isEnableDWM = FALSE;
            DwmIsCompositionEnabled(&isEnableDWM);
            if (isEnableDWM)
            {
                DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
                DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
                DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &ncrp, sizeof(ncrp));
            }
        }
        break;
    case WM_DESTROY:
        {
            BOOL isEnableDWM = FALSE;
            DwmIsCompositionEnabled(&isEnableDWM);
            if (isEnableDWM)
            {
                DWMNCRENDERINGPOLICY ncrp = DWMNCRP_DISABLED;
                DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
                DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &ncrp, sizeof(ncrp));
            }
        }
		delete g_pStrm;
		DeleteCriticalSection(&g_ce);
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        if (wParam == 0x1234)
        {
            hThread = CreateThread(NULL, NULL, DrawProc, (LPVOID)hwnd, NULL, NULL);
        }
        else if (wParam >= 0x10000 && wParam <= 0x1000A)
        {
            SetWindowPos(hwnd, HWND_TOPMOST, 0, 0,
                GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

            vSet[wParam - 0x10000]();
        }
        else if (wParam == 0x101772)
        {
            DwmEnableBlurBehindWindow(hwnd, &bb);
            bb.fEnable = !bb.fEnable;
        }
        else if (wParam == 0x888888)
        {
            lpfnSet SetM = (lpfnSet)GetProcAddress(g_hDll, "SetM");
            SetM();
        }
        break;
	case WM_COPYDATA:
        {
			PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;
			if (pcds->dwData == 0x8765)
			{
				int size = pcds->cbData;
				size = (size > 255) ? 255 : size;
				char *p = new char[size+1];
				strncpy_s(p, size, (const char*)pcds->lpData, size);
				CStringA str(p);
				EnterCriticalSection(&g_ce);
				*g_pStrm = str;
				LeaveCriticalSection(&g_ce);
				CreateThread(NULL, NULL, DispatchMsgTxt, NULL, NULL, NULL);
			}
        }
		break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

#define IsHexNum(c) ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
CString Utf8ToStringT(LPSTR str)
{
    _ASSERT(str);
    USES_CONVERSION;
    WCHAR *buf;
    int length = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
    buf = new WCHAR[length+1];
    ZeroMemory(buf, (length+1) * sizeof(WCHAR));
    MultiByteToWideChar(CP_UTF8, 0, str, -1, buf, length);
    return (CString(W2T(buf)));
}

DWORD WINAPI DispatchMsgTxt(LPVOID /*lParam*/)
{
	CString utf8 = Utf8ToStringT(g_pStrm->GetBuffer(0));
	int n = utf8.Find(_T("#"));
	if (n < 0) return 1;
	CString re = utf8.Mid(n+1);
	LPCTSTR p = re.GetBuffer(0);
	EnterCriticalSection(&g_ce);
	ShowMsg(p);
	LeaveCriticalSection(&g_ce);
	return 0;
}

DWORD WINAPI DrawProc(LPVOID lParam)
{
    HWND hwnd = (HWND)lParam;

    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0,
        GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

    HDC hDC = GetDC(hwnd);

    RECT rect;
    GetClientRect(hwnd, &rect);

    int mWidth = GetDeviceCaps(hDC, HORZRES);
    int mHeight = GetDeviceCaps(hDC, VERTRES);

    HDC hBufDC = CreateCompatibleDC(hDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, mWidth, mHeight);

    SetBkMode(hBufDC, OPAQUE);
    SetBkColor(hBufDC, RGB(0, 0, 0));

    SelectObject(hBufDC, hBitmap);

    HFONT hFont = CreateFont(60, 30, 0, 0, 16, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("楷体"));

    HFONT hSubFont = CreateFont(20, 12, 0, 0, 16, TRUE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("宋体"));

    BitBlt(hBufDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
        NULL, 0, 0, BLACKNESS);

    //////////////////////////////////////////////////////////////////////////

    LPCWSTR szMessage = TEXT("此系统已被锁定");
    LPCWSTR szSubMessage = TEXT("---- Hooked by Bajdcc");
    RECT rtText = {rect.left, rect.bottom / 2 - 50, rect.right, rect.bottom / 2 + 50};
    RECT rtSubText = {rect.left, rect.bottom - 20, rect.right, rect.bottom};

    SetTextColor(hBufDC, RGB(255, 0, 0));
    SelectObject(hBufDC, hFont);
    DrawText(hBufDC, szMessage, lstrlen(szMessage), &rtText, DT_CENTER);

    SetTextColor(hBufDC, RGB(255, 255, 0));
    SelectObject(hBufDC, hSubFont);
    DrawText(hBufDC, szSubMessage, lstrlen(szSubMessage), &rtSubText, DT_RIGHT);

    //////////////////////////////////////////////////////////////////////////

    BitBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
        hBufDC, 0, 0, SRCCOPY);

    DeleteObject(hFont);
    DeleteObject(hSubFont);
    DeleteObject(hBitmap);
    DeleteDC(hBufDC);
    ReleaseDC(hwnd, hBufDC);

    return 0;
}