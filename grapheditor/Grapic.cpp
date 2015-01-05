// Grapic.cpp : implementation file
//

#include "stdafx.h"
#include "Ex_Draw.h"
#include "Grapic.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ѡ�����
#define SELECT_MISS_LINE 1.005
#define SELECT_MISS_ELLIPSE 0.2f
#define DRAG_INF_WIDTH 8

GraphicMember::GraphicMember(): m_DrawType(SELECT), m_clrPen(RGB(0, 0, 0)), m_clrBrush(RGB(128, 128, 128)),
m_iPenStyle(PS_SOLID), m_iBrushStyle(BS_SOLID), m_iPenWidth(1), m_bHidden(FALSE) {}

/////////////////////////////////////////////////////////////////////////////
// CGrapic

/*
    �����ͼ����Ĳ��裺

  1. ����ID
  2. �̳���CGraphic
  3. ��CreateGraphic�����
  4. ��дDraw����
  5. ��д���л�����(*)
  6. ��Ӱ�ť
  7. ��д����¼�(*)

  */

CGraphic::CGraphic(UINT type /*= NONE*/): m_DrawType(type)
{
    m_bHidden = FALSE;
    m_createTime = CTime::GetCurrentTime();
    m_modifiedTime = CTime::GetCurrentTime();
}

void CGraphic::Draw( CDC* pDC )
{

}

IMPLEMENT_SERIAL( CGraphic, CObject, 1 )
void CGraphic::Serialize( CArchive &ar )
{
    if ( ar.IsStoring() )
    {
        ar << m_DrawType << m_bHidden << m_lpszName << m_pt1 << m_pt2 << m_createTime << m_modifiedTime;
    }
    else
    {
        ar >> m_DrawType >> m_bHidden >> m_lpszName >> m_pt1 >> m_pt2 >> m_createTime >> m_modifiedTime;
	}
}

LPCTSTR CGraphic::GetName() const
{
    return _T("ͼ�λ���");
}

void CGraphic::DrawSelectedEdge( CDC* pDC )
{
    
}

void CGraphic::HitTest( CPoint& pt, BOOL& bResult )
{
    
}

void CGraphic::LineHitTest( CPoint& a, CPoint& b, CPoint& c, BOOL& bResult )
{
    static double A, B, C;
    A = DotsLengthSquare(b, c);
    B = DotsLengthSquare(a, c);
    C = DotsLengthSquare(a, b);
    double z = ( 1.0 * (A + B + 2 * sqrt(A * B)) / C);
    bResult = ( z >= 0.99 && z < SELECT_MISS_LINE ) ? TRUE : FALSE;
}

CGraphic* CGraphic::CreateGraphic( GraphicMember* pSrc )
{
    ASSERT(pSrc);
    CGraphic* pRet = NULL;
    switch (pSrc->m_DrawType)
    {
    case LINE:          pRet = new CLine;       break;
    case RECTANGLE:     pRet = new CRectangle;  break;
    case ELLIPSE:       pRet = new CEllipse;    break;
    case CURVE:         pRet = new CCurve;      break;
    default: return NULL;
    }
    pRet->UpdateData(pSrc);
    return pRet;
}

void CGraphic::GraphicDrawSelectedEdge( CDC* pDC, CPoint& pt, int& inflate )
{
    static CRect rt;
    rt.SetRect(pt, pt);
    rt.InflateRect(inflate, inflate);
    pDC->Ellipse(&rt);
}

LONG CGraphic::DotsLengthSquare( CPoint& p1, CPoint& p2 )
{
    return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
}

int CGraphic::GetIdBySelection( _GBS SelectType, int ID )
{
    switch (SelectType)
    {
    case GBS_PEN: 
        {
            switch (ID)
            {
            case 0:     return PS_SOLID;
            case 1:     return NULL_OBJECT_BASE + PS_NULL;
            case 2:     return PS_DASH;
            case 3:     return PS_DOT;
            case 4:     return PS_DASHDOT;
            case 5:     return PS_DASHDOTDOT;
            }
            break;
            
        } 
    case GBS_BRUSH:
        {
            switch (ID)
            {
            case 0:     return BS_SOLID;
            case 1:     return BS_NULL;
            case 2:     return HATCH_BRUSH_BASE + HS_HORIZONTAL;
            case 3:     return HATCH_BRUSH_BASE + HS_VERTICAL;
            case 4:     return HATCH_BRUSH_BASE + HS_FDIAGONAL;
            case 5:     return HATCH_BRUSH_BASE + HS_BDIAGONAL;
            case 6:     return HATCH_BRUSH_BASE + HS_CROSS;
            case 7:     return HATCH_BRUSH_BASE + HS_DIAGCROSS;
            }
            break;
        }
    }
    return -1;
}

