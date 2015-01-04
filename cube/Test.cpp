// Test.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Test.h"

#include "MainFrm.h"
#include "TestDoc.h"
#include "TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestApp

BEGIN_MESSAGE_MAP(CTestApp, CWinApp)
	//{{AFX_MSG_MAP(CTestApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestApp construction

CTestApp::CTestApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTestApp object

CTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTestApp initialization

BOOL CTestApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register document templates

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTestDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CTestView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTestApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTestApp message handlers

IMPLEMENT_SERIAL(CGraph, CObject, 1)

CGraph::CGraph() {}

CGraph::~CGraph() {}

CGraph::CGraph(UINT m_nDrawType,CPoint m_ptOrigin,CPoint m_ptEnd)
{
	this->m_nDrawType = m_nDrawType;
	this->m_ptOrigin = m_ptOrigin;
	this->m_ptEnd = m_ptEnd;
}


void CGraph::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar<<m_nDrawType<<m_ptOrigin<<m_ptEnd;
	}
	else
	{
		ar>>m_nDrawType>>m_ptOrigin>>m_ptEnd;
	}
}

void CGraph::Draw(CDC *pDC)
{
	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *pOldBrush = pDC->SelectObject(pBrush);
	switch(m_nDrawType)
	{
	case 1:
		pDC->SetPixel(m_ptEnd, RGB(0,0,0));
		break;
	case 2:
		pDC->MoveTo(m_ptOrigin);
		pDC->LineTo(m_ptEnd);
		break;
	case 3:
		pDC->Rectangle(CRect(m_ptOrigin, m_ptEnd));
		break;
	case 4:
		pDC->Ellipse(CRect(m_ptOrigin, m_ptEnd));
		break;
	}
	pDC->SelectObject(pOldBrush);
}
