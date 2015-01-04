#pragma once
#include "afx.h"
#include <afxdialogex.h>

#define WM_GUI_CMD      ( WM_USER + 0x10 )

extern "C" AFX_EXT_CLASS void globalSetDialog(CDialogEx* pDlg);
extern "C" AFX_EXT_CLASS void globalStartupGdiPlus();
extern "C" AFX_EXT_CLASS void globalShutdownGdiPlus();
extern "C" AFX_EXT_CLASS void globalDialogInit();
extern "C" AFX_EXT_CLASS void globalDialogOnSize(UINT nType, int cx, int cy);
extern "C" AFX_EXT_CLASS void globalDialogOnPaint(HDC hDC, RECT rect);
extern "C" AFX_EXT_CLASS void globalDialogOnLButtonDown(UINT nFlags, POINT point);
extern "C" AFX_EXT_CLASS void globalDialogOnLButtonUp(UINT nFlags, POINT point);
extern "C" AFX_EXT_CLASS bool globalDialogOnSetCursor(UINT nHitTest, UINT message);
extern "C" AFX_EXT_CLASS void globalDialogOnMouseMove(UINT nFlags, POINT point);
extern "C" AFX_EXT_CLASS void globalDialogOnMouseLeave();
extern "C" AFX_EXT_CLASS void globalDialogOnMouseHover(UINT nFlags, POINT point);
extern "C" AFX_EXT_CLASS void globalDialogOnDestroy();
extern "C" AFX_EXT_CLASS LRESULT globalDialogOnCommand(WPARAM wParam, LPARAM lParam);
extern "C" AFX_EXT_CLASS LRESULT globalDialogOnNcHitTest(LRESULT nHitTest, POINT point);
extern "C" AFX_EXT_CLASS void globalDialogOnMove(int x, int y);
extern "C" AFX_EXT_CLASS void globalDialogOnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
extern "C" AFX_EXT_CLASS void globalDialogOnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
extern "C" AFX_EXT_CLASS void globalDialogOnTimer(UINT_PTR nIDEvent);

