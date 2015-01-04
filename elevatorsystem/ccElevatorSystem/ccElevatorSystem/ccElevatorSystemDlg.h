
// ccElevatorSystemDlg.h : ͷ�ļ�
//

#pragma once
#include "..\..\ccGui\api.h"


// CElevatorSystemDlg �Ի���
class CElevatorSystemDlg : public CDialogEx
{
// ����
public:
	CElevatorSystemDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CCELEVATORSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg LRESULT OnGuiCommand(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg void OnMove(int x, int y);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
