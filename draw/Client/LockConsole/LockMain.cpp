#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
#include <commctrl.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#pragma comment(lib,"comctl32.lib")
#include <atlbase.h>
#include <atlstr.h>

#define SAFE_RELEASE(p)  \
    if ((p) != NULL)  \
                { (p)->Release(); (p) = NULL; }

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI SocketProc(LPVOID);

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    LPCWSTR ClassName = TEXT("LockMain");
    LPCWSTR WindowName = TEXT("LockMain");

    HANDLE hMutex = CreateMutex(NULL, FALSE, TEXT("LockMain MUTEX"));

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return 0;
    }

    WSADATA wsadata = {0};
    WSAStartup(MAKEWORD(2, 2), &wsadata);

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

    HWND hwnd = CreateWindow(ClassName, WindowName, WS_POPUP, 0, 0, 0, 0,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, SW_HIDE);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnregisterClass(ClassName, hInstance);
    CloseHandle(hMutex);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int HKID_Run = 0;
    static BOOL bHK_Run = FALSE;
    static int HKID_Exit = 0;
    static BOOL bHK_Exit = FALSE;
    static int HKID_Active = 0;
    static BOOL bHK_Active = FALSE;

    static HANDLE hThread = NULL;

    static GUID guid = GUID_NULL;;
    static IMMDeviceEnumerator *pEnumerator = NULL;
    static IMMDevice *pDevice = NULL;
    static IAudioEndpointVolume *pEndptVol = NULL;

    static BOOL bMute = FALSE;
    static float fVolume = 0.0f;

    switch(message)
    {
    case WM_HOTKEY:
        if ((UINT)LOWORD(lParam) == (MOD_CONTROL | MOD_ALT))
        {
            if ((UINT)HIWORD(lParam) == '1')
            {
                STARTUPINFO si = {0};
                si.cb = sizeof(STARTUPINFO);
                si.dwFlags = STARTF_USESHOWWINDOW;
                si.wShowWindow = FALSE;
                PROCESS_INFORMATION pi = {0};
                CreateProcess(TEXT("Win32Draw.exe"), NULL, NULL, NULL, FALSE,
                    0, NULL, NULL, &si, &pi);
            }
            else if ((UINT)HIWORD(lParam) == '2')
            {
                SendMessage(hwnd, WM_CLOSE, 0, 0);
            }
            else if ((UINT)HIWORD(lParam) == '9')
            {
                HWND hwnd_s = FindWindow(TEXT("Win32DrawWnd"), TEXT("Win32DrawWnd"));
                if (hwnd_s != NULL)
                {
                    SendMessage(hwnd_s, WM_COMMAND, 0x888888, 0);
                }
            }
        }
        break;
    case WM_COMMAND:
        if (wParam == 0x1234)
        {
            if (lParam == 0)
            {
                STARTUPINFO si = {0};
                si.cb = sizeof(STARTUPINFO);
                si.dwFlags = STARTF_USESHOWWINDOW;
                si.wShowWindow = FALSE;
                PROCESS_INFORMATION pi = {0};
                CreateProcess(TEXT("Win32Draw.exe"), NULL, NULL, NULL, FALSE,
                    0, NULL, NULL, &si, &pi);
            }
        }
        else if (wParam >= 0x10000 && wParam <= 0x1000A)
        {
            HWND hwnd_s = FindWindow(TEXT("Win32DrawWnd"), TEXT("Win32DrawWnd"));
            if (hwnd_s != NULL)
            {
                SendMessage(hwnd_s, WM_COMMAND, wParam, 0);
            }
        }
        else if (wParam == 0x128903)
        {
            HWND hwnd_s = FindWindow(TEXT("Win32DrawWnd"), TEXT("Win32DrawWnd"));
            if (hwnd_s != NULL)
            {
                SendMessage(hwnd_s, WM_COMMAND, 0x101772, 0);
            }
		}
		else if (wParam == 0x7778)
		{
			HWND hwnd_s = FindWindow(TEXT("Win32DrawWnd"), TEXT("Win32DrawWnd"));
			if (hwnd_s != NULL)
			{
				COPYDATASTRUCT cds;
				cds.cbData = GlobalSize((LPVOID)lParam);
				cds.lpData = (LPVOID)lParam;
				cds.dwData = 0x8765;
				SendMessage(hwnd_s, WM_COPYDATA, 0, (LPARAM) &cds);
				GlobalFree((LPVOID)lParam);
			}
		}
        else if (wParam == 0x235571)
        {
            switch (lParam)
            {
            case 0x5000:
                pEndptVol->GetMasterVolumeLevelScalar(&fVolume);
                fVolume += 0.05f;                
                if (fVolume > 1.0f)
                {
                    fVolume = 1.0f;
                }
                pEndptVol->SetMasterVolumeLevelScalar(fVolume, &guid);
                break;
            case 0x5001:
                pEndptVol->GetMasterVolumeLevelScalar(&fVolume);
                fVolume -= 0.05f;
                if (fVolume < 0.0f)
                {
                    fVolume = 0.0f;
                }
                pEndptVol->SetMasterVolumeLevelScalar(fVolume, &guid);
                break;
            case 0x10000:
                pEndptVol->GetMute(&bMute);
                bMute = !bMute;
                pEndptVol->SetMute(bMute, &guid);
                break;
            default:
                break;
            }
        }
        break;
    case WM_CREATE:
        {
            HKID_Run = GlobalAddAtom(TEXT("Lock Hot Key - Run")) - 0xC000;
            bHK_Run = RegisterHotKey(hwnd, HKID_Run, MOD_CONTROL | MOD_ALT, '1');
            HKID_Exit = GlobalAddAtom(TEXT("Lock Hot Key - Exit")) - 0xC000;
            bHK_Exit = RegisterHotKey(hwnd, HKID_Exit, MOD_CONTROL | MOD_ALT, '2');
            HKID_Active = GlobalAddAtom(TEXT("Lock Hot Key - Active")) - 0xC000;
            bHK_Active = RegisterHotKey(hwnd, HKID_Active, MOD_CONTROL | MOD_ALT, '9');

            CoInitialize(NULL);
            GUID guid = GUID_NULL;
            CoCreateGuid(&guid);
            CoCreateInstance(__uuidof(MMDeviceEnumerator),
                NULL, CLSCTX_INPROC_SERVER,
                __uuidof(IMMDeviceEnumerator),
                (void**)&pEnumerator);

            pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
            pDevice->Activate(__uuidof(IAudioEndpointVolume),
                CLSCTX_ALL, NULL, (void**)&pEndptVol);

            InitCommonControls();

            STARTUPINFO si = {0};
            si.cb = sizeof(STARTUPINFO);
            si.dwFlags = STARTF_USESHOWWINDOW;
            si.wShowWindow = FALSE;
            PROCESS_INFORMATION pi = {0};
            CreateProcess(TEXT("Win32Draw.exe"), NULL, NULL, NULL, FALSE,
                0, NULL, NULL, &si, &pi);
            hThread = CreateThread(NULL, NULL, SocketProc, (LPVOID)hwnd, NULL, NULL);         
        }
        break;
    case WM_CLOSE:
        if (hThread != NULL && hThread != INVALID_HANDLE_VALUE)
        {
            CloseHandle(hThread);
        }        
        if (bHK_Run)
        {
            UnregisterHotKey(hwnd, HKID_Run);
            GlobalDeleteAtom(HKID_Run);
        }
        if (bHK_Exit)
        {
            UnregisterHotKey(hwnd, HKID_Exit);
            GlobalDeleteAtom(HKID_Exit);
        }
        if (bHK_Active)
        {
            UnregisterHotKey(hwnd, HKID_Active);
            GlobalDeleteAtom(HKID_Active);
        }

        SAFE_RELEASE(pEnumerator);
        SAFE_RELEASE(pDevice);
        SAFE_RELEASE(pEndptVol);
        CoUninitialize();

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

DWORD WINAPI SocketProc(LPVOID lParam)
{
    HWND hwnd = (HWND)lParam;

    SOCKET sockServer = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN addrServer;
    addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(7779);

    bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

    listen(sockServer, 1);

    SOCKADDR_IN addrClient;
    int addrLen = sizeof(SOCKADDR);

    while (true)
    {
        SOCKET sockConn = accept(sockServer, (SOCKADDR*)&addrClient, &addrLen);

        if (sockConn == SOCKET_ERROR)
        {
            Sleep(100);
            continue;
        }

        while (true)
        {
            char recvBuf[255] = {0};
            int hr = recv(sockConn, recvBuf, 255, 0);
            if (hr != SOCKET_ERROR && hr != 0)
            {
				const char *m[] = {
					"Post$bajdcc_console",
					"Post message successfully!",
				};
                const char *p[] = {
                    "query host is running",
                    "The host is running!",
                    "query host start lock",
                    "The locking is start!",
                    "Host stopped locking.",
                    "Time stopped.",
                    "All lock window.",
                    "Some transparent window.",
                    "All transparent window.",
                    "All transparent and fade window.",
                    "Hide Window.",
                    "Hide text.",
                    "Show text.",
                    "Timer show.",
                    "Clip background.",
                };
                const char *q[] = {
                    "Error.",
                    "Target window is not running.",
                    "Wrong order.",
                };
                const char *v[] = {
                    "Set volume increase.",
                    "Set volume decrease.",
                    "Set volume mute or not.",
                    "Set blur enabled or not.",
                };
                
                if (strcmp(recvBuf, p[0]) == 0)
                {
                    HWND hwnd_s = FindWindow(TEXT("Win32DrawWnd"), TEXT("Win32DrawWnd"));
                    if (hwnd_s != NULL)
                    {
                        send(sockConn, p[1], strlen(p[1]), 0);
                        break;
                    }
                    else
                    {
                        send(sockConn, q[1], strlen(q[1]), 0);
                        break;
                    }
                }
                else if (strcmp(recvBuf, p[2]) == 0)
                {
                    SendMessage(hwnd, WM_COMMAND, 0x1234, 0);
                    send(sockConn, p[3], strlen(p[3]), 0);
                    break;
                }
				else if (strncmp(recvBuf, m[0], strlen(m[0])) == 0)
                {
					LPVOID p = GlobalAlloc(GMEM_ZEROINIT, strlen(recvBuf)+1);
					strncpy_s((char*)p, 255, (const char*)recvBuf, 255);
                    SendMessage(hwnd, WM_COMMAND, 0x7778, (LPARAM) p);
                    send(sockConn, m[1], strlen(m[1]), 0);
                    break;
                }
                else if (strcmp(recvBuf, v[0]) == 0)
                {
                    SendMessage(hwnd, WM_COMMAND, 0x235571, 0x5000);
                    send(sockConn, v[0], strlen(p[0]), 0);
                    break;
                }
                else if (strcmp(recvBuf, v[1]) == 0)
                {
                    SendMessage(hwnd, WM_COMMAND, 0x235571, 0x5001);
                    send(sockConn, v[1], strlen(v[1]), 0);
                    break;
                }
                else if (strcmp(recvBuf, v[2]) == 0)
                {
                    SendMessage(hwnd, WM_COMMAND, 0x235571, 0x10000);
                    send(sockConn, v[2], strlen(v[2]), 0);
                    break;
                }
                else if (strcmp(recvBuf, v[3]) == 0)
                {
                    SendMessage(hwnd, WM_COMMAND, 0x128903, 0);
                    send(sockConn, v[3], strlen(v[3]), 0);
                    break;
                }
                else
                {
                    HWND hwnd_s = FindWindow(TEXT("Win32DrawWnd"), TEXT("Win32DrawWnd"));
                    if (hwnd_s != NULL)
                    {
                        bool bRt = false;
                        for (int i = 0; i <= 10; i++)
                        {
                            if (strcmp(recvBuf, p[i+4]) == 0)
                            {
                                SendMessage(hwnd, WM_COMMAND, 0x10000+i, 0);
                                send(sockConn, p[i+4], strlen(p[i+4]), 0);
                                bRt = true;
                                break;
                            }
                        }
                        if (!bRt)
                        {
                            send(sockConn, q[2], strlen(q[2]), 0);
                            break;
                        }
                    }
                    else
                    {
                        send(sockConn, q[1], strlen(q[1]), 0);
                        break;
                    }
                }
            }
            else
            {
                break;
            }
        }

        closesocket(sockConn);
    }

    WSACleanup();

    return 0;
}