int CGraphic::GetSelectionById( _GBS SelectType, int sel )
{
    switch (SelectType)
    {
    case GBS_PEN: 
        {
            switch (sel)
            {
            case PS_SOLID:          return 0;
            case NULL_OBJECT_BASE + PS_NULL: return 1;
            case PS_DASH:           return 2;
            case PS_DOT:            return 3;
            case PS_DASHDOT:        return 4;
            case PS_DASHDOTDOT:     return 5;
            }
            break;
        }
    case GBS_BRUSH:
        {
            switch (sel)
            {
            case BS_SOLID:  return 0;
            case BS_NULL:   return 1;
            case HATCH_BRUSH_BASE + HS_HORIZONTAL:  return 2;
            case HATCH_BRUSH_BASE + HS_VERTICAL:    return 3;
            case HATCH_BRUSH_BASE + HS_FDIAGONAL:   return 4;
            case HATCH_BRUSH_BASE + HS_BDIAGONAL:   return 5;
            case HATCH_BRUSH_BASE + HS_CROSS:       return 6;
            case HATCH_BRUSH_BASE + HS_DIAGCROSS:   return 7;
            }
            break;
        }
    }
    return -1;
}

LPCTSTR CGraphic::GetPenStyleById( int ID, BOOL bConvert /*= TRUE*/ )
{
    int id = ID;
    if (bConvert)
    {
        id = GetIdBySelection(GBS_PEN, ID);
    }
    if (IF_NULL_OBJECT(id))
    {
        id &= ~NULL_OBJECT_BASE;
        if (id == PS_NULL)      return _T("͸��");
    }
    else
    {
        switch (id)
        {
        case PS_SOLID:          return _T("ʵ��");
        case PS_DASH:           return _T("���� (-------)");
        case PS_DOT:            return _T("��   (.......)");
        case PS_DASHDOT:        return _T("���1(_._._._)");
        case PS_DASHDOTDOT:     return _T("���2(_.._.._)");
        }
    }
    ASSERT(!"PS> ID falut!"); return NULL;
}

LPCTSTR CGraphic::GetBrushStyleById( int ID, BOOL bConvert /*= TRUE*/ )
{
    int id = ID;
    if (bConvert)
    {
        id = GetIdBySelection(GBS_BRUSH, ID);
    }
    if (!IF_HATCH_BRUSH(id))
    {
        if (id == BS_NULL)      return _T("͸��");
        if (id == BS_SOLID)     return _T("ʵ��");
    }
    else
    {
        id &= ~HATCH_BRUSH_BASE;
        switch (id)
        {
        case HS_HORIZONTAL:     return _T("ˮƽ�� (-------)");
        case HS_VERTICAL:       return _T("��ֱ�� (|||||||)");
        case HS_FDIAGONAL:      return _T("��б�� (\\\\\\\\\\\\\\)");
        case HS_BDIAGONAL:      return _T("��б�� (///////)");
        case HS_CROSS:          return _T("������ (+++++++)");
        case HS_DIAGCROSS:      return _T("б���� (xxxxxxx)");
        }
    }
    ASSERT(!"BS> ID falut!"); return NULL;
}

BOOL CGraphic::PtInRectTest( CPoint& pt )
{
    CRect rt;
    rt.SetRect(m_pt1, m_pt2);
    rt.NormalizeRect();
    return rt.PtInRect(pt);
}

void CGraphic::GetRect( CRect& rt )
{

}

int CGraphic::GetPts() const
{
    return 2;
}

BOOL CGraphic::EnableBrush() const
{
    return TRUE;
}

void CGraphic::UpdateData( GraphicMember* pSrc, BOOL bSave /*= TRUE */ )
{
    ASSERT(pSrc);
    if (bSave)
    {
        m_DrawType = pSrc->m_DrawType;
        m_pt1 = pSrc->m_pt1;
        m_pt2 = pSrc->m_pt2;
        m_modifiedTime = pSrc->m_modifiedTime;
        m_bHidden = pSrc->m_bHidden;
        m_lpszName = pSrc->m_lpszName;
    }
    else
    {
        pSrc->m_DrawType = m_DrawType;
        pSrc->m_pt1 = m_pt1;
        pSrc->m_pt2 = m_pt2;
        pSrc->m_modifiedTime = m_modifiedTime;
        pSrc->m_createTime = m_createTime;
        pSrc->m_bHidden = m_bHidden;
        pSrc->m_lpszName = m_lpszName;
    }
}

void CGraphic::CreateGdiObjectFromId( _GBS GdiType, int ID, CGdiObject* object, int width, int color )
{
    ASSERT_VALID(object);
    int id = ID;
    switch (GdiType)
    {
    case GBS_PEN:
        ASSERT(object->IsKindOf(RUNTIME_CLASS(CPen)));
        if (IF_NULL_OBJECT(id))
        {
            id &= ~NULL_OBJECT_BASE;
            if (id == PS_NULL)      { static_cast<CPen*>(object)->Attach(GetStockObject(NULL_PEN)); return; }
        }
        else
        {
            switch (id)
            {
            case PS_SOLID:          
            case PS_DASH:
            case PS_DOT:
            case PS_DASHDOT:
            case PS_DASHDOTDOT:
                static_cast<CPen*>(object)->CreatePen(id, width, color); return;
            }
        } 
        break;
    case GBS_BRUSH:
        ASSERT(object->IsKindOf(RUNTIME_CLASS(CBrush)));
        if (!IF_HATCH_BRUSH(id))
        {
            if (id == BS_NULL)      { static_cast<CBrush*>(object)->Attach(GetStockObject(NULL_BRUSH)); return; }
            if (id == BS_SOLID)     { static_cast<CBrush*>(object)->CreateSolidBrush(color); return; }
        }
        else
        {
            id &= ~HATCH_BRUSH_BASE;
            switch (id)
            {
            case HS_HORIZONTAL:
            case HS_VERTICAL:
            case HS_FDIAGONAL:
            case HS_BDIAGONAL:
            case HS_CROSS:
            case HS_DIAGCROSS:
                static_cast<CBrush*>(object)->CreateHatchBrush(id, color); return;
            }
        }
        break;
    }
    ASSERT(!"CreateGdiObjectFromId Error");
}

