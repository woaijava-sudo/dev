// HelpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ex_draw.h"
#include "HelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpDlg dialog


CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHelpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpDlg)
	DDX_Control(pDX, IDC_STATIC_HLP, m_stat_help);
	DDX_Control(pDX, IDC_EDIT_HELP, m_edit_help);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialog)
	//{{AFX_MSG_MAP(CHelpDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpDlg message handlers

BOOL CHelpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_captionFont.CreateFont(30, 15, 0, 0, 16, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("����"));

    m_txtFont.CreateFont(16, 8, 0, 0, 10, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("����"));

    m_edit_help.SetFont(&m_txtFont);
    m_stat_help.SetFont(&m_captionFont);
    GetDlgItem(IDOK)->SetFont(&m_txtFont);

    CString str = _T(
        " �����漰���������͹��ܽ��⡣\r\n"

        "\r\n ============================= һ�����ܸ��� =============================\r\n"
        "\r\n 1. �����������"
        "\r\n 2. �ļ��ɱ���Ϊλͼ����դ������ʸ��ͼ�����л���"
        "\r\n 3. ��ӿ������͹�����"
        "\r\n 4. ���ͼ���б�˫����λ��"
        "\r\n 5. ͼ����Ϣ��ʾ��ѡ���Ҽ���"
        "\r\n 6. ͼ����Ϣ�༭��ѡ���Ҽ���"
        "\r\n 7. ͼ��ɾ����ѡ���Ҽ���"
        "\r\n 8. ͼ����ק�븴�ƣ���Ctrl����"
        "\r\n 9. ͼ���������"
        "\r\n 10. ͼ��λ��΢�������������"
        "\r\n 11. �����ͻָ�����"
        "\r\n 12. Tab����Ԥ����Tab����"
        "\r\n 13. �Ҽ�ֱ�ӽ���ѡ��״̬"
        "\r\n 14. �������غ���ʾ���أ�ͼ�β˵���"
        "\r\n 15. ��Ӧ������"
        "\r\n 15. ������̬��Ӧ���ڴ�С�仯"
        "\r\n \r\n"

        "\r\n ============================= ����ʹ�ð��� =============================\r\n"
        "\r\n 1. ͼƬֻ�ܱ���Ϊλͼ��*.bmp������Ϊ����ѹ��"
        "\r\n 2. �������ɹ�ѡ������ݣ�"
        "\r\n    ��1��������ɫ����񡢴�ϸ"
        "\r\n    ��2����ˢ��ɫ�����"
        "\r\n 3. ��ѡ��ģʽ�¿��Խ��еĲ�����"
        "\r\n    ��1��ѡ�С��϶���������Ρ���CTRL�����ơ������������΢��"
        "\r\n    ��2���Ҽ��˵�������Ϣ����ѡ�У�������������С��ͼ�β�����ͼ����Ϣ�ſ�"
        "\r\n    ��3��ѡ��֮����밴סͼ����Χ��Բ����б���"
        "\r\n 4. �ڻ�ͼģʽ�¿��Խ��еĲ�����"
        "\r\n    ��1��һ�����"
        "\r\n    ��2����SHIFT�������Σ���Բ�������Ρ�б45��ֱ�ߣ�"
        "\r\n 5. ���ӹ��ܣ�"
        "\r\n    ��1��Tab���򣺿�����ʾ�ڡ�ͼ�Ρ��˵��е���"
        "\r\n    ��2�����أ�ͼ���������أ�������ʾ�ڡ�ͼ�Ρ��˵��е���"
        "\r\n    ��3���Զ������ƣ�ͼ�������и���"
        "\r\n    ��4��ͼ���б�˫���ɶ�λͼ�Σ�ѡ��ͼ��Ҳ�ᶨλ�б�"
        "\r\n \r\n"

        "\r\n ============================= ��������ṹ =============================\r\n"
        "\r\n 1. �ſ�"
        "\r\n    ��1������������Լ3000�У�View��~1000 Graphic��~1000��"
        "\r\n    ��2����Ҫ����������*�Ļ��м������⣩"
        "\r\n        * CGraphic�����ֱࣺ����CLine����Բ��CEllipse��������CRectangle��������CCurve��"
        "\r\n        CHelpDlg��ģ̬������ʾ���棩"
        "\r\n        * CStartupDlg����ģ̬�������棩"
        "\r\n        ConfigDlg��ģ̬ͼ�α༭���棩"
        "\r\n        GraphicMember��ͨ��ͼ������ת���ࣩ"
        "\r\n        * CGraphicLog��������¼�࣬ʵ�ֳ����ͻָ���"
        "\r\n 2. ��Ľ���ͼ�����������*�Ļ���⣩"
        "\r\n    ��1��CGraphic��ͼ�λ���"
        "\r\n        1) ���༰���������ݳ�Ա�������ƣ��ԣ�"
        "\r\n        2) ��Ҫ��̬����"
        "\r\n            CreateGraphic�� ���ݽ��ʴ�����Ӧ�ಢ��ֵ"
        "\r\n            GetIdBySelection... �� �洢���ʣ�ˢ�����͵�һ�ַ�ʽ������λ����"
        "\r\n            LineHitTest�� ������ֱ���ϣ���ô�����ߺ͵��ڵ����ߣ��Դ��ж�"
        "\r\n        3) ��Ҫ��Ա����"
        "\r\n            Serialize�� ���л����������б����ȵ��ø�������л���"
        "\r\n            UpdateData�� ������MFC�е�UpdateData��GraphicMember�䵱�������ʣ�"
        "\r\n            Draw�� ��ͼ"
        "\r\n            DrawSelectedEdge�� ��ѡ�к����ܵĵ�"
        "\r\n            * HitTest�� ���Ե��Ƿ���ͼ���ڲ������ǣ�bResultΪ��"
        "\r\n            * HitSizingTest�� ���Ե��Ƿ���ͼ�ε����ܱ��ε��ϣ����ǣ�bResultΪ��"
        "\r\n            GetRect�� ����ͼ�εľ������꣬�����ػ�"
        "\r\n        4) ��"
        "\r\n            ѡ������->N������N����ȷ��"
        "\r\n                SELECT_MISS_LINE�� ֱ�ߣ�->1��"
        "\r\n                SELECT_MISS_ELLIPSE�� ��Բ��->0��"
        "\r\n                DRAG_INF_WIDTH�� ����Բ��"
        "\r\n            ������"
        "\r\n                INF_WIDTH�� ͼ����߿���"
        "\r\n                LOG_MAX_SAVE�� �������"
        "\r\n                ͼ��������ͣ�LINE...��"
        "\r\n                IF_NULL_PEN�� �Ƿ���͸������"
        "\r\n                IF_NULL_BRUSH�� �Ƿ���͸����ˢ"
        "\r\n                Convert_To_Graphic��ת��Ϊͼ����"
        "\r\n        5) ��*����"
        "\r\n            HitTest��HitSizingTest����ʵ���Ǹ��ص������������жϵ��Ƿ���ͼ���ڲ�"
        "\r\n                HitSizingTest�������������� PtX PtY����int**���ͣ���ԭ��ģ�"
        "\r\n                ����Ҫ��ͼ�α��Σ���Ҫ�������꣬��ʱҪ������A�����X��B�����Y"
        "\r\n                ��ô��Ҫ����ԭ�����LONG����ָ�룬�ټ�Ӹı���"
        "\r\n        6) ��������"
        "\r\n            ����ͼ�εĽ�������⡣������ô�룬��������ͼ�ξͲ����϶��͸��ģ�ͼ��"
        "\r\n                ���ݱ�ķḻ��������Ӧ�����Ϣ�����Լ���λͼ����ô��������ʵ��"
        "\r\n                ��һ����ҵ����UI�Ĺ��ܡ����ԣ����������UI��˼���������Щ�ࡣ"
        "\r\n            һ��Ŀ�ԴUI����Duilib��XCGUI���漰����ģʽ���۲���ģʽ�ȵ�ģʽ����"
        "\r\n                ʵ�ֿؼ������Ӳ�Ա��룬�������ZIP���أ�ͨ������XML��ͼƬ����"
        "\r\n                ���Զ������Ĵ����������˼�롣"
        "\r\n            ����Ȥ���Կ�Java�ġ�Head First���ģʽ�������Բ�ͬ����˼���ǻ�ͨ�ġ�"
        "\r\n            MFC��˼�벻��ֻ�ǿ������Բ��㣬��ȫ�Բ��㣬�������ڵ����һ�����"
        "\r\n                ��Դ�Ļ��Լ������Ľ���⡣"
        "\r\n            ������˼�룬����ʵ�־Ͳ������⣨ֻ��ʱ�����⣩��"
        "\r\n    ��2��CGraphicLog"
        "\r\n        1) ö���壺 ����������ͣ��������������ģ�ɾ��"
        "\r\n        2) �ṹ�壺 GraphicOperation �����������ͣ���������ݶ��󣬶�Ӧ�±�"
        "\r\n            Trace�������ڵ���"
        "\r\n        3) ���ʽ��ͣ� Undo - ������Done - �ָ�"
        "\r\n        4) ����"
        "\r\n            �����ʵ��˼����Ϊ���ӣ��������ע�ͣ�"
        "\r\n            ��Ȼ��������Ա��CList����ʵ�ֵĹ����Ƕ�ջ��˼��Ϊʲô��"
        "\r\n            ��Ϊɾ���˶��󣬲�����ɾ��������Ȼ��ô�ָ���"
        "\r\n            ���󱻸����ˣ��ñ������֮ǰ�ı��ݣ�����UPDATE������new������"
        "\r\n            ���ֻ��˶�������ôDone��Ķ����������ڣ���¼�ǲ����ˣ�Ҫɾ"
        "\r\n            ���ǻᵼ���ظ�ɾ����˼����������ô�����"
        "\r\n               �����á�˼��"
        "\r\n            ��������ʱά�����ü���Ϊ1��ɾ��ʱ��ȥ1���������Ϊ0��ɾ����"
        "\r\n    ��3��CStartupDlg"
        "\r\n        ���ù��ߣ���ʱ������ģ̬�Ի����߳�"
        "\r\n        �߳�ͬ���������¼���CEvent->HANDLE����WaitingForSingleObject"
        "\r\n \r\n"

        "\r\n 3. ������������"
        "\r\n    ��1��˫����λͼ�ͺ�̨λͼ"
        "\r\n            ֱ������ĻDC�ϲ�������ôÿ��һ�Σ��͵ø���һ�ν��棬���Ի�����"
        "\r\n            ����ڻ����ϲ�����Ȼ��BitBlt����Ļ����ֻҪ����һ��"
        "\r\n            ��̨λͼ�Ĺ����ǣ�����קͼ��A��ʱ�򣬳�ȥ���ͼ��֮�������ͼ��"
        "\r\n            ȫ������X�ϣ���ʱ�϶�A��ֻҪ����Ļ�����X���ٻ��ı�������A��"
        "\r\n            �ټ���˫���壬�Ϳ�����Ч��������"
        "\r\n            ע��Ϊ�˼�㣨�ۡ���������Щϸ��û�д������Ի��ǻ�������"
        "\r\n            ע2�� ����ЩBitBlt����֮ǰ�������ȵ���UpdateWindow"
        "\r\n    ��2������Ϊλͼ"
        "\r\n            λͼ��BITMAPFILEHEADER��BITMAPINFO��λͼ����"
        "\r\n            ����ʵ�����XXXView::OnSaveAsBitmap�������̶�"
        "\r\n \r\n"

        "\r\n ============================= �ġ��������� =============================\r\n"
        "\r\n 1. ���˴��д����ʽ����ϰ�ߣ���ʽ��ֱ���������ͱ����Ķ���"
        "\r\n    * �Ƽ���ʽ�����ߣ�Visual Assist X��"
        "\r\n 2. ���������Խϳ�����˻���δ���ֵ�BUG����"
        "\r\n 3. ����ע�ͽ��٣��ʽ�����д�ڰ�����"
        "\r\n 4. д����Ҳ��Ϊ���ܽ�MFC����Ҫ����"
        "\r\n 5. ����Ϥ����ͷ������Բ�ѯMSDN"
        "\r\n 6. ��δ���ԵĹ��ܣ���ѡ���ܸ��ӣ��൱�����½���������ת�����ؼ����㣩��"
        "\r\n   ��PS�����Ժ󵹵ز���"
        "\r\n 7. ������£�����"
    );

    m_edit_help.SetWindowText(str);

    GetDlgItem(IDOK)->SetFocus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CHelpDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	

	// TODO: Change any attributes of the DC here
    if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_HLP)
    {
        pDC->SetTextColor(RGB(255, 0, 0));
    }
    else if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_EDIT_HELP)
    {
        pDC->SetTextColor(RGB(30, 20, 235));
    }

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
