// TestDoc.cpp : implementation of the CTestDoc class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDoc

IMPLEMENT_DYNCREATE(CTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestDoc, CDocument)
	//{{AFX_MSG_MAP(CTestDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDoc construction/destruction

CTestDoc::CTestDoc()
{
}

CTestDoc::~CTestDoc()
{
}

BOOL CTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestDoc serialization

void CTestDoc::Serialize(CArchive& ar)
{
	POSITION pos=GetFirstViewPosition();
	CTestView *pView=(CTestView*)GetNextView(pos);
	if (ar.IsStoring())
	{
	}
	else
	{
	}
	m_obArray.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CTestDoc diagnostics

#ifdef _DEBUG
void CTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestDoc commands

void CTestDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	int nCount;
	nCount=m_obArray.GetSize();
	/*for(int i=0;i<nCount;i++)
	{
		delete m_obArray.GetAt(i);
		//m_obArray.RemoveAt(i);
	}
	m_obArray.RemoveAll();*/
	while(nCount--)
	{
		delete m_obArray.GetAt(nCount);
		m_obArray.RemoveAt(nCount);
	}
	CDocument::DeleteContents();
}
