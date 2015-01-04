// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 HOOKDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// HOOKDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
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