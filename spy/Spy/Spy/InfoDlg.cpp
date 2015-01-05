// InfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Spy.h"
#include "InfoDlg.h"
#include "afxdialogex.h"
#include "SpyDlg.h"

// CInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CInfoDlg, CDialogEx)

    CInfoDlg::CInfoDlg(CWnd* pParent /*= Spy Window*/)
    : CDialogEx(CInfoDlg::IDD, pParent)
{
    // ��ʼ��
    icon_from = AfxGetApp()->LoadIcon(IDI_FROM);
    icon_blank = AfxGetApp()->LoadIcon(IDI_SCAN);
    cursor_scan = AfxGetApp()->LoadCursor(IDC_SCAN);
    saved_win_handle = NULL;
    is_scanning = FALSE;
    prev_window = NULL;
}

CInfoDlg::~CInfoDlg()
{
    DestroyCursor(cursor_scan);
    DestroyIcon(icon_from);
    DestroyIcon(icon_blank);
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_POS, edit_pos);
    DDX_Control(pDX, IDC_SPYPIC, scan);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialogEx)
    ON_WM_CLOSE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CInfoDlg ��Ϣ�������


void CInfoDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if(saved_win_handle != INVALID_HANDLE_VALUE)
    {
        ::ShowWindowAsync(saved_win_handle, SW_SHOW); // ǿ����ʾ
    }
    CDialogEx::OnClose();
}


void CInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    // ������ڴ���
    CWnd *pWnd = ChildWindowFromPoint(point);

    // ������Spy_Scan_Icon������ɨ��
    if(pWnd != NULL && pWnd->GetSafeHwnd() == scan.GetSafeHwnd())
    {
        SpyStartScanning();
    }

    CDialogEx::OnLButtonDown(nFlags, point);
}


void CInfoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    // ֹͣɨ��
    if(is_scanning)
    {
        SpyStopScanning();
    }

    CDialogEx::OnLButtonUp(nFlags, point);
}


void CInfoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    // ɨ����
    if(is_scanning)
    {
        SpyExecScanning(point);
    }

    CDialogEx::OnMouseMove(nFlags, point);
}


void CInfoDlg::SpyStartScanning()
{
    is_scanning = TRUE;
    scan.SetIcon(icon_blank);
    SetCursor(cursor_scan);

    SetCapture(); // ������겶��
}


void CInfoDlg::SpyStopScanning()
{
    is_scanning = FALSE;
    scan.SetIcon(icon_from);
    SpyInvertBorder(saved_win_handle);
    ReleaseCapture();
    CSpyDlg* spy_dlg_ptr = (CSpyDlg*)m_pParentWnd;
    spy_dlg_ptr->spy_window = saved_win_handle;
    HCURSOR cursor_normal = LoadCursor(NULL, IDC_ARROW);
    SetCursor(cursor_normal);

    Sleep(100);
    SendMessage(WM_CLOSE);
}


void CInfoDlg::SpyExecScanning(CPoint &pt)
{
    ClientToScreen(&pt); // ת������Ļ����

    {
        CString buf;
        buf.Format(L"(%d, %d)", pt.x, pt.y);
        edit_pos.SetWindowText(buf.GetBuffer());
        buf.ReleaseBuffer();
    }

    HWND current_window = SpyFindSmallestWindow(pt); //�ҵ���ǰλ�õ���С����

    if(current_window != NULL)
    {
        // ȷ���ҵ��Ĵ��ڲ����Լ��Ĵ��ڣ����ڽ��̲�ͬ
        if(GetWindowThreadProcessId(GetSafeHwnd(), NULL) != GetWindowThreadProcessId(current_window, NULL))
        {
            // �����´��ڣ��ͰѾɴ��ڵı߽�ȥ�������´��ڵı߽�
            if(current_window != prev_window)
            {	
                SpyInvertBorder(prev_window);
                prev_window = current_window;
                SpyInvertBorder(prev_window);
            }
        }
        else
        {
            // �������Լ���
            SpyInvertBorder(prev_window);
            prev_window = NULL;
        }
    }

    saved_win_handle = prev_window;

    UpdateData(FALSE); // ��������
}


HWND CInfoDlg::SpyFindSmallestWindow(const CPoint &pt)
{
    HWND hWnd = WindowFromPoint(pt)->GetSafeHwnd(); // ������ڴ���

    if(hWnd != NULL)
    {
        // �õ������ڴ�С�͸����ھ�����Ա�Ƚ�
        CRect rect;
        ::GetWindowRect(hWnd, &rect);
        HWND parent = ::GetParent(hWnd); // ������

        // ֻ�иô����и����ڲż����Ƚ�
        if(parent != NULL)
        {
            // ��Z��������
            HWND find = hWnd; // �ݹ���þ��
            CRect rect_find;

            while(TRUE) // ѭ��
            {
                find = ::GetWindow(find, GW_HWNDNEXT); // �õ���һ�����ڵľ��
                ::GetWindowRect(find, &rect_find); // �õ���һ�����ڵĴ�С

                if( ::PtInRect(&rect_find, pt) // �������λ���Ƿ����´�����
                    && ::GetParent(find) == parent // �´��ڵĸ������Ƿ����������������
                    && ::IsWindowVisible(find)) // �����Ƿ����
                {
                    // �Ƚϴ��ڣ����ĸ���С
                    if(rect_find.Width() * rect_find.Height() < rect.Width() * rect.Height())
                    {
                        // �ҵ���С����
                        hWnd = find;

                        // �����´��ڵĴ�С
                        ::GetWindowRect(hWnd, &rect);
                    }
                }

                // hWnd���Ӵ���findΪNULL����hWndΪ��С����
                if(find == NULL)
                {
                    break; // �˳�ѭ��
                }
            }
        }
    }

    return hWnd;
}


void CInfoDlg::SpyInvertBorder(const HWND &hWnd)
{
    // ���Ǵ����򷵻�
    if(!IsWindow(hWnd))
        return;

    CRect rect; // ���ھ���

    // �õ����ھ���
    ::GetWindowRect(hWnd, &rect);

    CDC dc; // �����豸������
    dc.Attach(::GetWindowDC(hWnd));

    // ���ô��ڵ�ǰǰ��ɫ�Ļ��ģʽΪR2_NOT
    // R2_NOT - ��ǰ������ֵΪ��Ļ����ֵ��ȡ�����������Ը��ǵ��ϴεĻ�ͼ
    dc.SetROP2(R2_NOT);

    // ��������
    CPen pen;

    // PS_INSIDEFRAME - ���������״�Ŀ����ֱ�ߣ�ָ��һ���޶�����
    // 3 * GetSystemMetrics(SM_CXBORDER) - �����߽��ϸ
    // RGB(0,0,0) - ��ɫ
    pen.CreatePen(PS_INSIDEFRAME, 3 * GetSystemMetrics(SM_CXBORDER), RGB(0, 0, 0));

    // ѡ�񻭱�
    HGDIOBJ old_pen = dc.SelectObject(pen);

    // �趨��ˢ
    HGDIOBJ old_brush = dc.SelectObject(GetStockObject(NULL_BRUSH));

    // ������
    dc.Rectangle(0, 0, rect.Width(), rect.Height());

    // �ָ�ԭ�����豸����
    dc.SelectObject(old_pen);
    dc.SelectObject(old_brush);

    pen.DeleteObject();
}