LPCTSTR CGraphic::HitSizingTest( CPoint& pt, BOOL& bResult, LONG** PtX /*= NULL*/, LONG** PtY /*= NULL */ )
{
    return NULL;
}

void CGraphic::GraphicHitSizingTest( LONG& x, LONG& y, int inf, CPoint& pt, BOOL& bResult,
    LONG** X /*= NULL*/, LONG** Y /*= NULL*/ )
{
    static CRect rt;
    rt.SetRect(x, y, x, y);
    rt.InflateRect(inf, inf);
    if (rt.PtInRect(pt))
    {
        if (X) *X = &x;
        if (Y) *Y = &y;
        bResult = TRUE;
    }
}


CGraphicLog::CGraphicLog( CObArray* arr ): m_dones(0), m_undos(0)
{
    ASSERT_VALID(arr);
    m_parr = arr;
}

CGraphicLog::~CGraphicLog()
{
    Clear();
}

CString CGraphicLog::GraphicOperation::Trace()
{
    CString str;
    LPCTSTR OP;
    switch (oper)
    {
    case GOS_ADD: OP = _T("ADD"); break;
    case GOS_DELETE: OP = _T("DELETE"); break;
    case GOS_UPDATE: OP = _T("UPDATE"); break;
    default: OP = _T("UNKNOWN");
    }
    str.Format(_T("[%s] - [%p] - [%s] - [%s] - [%d]"), OP, pGraphic, pGraphic->GetName(), pGraphic->m_lpszName, index);
    return str;
}

BOOL CGraphicLog::CanUndo() const
{
    // ���Գ�����
    return ( m_undos == 0 ) ? FALSE : TRUE;
}

BOOL CGraphicLog::CanDo() const
{
    // ���Իָ���
    return ( m_dones == 0 ) ? FALSE : TRUE;
}

void CGraphicLog::Undo()
{
    // * * * ������޸����ü����Ͳ����������� * * *

    if (m_undos == 0)
    {
        return;
    }
    TRACE("LOG UNDO ------\n");
    m_undos--;
    _GO go = m_listUndo.GetHead();
    CGraphic* pOb = NULL;
    switch (go.oper)
    {
    case GOS_ADD:
        // ������ӵģ�����Ϊɾ������
        // �����ͼ���������Ƴ������ü�����һ
        m_parr->RemoveAt(go.index);

        // �����б��б�������¼ɾ����������ɾ���������ü�����һ
        // ��ʱҪ����ָ�������Ҫ�ָ��������
        // ������listDone��Ҫ������Ӳ��������ü�����һ        
        // ��֮���ü�����һ
        Delete(go.pGraphic);
        DoneOper(GOS_ADD, go.pGraphic, go.index);
        break;
    case GOS_DELETE:
        // ����ɾ���ģ�����Ϊ��Ӳ���
        // �����ƶ���ͼ����������Ӧλ�ã����ü�������
        m_parr->InsertAt(go.index, go.pGraphic);

        // ����֮ǰ�Ķ���Ҫ���棨�ƶ������ָ��б��У����ü�������
        // ����ָ���ԭʼ���飬���ü�����һ
        AddRef(go.pGraphic);
        DoneOper(GOS_DELETE, go.pGraphic, go.index);
        break;
    case GOS_UPDATE:
        // �������ģ��������ж���Ҫ�ָ��ɳ���֮ǰ��
        pOb = Convert_To_Graphic(m_parr->GetAt(go.index));
        m_parr->ElementAt(go.index) = go.pGraphic;

        // ����ԭ���󱻱��棨�ƶ������ָ��б����ü�������
        // �¶���ӳ���������¼�б����ƶ����������飬���ü�������
        // ��֮���ü�������
        DoneOper(GOS_UPDATE, pOb, go.index);
        break;
    default: ASSERT(!"operation fault!");
    }
    m_listUndo.RemoveHead();
}

void CGraphicLog::DoneOper( GOS oper, CGraphic* p, int index )
{
    // �����ﲻ�޸����ü���

    if (m_dones == MAX_SAVE)
    {
        ASSERT(!m_listDone.IsEmpty());
        Delete(m_listDone.GetTail().pGraphic);
        m_listDone.RemoveTail();
    }
    else
    {
        m_dones++;
    }
    _GO go;
    go.index = index;
    go.oper = oper;
    go.pGraphic = p;

    TRACE("LOG DONE %s / UN: %d DN: %d REF: %d\n", go.Trace(), m_undos, m_dones, m_refs[go.pGraphic]);
    m_listDone.AddHead(go);
}

