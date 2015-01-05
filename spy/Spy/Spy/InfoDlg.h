#pragma once
#include "afxwin.h"


// CInfoDlg �Ի���

class CInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInfoDlg();

// �Ի�������
	enum { IDD = IDD_INFO };

protected:
    BOOL is_scanning; // �Ƿ�����ɨ��
    BOOL is_found; // �Ƿ��Ѿ��ҵ�
    HICON icon_from; // origin icon
    HICON icon_blank; // blank icon
    HCURSOR cursor_scan; // ���
    HWND saved_win_handle; // �����ɨ����
    HWND parent_handle; // ������Spy���
    HWND prev_window; // ɨ�����һ������

// ɨ�躯��
    void SpyStartScanning(); // beginning of scan
    void SpyStopScanning(); // end of scan
    void SpyExecScanning(CPoint &pt); // process of scan
    HWND SpyFindSmallestWindow(const CPoint &pt); // find the smallest window
    void SpyInvertBorder(const HWND &hWnd); // invert the window border

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CEdit edit_pos;
    CStatic scan;
    afx_msg void OnClose();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
