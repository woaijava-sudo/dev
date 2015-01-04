// TestView.cpp : implementation of the CTestView class
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
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	//{{AFX_MSG_MAP(CTestView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestView construction/destruction

CTestView::CTestView()
{
}

CTestView::~CTestView()
{
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView drawing

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView printing

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestView message handlers

void CTestView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CGraph *pGraph=new CGraph(2, m_ptOrigin, point);
	CClientDC dc(this);
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.SelectObject(pBrush);
	dc.MoveTo(m_ptOrigin);
	dc.LineTo(point);
	/*switch(m_nDrawType)
	{
	case 1:
		dc.SetPixel(point,RGB(0,0,0));
		break;
	case 2:
		
		break;
	case 3:
		dc.Rectangle(CRect(m_ptOrigin,point));
		break;
	case 4:
		dc.Ellipse(CRect(m_ptOrigin,point));
		break;
	}*/

	//m_obArray.Add(pGraph);
	CTestDoc *pDoc = GetDocument();
	pDoc->m_obArray.Add(pGraph);

	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ptOrigin=point;
	CView::OnLButtonUp(nFlags, point);
}

void CTestView::AAA() 
{
	// TODO: Add your message handler code here and/or call default
	CPoint aa,bb;
	aa.Offset(33,33);
	bb.Offset(200,200);
	CGraph *pGraph=new CGraph(2, aa, bb);
	CClientDC dc(this);
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.SelectObject(pBrush);
	dc.MoveTo(m_ptOrigin);
	dc.LineTo(bb);
	/*switch(m_nDrawType)
	{
	case 1:
		dc.SetPixel(point,RGB(0,0,0));
		break;
	case 2:
		
		break;
	case 3:
		dc.Rectangle(CRect(m_ptOrigin,point));
		break;
	case 4:
		dc.Ellipse(CRect(m_ptOrigin,point));
		break;
	}*/

	//m_obArray.Add(pGraph);
	CTestDoc *pDoc = GetDocument();
	pDoc->m_obArray.Add(pGraph);

}