// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� HOOKDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// HOOKDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef HOOKDLL_EXPORTS
#define HOOKDLL_API __declspec(dllexport)
#else
#define HOOKDLL_API __declspec(dllimport)
#endif

#pragma data_seg("DllSec")
HWND g_hwnd = NULL;
#pragma data_seg()

LRESULT CALLBACK KeyboardProc(int, WPARAM ,LPARAM);
LRESULT CALLBACK LLKeyboardProc(int, WPARAM ,LPARAM);
LRESULT CALLBACK MouseProc(int, WPARAM ,LPARAM);
VOID CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD);
VOID CALLBACK SetPicProc(HWND, UINT, UINT_PTR, DWORD);
VOID CALLBACK MoveEProc(HWND, UINT, UINT_PTR, DWORD);
VOID CALLBACK MsgProc(HWND, UINT, UINT_PTR, DWORD);
BOOL ShowBS(HWND, LPWSTR);
VOID ShowNextPic();

HHOOK g_hHKey = NULL;
HHOOK g_hHMouse = NULL;
HHOOK g_hHKeyLL = NULL;
int g_uTime = 0;
int g_uF3cnt = 0;
HWND g_hwndE = NULL;
HWND g_hwndBS = NULL;
UINT g_uCase = 0x0;
HANDLE g_hFindFile = NULL;
COLORREF g_crTC = RGB(255, 0, 0);
BOOL g_bHid = FALSE;
bool g_bST = FALSE;
UINT g_uTBcnt = 0;
UINT g_uTBpos = 0;
int g_uTimMsg = 0;
ATL::CString g_sMsg;
CRITICAL_SECTION g_ce;
bool g_bLockMouse = true;

extern "C" void SetF3();
extern "C" void Set0();
extern "C" void Set1();
extern "C" void Set2();
extern "C" void Set3();
extern "C" void Set4();
extern "C" void Set5();
extern "C" void Set6();
extern "C" void Set7();
extern "C" void SetF1();
extern "C" void SetF6();
extern "C" void SetM();
extern "C" void SetHook(HWND hwnd);
extern "C" void ShowMsg(LPCTSTR msg);

#define SAFE_RELEASE(p) {if (p){(p)->Release();(p) = NULL;} }