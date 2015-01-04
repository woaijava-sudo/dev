// Test.h : main header file for the TEST application
//

#if !defined(AFX_TEST_H__57521806_DB3A_4151_BDE8_071039A66278__INCLUDED_)
#define AFX_TEST_H__57521806_DB3A_4151_BDE8_071039A66278__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestApp:
// See Test.cpp for the implementation of this class
//


class CGraph: public CObject
{
	DECLARE_SERIAL(CGraph)
public:
	void Draw(CDC* pDC);
	CPoint m_ptOrigin;
	CPoint m_ptEnd;
	UINT m_nDrawType;
	CGraph();
	CGraph(UINT m_nDrawType, CPoint m_ptOrigin, CPoint m_ptEnd);
	void Serialize(CArchive& ar);
	virtual ~CGraph();

};

class CTestApp : public CWinApp
{
public:
	CTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTestApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_H__57521806_DB3A_4151_BDE8_071039A66278__INCLUDED_)