void CGraphicLog::Done()
{
    // * * * ������޸����ü����Ͳ����������� * * *

    // �ָ�������ѭoperָ��

    if (m_dones == 0)
    {
        return;
    }
    TRACE("LOG DONE ------\n");
    m_dones--;
    _GO go = m_listDone.GetHead();
    CGraphic* pOb = NULL;
    switch (go.oper)
    {
    case GOS_ADD:
        // ��Ӳ���
        m_parr->InsertAt(go.index, go.pGraphic);
        
        // �ӱ����б��ƶ���Ŀ�����飬���ü�������
        // ��ӳ������������ü�����һ
        // ��֮���ü�����һ
        AddRef(go.pGraphic);
        Operator(GOS_ADD, go.pGraphic, go.index, FALSE);
        break;
    case GOS_DELETE:
        // ɾ������
        m_parr->RemoveAt(go.index);

        // ԭ�������䱻ɾ�����ָ��б�ɾ�������ü�������
        // Ψһ�����ڳ����б��У����ü�����һ
        // ��֮���ü�����һ
        Delete(go.pGraphic);
        Operator(GOS_DELETE, go.pGraphic, go.index, FALSE);
        break;
    case GOS_UPDATE:
        // ���Ĳ���
        pOb = Convert_To_Graphic(m_parr->GetAt(go.index));
        m_parr->ElementAt(go.index) = go.pGraphic;

        // go.pGraphic �ָ��б�->Ŀ�����飬���ü�������
        // pOb Ŀ������->�ָ��б����ü�������
        Operator(GOS_UPDATE, pOb, go.index, FALSE);
        break;
    default: ASSERT(!"operation fault!");
    }
    m_listDone.RemoveHead();
}

void CGraphicLog::Operator( GOS oper, CGraphic* p, int index, BOOL bClear /*= TRUE*/ )
{
    ASSERT_VALID(p);

    // ÿ�β���֮�󣬼���Ļָ�����Ӧ��ȫ�����
    // ʹ���߲���ʱ������bClearΪ��
    // ��������ʱ��bClearΪ��
    if (bClear) ClearDone();
    
    if (bClear)
    {
        // * * * ������޸����ü����Ͳ����������� * * *

        // ���ǽ������m_obArray��*m_parr���Ƴ�����listUndo������ô����������
        switch (oper)
        {
        case GOS_ADD:
            // ʹ�ñ����Add(CGraphic*)��Ӷ��󲢳�ʼ�����ü���
            ASSERT(!Add(p));
            // ��Ϊ�����б���Ҫ������Ӳ������������ü�����һ
            AddRef(p);
            // �������ü���Ϊ��
            break;
        case GOS_DELETE:
            // �����ԭ�������Ƴ�������ɾ����
            m_parr->RemoveAt(index);
            break;
        case GOS_UPDATE:
            // ���Ĳ�������ʱҪ����ԭ���󣨸���ǰ�ģ�
            // �����޸ĺ�Ķ��������´����ģ�û�����ü���
            // ���Ի��ó�ʼ�����ü���
            // ��ʱpΪ�½�����
            ASSERT(!AddRef(p));
            break;
        default: ASSERT(!"Operation fault!");
        }
    }

    if (m_undos == MAX_SAVE)
    {
        // ��������б��Ѿ������Զ�ɾ����β
        ASSERT(!m_listUndo.IsEmpty());
        Delete(m_listUndo.GetTail().pGraphic);
        m_listUndo.RemoveTail();
    }
    else
    {
        m_undos++;
    }
    _GO go;
    go.index = index;
    go.oper = oper;
    go.pGraphic = p;
    TRACE("LOG OPER %d %s / UN: %d DN: %d REF: %d\n", bClear, go.Trace(), m_undos, m_dones, m_refs[go.pGraphic]);

    // ��ӳ�����¼
    m_listUndo.AddHead(go);
}

void CGraphicLog::ClearDone()
{
    if (!CanDo()) return;
    TRACE("$ Clear DONE start - - >>>>>>\n");
    POSITION pos = m_listDone.GetHeadPosition();
    _GO go;
    while (pos)
    {
        go = m_listDone.GetNext(pos);
        TRACE("LOG CLEAR DONE DELETE %s / UN: %d DN: %d REF: %d\n", go.Trace(), m_undos, m_dones, m_refs[go.pGraphic] - 1);
        Delete(go.pGraphic);
    }
    m_listDone.RemoveAll();
    m_dones = 0;
    TRACE("$ Clear DONE end - - <<<<<<\n");
}

void CGraphicLog::ClearUndo()
{
    if (!CanUndo()) return;
    TRACE("$ Clear UNDO start - - >>>>>>\n");
    POSITION pos = m_listUndo.GetHeadPosition();
    _GO go;
    while (pos)
    {
        go = m_listUndo.GetNext(pos);
        TRACE("LOG CLEAR DONE DELETE %s / UN: %d DN: %d REF: %d\n", go.Trace(), m_undos, m_dones, m_refs[go.pGraphic] - 1);
        Delete(go.pGraphic);
    }
    m_listUndo.RemoveAll();
    m_undos = 0;
    TRACE("$ Clear UNDO end- - <<<<<<\n");
}

void CGraphicLog::ClearArray()
{
    TRACE("$ Clear GRAPHIC Object Array start - - >>>>>>\n");
    POSITION pos = m_refs.GetStartPosition();
    CGraphic* pOb; int ref;
    while (pos)
    {
        m_refs.GetNextAssoc(pos, pOb, ref);
        TRACE("LOG CLEAR ARRAY - %p - [%s] - [%s] - REF: %d\n", pOb, pOb->GetName(), pOb->m_lpszName, ref - 1);
        Delete(pOb);
    }
    TRACE("$ Clear GRAPHIC Object Array end - - <<<<<<\n");
}

