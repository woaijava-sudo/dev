#include "stdafx.h"
#include "UI.h"

using namespace Gdiplus;

UIManager& globalGetManagerInstance(void)
{
    static UIManager layout;
    return layout;
}

ULONG_PTR gdiplus_token;

extern "C" AFX_EXT_CLASS void globalSetDialog(CDialogEx* pDlg)
{
    ASSERT(pDlg);
    globalGetManagerInstance().setDialog(pDlg);
}

extern "C" AFX_EXT_CLASS void globalStartupGdiPlus()
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplus_token, &gdiplusStartupInput, NULL);
}

extern "C" AFX_EXT_CLASS void globalShutdownGdiPlus()
{
    GdiplusShutdown(gdiplus_token);
}

extern "C" AFX_EXT_CLASS void globalDialogInit()
{
    globalGetManagerInstance().init();
}

extern "C" AFX_EXT_CLASS void globalDialogOnSize(UINT nType, int cx, int cy)
{
    globalGetManagerInstance().onSize(nType, cx, cy);
}

extern "C" AFX_EXT_CLASS void globalDialogOnPaint(HDC hDC, RECT rect)
{
    globalGetManagerInstance().onPaint(hDC, rect);
}

extern "C" AFX_EXT_CLASS void globalDialogOnLButtonDown(UINT nFlags, POINT point)
{
    globalGetManagerInstance().onLButtonDown(nFlags, point);
}

extern "C" AFX_EXT_CLASS void globalDialogOnLButtonUp(UINT nFlags, POINT point)
{
    globalGetManagerInstance().onLButtonUp(nFlags, point);
}

extern "C" AFX_EXT_CLASS bool globalDialogOnSetCursor(UINT nHitTest, UINT message)
{
    return globalGetManagerInstance().onSetCursor(nHitTest, message);
}

extern "C" AFX_EXT_CLASS void globalDialogOnMouseMove(UINT nFlags, POINT point)
{
    globalGetManagerInstance().onMouseMove(nFlags, point);
}

extern "C" AFX_EXT_CLASS void globalDialogOnMouseLeave()
{
    globalGetManagerInstance().onMouseLeave();
}

extern "C" AFX_EXT_CLASS void globalDialogOnMouseHover(UINT nFlags, POINT point)
{
    globalGetManagerInstance().onMouseHover(nFlags, point);
}

extern "C" AFX_EXT_CLASS void globalDialogOnDestroy()
{
    globalGetManagerInstance().onDestroy();
}

extern "C" AFX_EXT_CLASS LRESULT globalDialogOnCommand(WPARAM wParam, LPARAM lParam)
{
    return globalGetManagerInstance().onCommand(wParam, lParam);
}

extern "C" AFX_EXT_CLASS LRESULT globalDialogOnNcHitTest(LRESULT nHitTest, POINT point)
{
    return globalGetManagerInstance().onNcHitTest(nHitTest, point);
}

extern "C" AFX_EXT_CLASS void globalDialogOnMove(int x, int y)
{
    return globalGetManagerInstance().onMove(x, y);
}

extern "C" AFX_EXT_CLASS void globalDialogOnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    return globalGetManagerInstance().onKeyDown(nChar, nRepCnt, nFlags);
}

extern "C" AFX_EXT_CLASS void globalDialogOnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    return globalGetManagerInstance().onKeyUp(nChar, nRepCnt, nFlags);
}

extern "C" AFX_EXT_CLASS void globalDialogOnTimer(UINT_PTR nIDEvent)
{
    return globalGetManagerInstance().onTimer(nIDEvent);
}
