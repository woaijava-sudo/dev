// Test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "Timer.h"
#include "Map.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND hWnd = NULL;
Map* g_pMapper = NULL;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

    Timer timer(FPS);
    timer.Start();
    bool bDone = false;
	// Main message loop:
    while(!bDone)
    {
	    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	    {
            if( msg.message == WM_QUIT ) 
			{
				bDone = true;
			} 
            else
		    {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
		    }	
	    }
        if (timer.ReadyForNextFrame())
		{
            g_pMapper->Update();
#if SECONDANCE == 1
            g_pMapper->Update();
#endif
            InvalidateRect(hWnd, NULL, false);
            UpdateWindow(hWnd);
        }	
    }
    UnregisterClass( szWindowClass, hInstance );
	return 0;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TEST);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
       3, 3, 1360, 760, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
       return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	static HDC hdcBackBuffer   = NULL;
	static HBITMAP	hBitmap    = NULL;
	static HBITMAP	hOldBitmap = NULL;
	static int cxClient, cyClient = 0;
    GetClientRect(hWnd, &rect);
    cxClient = rect.right;
    cyClient = rect.bottom;

	switch (message) 
	{
        case WM_CREATE:
            g_pMapper = new Map;
            g_pMapper->Init(NUMBERS_X, NUMBERS_Y, CUBE_SIZE, INIT_DEPTH);
#if SETUP == 1
            g_pMapper->SetCellDepth(INIT_DOT_X, INIT_DOT_Y, INIT_DOT_D);
            //g_pMapper->SetCellDepth(INIT_DOT_X-80, INIT_DOT_Y, INIT_DOT_D);
            //g_pMapper->SetCellDepth(INIT_DOT_X+80, INIT_DOT_Y, INIT_DOT_D);
            //g_pMapper->SetCellDepth(INIT_DOT_X, INIT_DOT_Y-20, INIT_DOT_D);
            //g_pMapper->SetCellDepth(INIT_DOT_X, INIT_DOT_Y+20, INIT_DOT_D);
            //g_pMapper->SetCellDepth(NUMBERS_X - INIT_DOT_X, NUMBERS_Y - INIT_DOT_Y, -INIT_DOT_D);
#endif
			hdcBackBuffer = CreateCompatibleDC(NULL);
			hdc = GetDC(hWnd);
			hBitmap = CreateCompatibleBitmap(hdc, cxClient, cyClient);
			ReleaseDC(hWnd, hdc);
			hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);
            break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			BeginPaint(hWnd, &ps);
            ///////////
            BitBlt(hdcBackBuffer, 0, 0, cxClient, cyClient, NULL, NULL, NULL, WHITENESS);
            DrawText(hdcBackBuffer, szHello, strlen(szHello), &rect, DT_CENTER);
            g_pMapper->Render(hdcBackBuffer);
            BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY); 
            ///////////
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
            SelectObject(hdcBackBuffer, hOldBitmap);
			DeleteDC(hdcBackBuffer);
			DeleteObject(hBitmap); 
			PostQuitMessage(0);
			break;
        case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
            break;
        case WM_LBUTTONDOWN:
            POINT pt;
            GetCursorPos(&pt);
            g_pMapper->SetCellDepth((int)((pt.x-5)/CUBE_SIZE), (int)((pt.y-25)/CUBE_SIZE), INIT_DOT_D);
            break;
        case WM_RBUTTONDOWN:
            GetCursorPos(&pt);
            g_pMapper->SetCellDepth((int)((pt.x-5)/CUBE_SIZE), (int)((pt.y-25)/CUBE_SIZE), INIT_DOT_D * 10);
            break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}