void CGraphicLog::Clear()
{
    TRACE("$ Clear LOG start - - >>>>\n");
    // ����ָ�����
    ClearDone();
    // �����������
    ClearUndo();
    // ��ʱ�����ñ���Ӧ�ö��ǽ��е����ü���Ϊһ�Ķ���
    // ���Trace����0����ô����û������
    ClearArray();
    TRACE("$ Clear LOG end - - <<<<\n");
}

void CGraphicLog::Delete( CGraphic* pOb )
{
    // ɾ�����������ҽ������ü���Ϊ1ʱ�����������ã�ɾ��
    ASSERT_VALID(pOb);
    int ref;
    if (m_refs.Lookup(pOb, ref))
    {
        ASSERT(ref >= 1);
        if (ref == 1)
        {
			for (int i = 0; i < m_parr->GetSize(); i++)
			{
				if (m_parr->GetAt(i) == (CObject*)pOb)
				{
					TRACE("Graphic Delete ID: %d, ADDR: %p In Main Array\n", i, pOb);
					m_parr->RemoveAt(i);
					break;
				}
			}
            delete pOb;
            m_refs.RemoveKey(pOb);
            return;
        }
        m_refs[pOb] = ref - 1;
    }
    else
    {
        ASSERT(!"Object not found!");
    }
}

BOOL CGraphicLog::AddRef( CGraphic* pOb )
{
    // �������ü���
    ASSERT_VALID(pOb);
    int ref;
    if (m_refs.Lookup(pOb, ref))
    {
        m_refs[pOb] = ref + 1;
        return TRUE;
    }
    else
    {
        m_refs[pOb] = 1;
        return FALSE;
    }

    // �����ǳ�ʼ�����ü�������ô����FALSE
}

BOOL CGraphicLog::Add( CGraphic* pOb )
{
    // �½������ı������
    // ����������������
    // ��ʼ�����ü���
    ASSERT_VALID(pOb);
    m_parr->Add(pOb);
    return AddRef(pOb);
}

BOOL CGraphicLog::Add( CGraphic* pOb, int ID )
{
    // ֻ�����л���ȡʱ��������ͼ�ε����ü�����ʼ��Ϊ1
    // m_parr֮ǰ�������SetSize�������죩
    ASSERT_VALID(pOb);
    m_parr->ElementAt(ID) = pOb;
    return AddRef(pOb);
}

IMPLEMENT_SERIAL( CLine, CGraphic, 1 )
CLine::CLine(): CGraphic(LINE)
{
    
}

LPCTSTR CLine::GetName() const
{
    return _T("ֱ��");
}

void CLine::Draw( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CPen pen;
    CreateGdiObjectFromId(GBS_PEN, m_iPenStyle, &pen, m_iPenWidth, m_clrPen);
    pDC->SelectObject(&pen);
    pDC->MoveTo(m_pt1);
    pDC->LineTo(m_pt2);
}

void CLine::DrawSelectedEdge( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    pDC->SelectObject(&pen);
    CRect rt;
    int inf = INF_WIDTH + m_iPenWidth;
    GraphicDrawSelectedEdge(pDC, m_pt1, inf);
    GraphicDrawSelectedEdge(pDC, m_pt2, inf);
}

void CLine::HitTest( CPoint& pt, BOOL& bResult )
{
    if (!PtInRectTest(pt)) return;
    LineHitTest(m_pt1, m_pt2, pt, bResult);
}

BOOL CLine::EnableBrush() const
{
    return FALSE;
}

void CLine::GetRect( CRect& rt )
{
    rt.SetRect(m_pt1, m_pt2);
    rt.NormalizeRect();
    LONG inf = m_iPenWidth + INF_WIDTH + 1;
    rt.InflateRect(inf, inf);    
}

void CLine::UpdateData( GraphicMember* pSrc, BOOL bSave /*= TRUE */ )
{
    CGraphic::UpdateData(pSrc, bSave);
    if (bSave)
    {
        m_clrPen = pSrc->m_clrPen;
        m_iPenStyle = pSrc->m_iPenStyle;
        m_iPenWidth = pSrc->m_iPenWidth;
    }
    else
    {
        pSrc->m_clrPen = m_clrPen;
        pSrc->m_iPenStyle = m_iPenStyle;
        pSrc->m_iPenWidth = m_iPenWidth;
    }
}

void CLine::Serialize( CArchive &ar )
{
    CGraphic::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_clrPen << m_iPenStyle << m_iPenWidth;
    }
    else
    {
        ar >> m_clrPen >> m_iPenStyle >> m_iPenWidth;
    }
}

LPCTSTR CLine::HitSizingTest( CPoint& pt, BOOL& bResult, LONG** PtX /*= NULL*/, LONG** PtY /*= NULL */ )
{
    int inf = DRAG_INF_WIDTH + m_iPenWidth;

    GraphicHitSizingTest(m_pt1.x, m_pt1.y, inf, pt, bResult, PtX, PtY);
    if (bResult) return MAKEINTRESOURCE(IDC_SIZEALL);
    GraphicHitSizingTest(m_pt2.x, m_pt2.y, inf, pt, bResult, PtX, PtY);
    if (bResult) return MAKEINTRESOURCE(IDC_SIZEALL);

    return NULL;
}

IMPLEMENT_SERIAL( CRectangle, CGraphic, 1 )
CRectangle::CRectangle(): CGraphic(RECTANGLE)
{
    
}

LPCTSTR CRectangle::GetName() const
{
    return _T("����");
}

void CRectangle::Draw( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CGraphic::Draw(pDC);
    CPen pen;
    CreateGdiObjectFromId(GBS_PEN, m_iPenStyle, &pen, m_iPenWidth, m_clrPen);
    pDC->SelectObject(&pen);
    CBrush br;
    CreateGdiObjectFromId(GBS_BRUSH, m_iBrushStyle, &br, 0, m_clrBrush);
    pDC->SelectObject(&br);
    pDC->Rectangle(m_pt1.x, m_pt1.y, m_pt2.x, m_pt2.y);
}

void CRectangle::DrawSelectedEdge( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CRect rt;
    int inf = INF_WIDTH + m_iPenWidth;
    pDC->SelectObject(&pen);
    CPoint pt;
    CPoint ct;
    ct.x = ( m_pt1.x + m_pt2.x ) / 2;
    ct.y = ( m_pt1.y + m_pt2.y ) / 2;

    GraphicDrawSelectedEdge(pDC, m_pt1, inf);
    GraphicDrawSelectedEdge(pDC, m_pt2, inf);
    pt.x = m_pt2.x; pt.y = m_pt1.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = m_pt1.x; pt.y = m_pt2.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);

    if (m_pt2.x - m_pt1.x < inf + INF_WIDTH || m_pt2.y - m_pt1.y < inf + INF_WIDTH)
    {
        return;
    }
    
    pt.x = ct.x; pt.y = m_pt1.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = ct.x; pt.y = m_pt2.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = m_pt1.x; pt.y = ct.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = m_pt2.x; pt.y = ct.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
}

void CRectangle::HitTest( CPoint& pt, BOOL& bResult )
{
    if (!PtInRectTest(pt)) return;

    static CPoint pt1, pt2;
    pt1.x = m_pt1.x; pt1.y = m_pt2.y; pt2.x = m_pt2.x; pt2.y = m_pt1.y;

    LineHitTest(m_pt1, pt1, pt, bResult); if (bResult) return;
    LineHitTest(m_pt1, pt2, pt, bResult); if (bResult) return;
    LineHitTest(m_pt2, pt1, pt, bResult); if (bResult) return;
    LineHitTest(m_pt2, pt2, pt, bResult);
}

void CRectangle::GetRect( CRect& rt )
{
    rt.SetRect(m_pt1, m_pt2);
    rt.NormalizeRect();
    LONG inf = m_iPenWidth + INF_WIDTH + 1;
    rt.InflateRect(inf, inf);    
}

void CRectangle::UpdateData( GraphicMember* pSrc, BOOL bSave /*= TRUE */ )
{
    CGraphic::UpdateData(pSrc, bSave);
    if (bSave)
    {
        m_clrPen = pSrc->m_clrPen;
        m_iPenStyle = pSrc->m_iPenStyle;
        m_iPenWidth = pSrc->m_iPenWidth;
        m_clrBrush = pSrc->m_clrBrush;
        m_iBrushStyle = pSrc->m_iBrushStyle;
    }
    else
    {
        pSrc->m_clrPen = m_clrPen;
        pSrc->m_iPenStyle = m_iPenStyle;
        pSrc->m_iPenWidth = m_iPenWidth;
        pSrc->m_clrBrush = m_clrBrush;
        pSrc->m_iBrushStyle = m_iBrushStyle;
    }
}

void CRectangle::Serialize( CArchive &ar )
{
    CGraphic::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_clrPen << m_iPenStyle << m_iPenWidth << m_clrBrush << m_iBrushStyle;
    }
    else
    {
        ar >> m_clrPen >> m_iPenStyle >> m_iPenWidth >> m_clrBrush >> m_iBrushStyle;
    }
}

LPCTSTR CRectangle::HitSizingTest( CPoint& pt, BOOL& bResult, LONG** PtX /*= NULL*/, LONG** PtY /*= NULL */ )
{
    int inf = DRAG_INF_WIDTH + m_iPenWidth;
    
    BOOL bTrans = TRUE;
    if ((m_pt1.x > m_pt2.x && m_pt1.y < m_pt2.y) || (m_pt1.x < m_pt2.x && m_pt1.y > m_pt2.y))
    {
        bTrans = FALSE;
    }
    GraphicHitSizingTest(m_pt1.x, m_pt1.y, inf, pt, bResult, PtX, PtY);
    if (bResult) return bTrans ? IDC_SIZENWSE : IDC_SIZENESW;
    GraphicHitSizingTest(m_pt2.x, m_pt2.y, inf, pt, bResult, PtX, PtY);
    if (bResult) return bTrans ? IDC_SIZENWSE : IDC_SIZENESW;
    GraphicHitSizingTest(m_pt1.x, m_pt2.y, inf, pt, bResult, PtX, PtY);
    if (bResult) return bTrans ? IDC_SIZENESW : IDC_SIZENWSE;
    GraphicHitSizingTest(m_pt2.x, m_pt1.y, inf, pt, bResult, PtX, PtY);
    if (bResult) return bTrans ? IDC_SIZENESW : IDC_SIZENWSE;

    LONG X = ( m_pt1.x + m_pt2.x ) / 2;
    LONG Y = ( m_pt1.y + m_pt2.y ) / 2;
    GraphicHitSizingTest(X, m_pt1.y, inf, pt, bResult, NULL, PtY);
    if (bResult) return IDC_SIZENS;
    GraphicHitSizingTest(X, m_pt2.y, inf, pt, bResult, NULL, PtY);
    if (bResult) return IDC_SIZENS;
    GraphicHitSizingTest(m_pt1.x, Y, inf, pt, bResult, PtX, NULL);
    if (bResult) return IDC_SIZEWE;
    GraphicHitSizingTest(m_pt2.x, Y, inf, pt, bResult, PtX, NULL);
    if (bResult) return IDC_SIZEWE;

    return NULL;
}

IMPLEMENT_SERIAL( CEllipse, CGraphic, 1 )
CEllipse::CEllipse(): CGraphic(ELLIPSE)
{
    
}

LPCTSTR CEllipse::GetName() const
{
    return _T("��Բ");
}

void CEllipse::Draw( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CGraphic::Draw(pDC);
    CPen pen;
    CreateGdiObjectFromId(GBS_PEN, m_iPenStyle, &pen, m_iPenWidth, m_clrPen);
    pDC->SelectObject(&pen);
    CBrush br;
    CreateGdiObjectFromId(GBS_BRUSH, m_iBrushStyle, &br, 0, m_clrBrush);
    pDC->SelectObject(&br);
    pDC->Ellipse(m_pt1.x, m_pt1.y, m_pt2.x, m_pt2.y);
}

void CEllipse::DrawSelectedEdge( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CRect rt;
    int inf = INF_WIDTH + m_iPenWidth;
    pDC->SelectObject(&pen);
    CPoint pt;
    CPoint ct;
    ct.x = ( m_pt1.x + m_pt2.x ) / 2;
    ct.y = ( m_pt1.y + m_pt2.y ) / 2;
    
    GraphicDrawSelectedEdge(pDC, m_pt1, inf);
    GraphicDrawSelectedEdge(pDC, m_pt2, inf);
    pt.x = m_pt2.x; pt.y = m_pt1.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = m_pt1.x; pt.y = m_pt2.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    
    if (m_pt2.x - m_pt1.x < inf + INF_WIDTH || m_pt2.y - m_pt1.y < inf + INF_WIDTH)
    {
        return;
    }
    
    pt.x = ct.x; pt.y = m_pt1.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = ct.x; pt.y = m_pt2.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = m_pt1.x; pt.y = ct.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
    pt.x = m_pt2.x; pt.y = ct.y;
    GraphicDrawSelectedEdge(pDC, pt, inf);
}

void CEllipse::HitTest( CPoint& pt, BOOL& bResult )
{
    /*
        ��Բ�ĳ��᳤ A |m_pt1.x - m_pt2.x| �� B |m_pt1.y - m_pt2.y|
        ���� 4*(X*X*A*A+Y*Y*B*B)=A*A+B*B
    */

    if (!PtInRectTest(pt)) return;

    CPoint S; // ���ĵ�
    S.x = ( m_pt1.x + m_pt2.x ) / 2;
    S.y = ( m_pt1.y + m_pt2.y ) / 2;
    CSize L; // �볤���᳤
    L = m_pt2 - S;
    CPoint P; // �������
    P = pt - S;
    LONG abs_ = (LONG) abs(P.x*P.x*L.cy*L.cy+P.y*P.y*L.cx*L.cx-L.cx*L.cx*L.cy*L.cy);
    float miss = (float) abs_ / (L.cx * L.cy * L.cx * L.cy);
    bResult = ( (float) fabsf(miss) < SELECT_MISS_ELLIPSE ) ? TRUE : FALSE;
}

void CEllipse::GetRect( CRect& rt )
{
    rt.SetRect(m_pt1, m_pt2);
    rt.NormalizeRect();
    LONG inf = m_iPenWidth + INF_WIDTH + 1;
    rt.InflateRect(inf, inf);
}

void CEllipse::UpdateData( GraphicMember* pSrc, BOOL bSave /*= TRUE */ )
{
    CGraphic::UpdateData(pSrc, bSave);
    if (bSave)
    {
        m_clrPen = pSrc->m_clrPen;
        m_iPenStyle = pSrc->m_iPenStyle;
        m_iPenWidth = pSrc->m_iPenWidth;
        m_clrBrush = pSrc->m_clrBrush;
        m_iBrushStyle = pSrc->m_iBrushStyle;
    }
    else
    {
        pSrc->m_clrPen = m_clrPen;
        pSrc->m_iPenStyle = m_iPenStyle;
        pSrc->m_iPenWidth = m_iPenWidth;
        pSrc->m_clrBrush = m_clrBrush;
        pSrc->m_iBrushStyle = m_iBrushStyle;
    }
}

void CEllipse::Serialize( CArchive &ar )
{
    CGraphic::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_clrPen << m_iPenStyle << m_iPenWidth << m_clrBrush << m_iBrushStyle;
    }
    else
    {
        ar >> m_clrPen >> m_iPenStyle >> m_iPenWidth >> m_clrBrush >> m_iBrushStyle;
    }
}

LPCTSTR CEllipse::HitSizingTest( CPoint& pt, BOOL& bResult, LONG** PtX /*= NULL*/, LONG** PtY /*= NULL */ )
{
    int inf = DRAG_INF_WIDTH + m_iPenWidth;
        
    LONG X = ( m_pt1.x + m_pt2.x ) / 2;
    LONG Y = ( m_pt1.y + m_pt2.y ) / 2;
    GraphicHitSizingTest(X, m_pt1.y, inf, pt, bResult, NULL, PtY);
    if (bResult) return IDC_SIZENS;
    GraphicHitSizingTest(X, m_pt2.y, inf, pt, bResult, NULL, PtY);
    if (bResult) return IDC_SIZENS;
    GraphicHitSizingTest(m_pt1.x, Y, inf, pt, bResult, PtX, NULL);
    if (bResult) return IDC_SIZEWE;
    GraphicHitSizingTest(m_pt2.x, Y, inf, pt, bResult, PtX, NULL);
    if (bResult) return IDC_SIZEWE;
    
    return NULL;
}

IMPLEMENT_SERIAL(CCurve, CGraphic, 1)
CCurve::CCurve(): CGraphic(CURVE)
{
    
}

LPCTSTR CCurve::GetName() const
{
    return _T("����");
}

void CCurve::Draw( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CPen pen;
    CreateGdiObjectFromId(GBS_PEN, m_iPenStyle, &pen, m_iPenWidth, m_clrPen);
    pDC->SelectObject(&pen);
    int size = m_pts.GetSize();

    CPoint pt;
    pDC->MoveTo(m_pt1);
    for (int i = 0; i < size; i++)
    {
        pt = m_pts[i] + m_pt1;
        pDC->LineTo(pt);
    }
    pDC->LineTo(m_pt2);
}

void CCurve::Serialize( CArchive &ar )
{
    CGraphic::Serialize(ar);
    int size = 0;
    if ( ar.IsStoring() )
    {
        size = m_pts.GetSize();
        ar << m_clrPen << m_iPenStyle << m_iPenWidth << size;
        if (size == 0)
        {
            return;
        }
        for (int i = 0; i < size; i++)
        {
            ar << m_pts[i];
        }
    }
    else
    {
        ar >> m_clrPen >> m_iPenStyle >> m_iPenWidth >> size;
        if (size == 0)
        {
            return;
        }
        m_pts.SetSize(size);
        for (int i = 0; i < size; i++)
        {
            ar >> m_pts[i];
        }
    }
}

void CCurve::DrawSelectedEdge( CDC* pDC )
{
    ASSERT_VALID(pDC);
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    pDC->SelectObject(&pen);
    CRect rt;
    CPoint pt;
    int inf = INF_WIDTH + m_iPenWidth;
    GraphicDrawSelectedEdge(pDC, m_pt1, inf);
    for (int i = 0; i < m_pts.GetSize(); i+=3)
    {
        pt = m_pts[i] + m_pt1;
        GraphicDrawSelectedEdge(pDC, pt, inf);
    }
    GraphicDrawSelectedEdge(pDC, m_pt2, inf);
}

void CCurve::HitTest( CPoint& pt, BOOL& bResult )
{
    CPoint a, b;
    a = m_pt1;
    for (int i = 0; i < m_pts.GetSize(); i++)
    {
        b = m_pts[i] + m_pt1;
        LineHitTest(a, b, pt, bResult); if (bResult) return;
        a = b;
    }
    b = m_pt2;
    LineHitTest(a, b, pt, bResult);    
}

void CCurve::GetRect( CRect& rt )
{
    rt.SetRect(m_pt1, m_pt2);
    rt.NormalizeRect();
    CPoint pt;
    for (int i = 0; i < m_pts.GetSize(); i++)
    {
        pt = m_pts[i] + m_pt1;
        if (!rt.PtInRect(pt))
        {
            if (pt.x < rt.left)     rt.left = pt.x;
            if (pt.y < rt.top)      rt.top = pt.y;
            if (pt.x > rt.right)    rt.right = pt.x;
            if (pt.y > rt.bottom)   rt.bottom = pt.y;
        }
    }
    LONG inf = m_iPenWidth + INF_WIDTH + 1;
    rt.InflateRect(inf, inf);
}

int CCurve::GetPts() const
{
    return CGraphic::GetPts() + m_pts.GetSize();
}

BOOL CCurve::EnableBrush() const
{
    return FALSE;
}

void CCurve::UpdateData( GraphicMember* pSrc, BOOL bSave /*= TRUE */ )
{
    CGraphic::UpdateData(pSrc, bSave);
    if (bSave)
    {
        m_clrPen = pSrc->m_clrPen;
        m_iPenStyle = pSrc->m_iPenStyle;
        m_iPenWidth = pSrc->m_iPenWidth;
        m_pts.Copy(pSrc->m_pts);
    }
    else
    {
        pSrc->m_clrPen = m_clrPen;
        pSrc->m_iPenStyle = m_iPenStyle;
        pSrc->m_iPenWidth = m_iPenWidth;
        pSrc->m_pts.Copy(m_pts);
    }
}
