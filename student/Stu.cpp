#include "Stu.h"

void Example(int)
{
    cout << "hello!" << endl;
    system("pause");
}

IStu *g_StuApp = new StuApp("ѧ����Ϣ����ϵͳ  V1.0", "By -- �³� 22121912   ");
const char* const g_arrInfo[8] = 
{
    "ѧ��",
    "����",
    "�Ա�",
    "��������",
    "רҵ",
    "�༶",
    "��ͥסַ",
    "�������"
};


// ����ʵ������
void CreateStuApp(IStu **i)
{
    assert(i);
    *i = g_StuApp;
}

StuApp::StuApp(const char *szTitle /* = NULL */, const char *szPS /* = NULL */):
m_Menu(szTitle, szPS)
{
    // �༭�˵�
    int hr = 0, i = 0;
    m_Menu.AppendOption("��ȡ�ļ�", LoadFromFile);
    m_Menu.AppendOption("�����ļ�", SaveToFile);
    m_Menu.AppendOption("�鿴���ݽṹ", ViewStructure);
    hr = m_Menu.AppendMenu("��ӡ��Ϣ...");
    m_Menu.GetMenuById(hr).AppendOption("�����˴�ӡ", PrintInfo);
    m_Menu.AppendOption("���Ӽ�¼", AppendPerson);
    hr = m_Menu.AppendMenu("�༭��¼...");
    for (i = 0; i < 8; i++)
    {
        m_Menu.GetMenuById(hr).AppendOption(g_arrInfo[i], EditPerson);
    }
    hr = m_Menu.AppendMenu("���Ҽ�¼...");
    for (i = 0; i < 8; i++)
    {
        m_Menu.GetMenuById(hr).AppendOption(g_arrInfo[i], SearchCallByIndex);
    }
    hr = m_Menu.AppendMenu("ɾ����¼...");
    for (i = 0; i < 8; i++)
    {
        m_Menu.GetMenuById(hr).AppendOption(g_arrInfo[i], DeleteCallByIndex);
    }
    hr = m_Menu.AppendMenu("��������...");
    for (i = 0; i < 8; i++)
    {
        m_Menu.GetMenuById(hr).AppendOption(g_arrInfo[i], SortCallByIndex);
    }
}

StuApp::~StuApp()
{

}

void StuApp::Run()
{
    // ��ʾ�˵�
    m_Menu.Run();
}

// p��һ��ָ�� p->string����ָ�� {string* Q[];p=&Q;}
void StuApp::GetInfoIndex(STUINFO *si, string **p)
{
    p[0] = &si->szID;
    p[1] = &si->szName;
    p[2] = &si->szSex;
    p[3] = &si->szBirth;
    p[4] = &si->szMajor;
    p[5] = &si->szClass;
    p[6] = &si->szAddress;
    p[7] = &si->szPhone;
}

void StuApp::LoadFromFile(int)
{
    cout << "���ļ���..." << endl;
    bool bRet = g_StuApp->Load();
    if (bRet)
    {
        cout << "���ļ��ɹ���" << endl;
    }
    else
    {
        cout << "���ļ�ʧ�ܣ��ļ������ڣ�" << endl;
    }
    g_StuApp->Pause();
}

bool StuApp::Load()
{
    ifstream ifs("Data.txt");
    if (!ifs.is_open())
    {
        return false;
    }
    if (m_Data.Size() > 0)
    {
        m_Data.Clear();
    }
    istringstream ss;
    string strLine;
    while (ifs.good() && !ifs.eof())
    {
        ss.clear();
        getline(ifs, strLine, '\n');
        ss.str(strLine);
        STUINFO si;
        ss >> si.szID
           >> si.szName
           >> si.szSex
           >> si.szBirth
           >> si.szMajor
           >> si.szClass
           >> si.szAddress
           >> si.szPhone;
        if (ss.fail())
        {
            continue;
        }
        m_Data.Append(si);
    }
    ifs.close();
    return true;
}

void StuApp::SaveToFile(int)
{
    cout << "�����ļ���..." << endl;
    bool bRet = g_StuApp->Save();
    if (bRet)
    {
        cout << "�����ļ��ɹ���" << endl;
    }
    else
    {
        cout << "�����ļ�ʧ�ܣ�" << endl;
    }
    g_StuApp->Pause();
}

bool StuApp::Save()
{
    ofstream ofs("Data.txt");
    if (!ofs.is_open())
    {
        return false;
    }
    if (ofs.eof())
    {
        cout << "�ļ�Ϊ�գ�" << endl;
        return false;
    }
    if (m_Data.Empty())
    {
        cout << "û�����ݣ�" << endl;
        return false;
    }

    for (int i = 0; i < m_Data.Size(); i++)
    {
        ofs << m_Data.AtRef(i).szID << " "
            << m_Data.AtRef(i).szName << " "
            << m_Data.AtRef(i).szSex << " "
            << m_Data.AtRef(i).szBirth << " "
            << m_Data.AtRef(i).szMajor << " "
            << m_Data.AtRef(i).szClass << " "
            << m_Data.AtRef(i).szAddress << " "
            << m_Data.AtRef(i).szPhone << endl;

        ofs.flush();
    }
    
    ofs.close();
    return true;
}

void StuApp::View()
{
    Cls();
    const char *szComment[8] =
    {
        "szID",
            "szName",
            "szSex",
            "szBirth (YYYY/MM/DD eg. 2000/01/01)",
            "szMajor",
            "szClass",
            "szAddress",
            "szPhone"
    };

    m_Menu.PrintHeader("���ݽṹ");
    m_Menu.PrintBlank();

    for (int i = 0; i < 8; i++)
    {
        cout << "ح"
            << setfill(' ') << setw(4) << ""
            << i << " "
            << setfill('_') << setw(6) << "" << " "
            << setiosflags(ios::left) << setfill(' ') << setw(16)
            << g_arrInfo[i]
            << " ======> "
            << setiosflags(ios::left) << setfill(' ') << setw(m_Menu.GetWidth()-16-24)
            << szComment[i]
            << "ح" << endl;
    }
    
    m_Menu.PrintBlank();
    m_Menu.PrintBottom();
    Pause();
}

void StuApp::ViewStructure(int)
{
    g_StuApp->View();
}

void StuApp::PrintInfo(int)
{
    g_StuApp->Print();
}

void StuApp::Print(LinkedList<STUINFO>* pData /* = NULL */, int Index /* = -1 */, bool isOptions /* = true */)
{
    if (pData == NULL)
    {
        pData = &m_Data;
    }
    int counts = pData->Size();
    if (counts == 0)
    {
        cout << "û���ҵ����ݣ�" << endl;
        Pause();
        return;
    }
    Cls();
    string* ppStr[8] = {NULL};
    string szSubTitle("");
    if (Index >=0 && Index < 8)
    {
        szSubTitle = "���ҽ��: ";
        szSubTitle.append(g_arrInfo[Index]);
        szSubTitle.append("   ");
    }
    for (int i = 0; i < counts; i++)
    {
        Cls();
        char szTitle[100] = {0};
        sprintf(szTitle, "%s�� %d �� / �� %d ��", szSubTitle.c_str(), i+1, counts);
        m_Menu.PrintHeader(szTitle);
        m_Menu.PrintBlank();
        GetInfoIndex((STUINFO*)&(pData->AtRef(i)), (string**)ppStr);
        
        for (int j = 0; j < 8; j++)
        {
            cout << "ح" << setfill(' ') << setw(12) << "" << setw(24) << g_arrInfo[j] << setw(m_Menu.GetWidth() - 38)
            << *(ppStr[j]) << "ح" << endl;
        }

        m_Menu.PrintShortHorizen();
        m_Menu.PrintBlank();

        cout << "ح" << setfill(' ') << setw(12) << "" << setw(24) << "�������(*)" << setw(m_Menu.GetWidth() - 38)
             << i << "ح" << endl;        
        
        m_Menu.PrintBlank(2);
        m_Menu.PrintBottom();

        if (isOptions)
        {
            cout << "����N���أ�G�鿴�����¼�����������: ";
            char ch = '\0';
            cin >> ch;
            if (ch == 'N' || ch == 'n')
            {
                break;
            }
            else if (ch == 'G' || ch == 'g')
            {
                int ID = 0;
                cout << "�����������: ";
                cin >> ID;
                if (ID >= 0 && ID < counts)
                {
                    cout << "��ת��: " << i << endl;
                    i = ID - 1;
                }
                else
                {
                    cout << "�޴˱�ţ�" << endl;
                }
            }
        }
    }
    cout << "�鿴��ϣ�" << endl;
    Pause();
}

void StuApp::AppendPerson(int)
{
    g_StuApp->Add();
}

void StuApp::Add()
{
    Cls();
    int counts = m_Data.Size();
    char szTitle[100] = {0};
    sprintf(szTitle, "�½��� %d ����¼", counts+1);
    STUINFO si;
    string* ppStr[8] = {NULL};
    GetInfoIndex(&si, (string**)ppStr);
    int i = 0;
    while (true)
    {
        Cls();
        m_Menu.PrintHeader(szTitle);
        m_Menu.PrintBlank();
        
        for (int j = 0; j < 8; j++)
        {
            cout << "ح" << setfill(' ') << setw(12) << "" << setw(24) << g_arrInfo[j] << setw(m_Menu.GetWidth() - 38)
            << *(ppStr[j]) << "ح" << endl;
        }
        
        m_Menu.PrintBlank(2);
        m_Menu.PrintBottom();

        if (i == 8)
        {
            break;
        }
        
        cout << "������" << g_arrInfo[i] << ": ";
        cin >> *(ppStr[i]);

        i++;
    }
    m_Data.Append(si);
    cout << "��Ϣ��¼�룡" << endl;
    Pause();
}

void StuApp::SearchCallByIndex(int i)
{
    cout << "������" << g_arrInfo[i] << ":  ";
    string str;
    cin >> str;
    g_StuApp->Search(i, str);
}

void StuApp::Search(int choose, string& str)
{
    int counts = m_Data.Size();
    if (counts == 0)
    {
        cout << "û�����ݣ�" << endl;
        Pause();
        return;
    }
    if (choose >= 0 && choose < 8)
    {
        LinkedList<STUINFO> stu;
        string *arrPtr[8] = {NULL};
        for (int index = 0; index < counts; index++)
        {
            GetInfoIndex((STUINFO*)&m_Data.AtRef(index), (string**)arrPtr);
            if (str.compare(*(arrPtr[choose])) == 0)
            {
                // ѡ���ҵ��ĺ�����
                stu.Append(m_Data.AtRef(index));
            }
        }
        Print(&stu, choose);
    }
    else
    {
        cout << "��������" << endl;
    }
}

void StuApp::DeleteCallByIndex(int i)
{
    cout << "������" << g_arrInfo[i] << ":  ";
    string str;
    cin >> str;
    g_StuApp->Delete(i, str);
}

void StuApp::Delete(int choose, string& str)
{
    int counts = m_Data.Size();
    if (counts == 0)
    {
        cout << "û�����ݣ�" << endl;
        Pause();
        return;
    }
    if (choose >= 0 && choose < 8)
    {
        LinkedList<STUINFO> stu;
        string *arrPtr[8] = {NULL};
        for (int index = 0; index < counts; index++)
        {
            GetInfoIndex((STUINFO*)&m_Data.AtRef(index), (string**)arrPtr);
            if (str.compare(*(arrPtr[choose])) != 0)
            {
                // ѡ�в����������ӵ�������
                stu.Append(m_Data.AtRef(index));
            }
        }
        if (counts == stu.Size())
        {
            cout << "δ�ҵ�ƥ���" << endl;
        }
        else
        {
            cout << "ɾ���ɹ���" << ends;
            cout << "ɾ�� " << counts - stu.Size() << " ����¼" << endl;
            m_Data = stu;
        }
        Pause();
    }
    else
    {
        cout << "��������" << endl;
    }
}

void StuApp::EditPerson(int choose)
{
    cout << "�������ѧ�����������:  ";
    int index;
    cin >> index;
    g_StuApp->Update(index, choose);
}

void StuApp::Update(int index, int choose)
{
    if (m_Data.Empty())
    {
        cout << "û�����ݣ�" << endl;
        Pause();
        return;
    }
    if (index < 0 || index >= m_Data.Size())
    {
        cout << "�����ڸ�������" << endl;
        return;
    }
    LinkedList<STUINFO> one;
    one.Append(m_Data.AtRef(index));

    Print(&one, choose, false);
    
    cout << "ȷ�ϱ༭? Y/N  ";
    char ch = '\0';
    cin >> ch;
    if (ch == 'N' || ch == 'n')
    {
        cout << "�����༭��" << endl;
        Pause();
        return;
    }
    else if (ch == 'Y' || ch == 'y')
    {
        string newinfo;
        cout << "�������µ�" << g_arrInfo[choose] << ": ";
        cin >> newinfo;
        string *pArr[8] = {NULL};
        GetInfoIndex((STUINFO*)&m_Data.AtRef(index), (string**)pArr);
        cout << *(pArr[choose]) << " �Ѿ�����Ϊ " << newinfo << endl;
        *pArr[choose] = "";
        pArr[choose]->append(newinfo);
        Pause();
    }
}

void StuApp::SortCallByIndex(int choose)
{
    cout << "Y: ��С�������� N:�ɴ�С���� ������:  ";
    char ch = '\0';
    cin >> ch;
    if (ch == 'Y' || ch == 'y')
    {
        g_StuApp->Sort(choose);
    }
    else if (ch == 'N' || ch == 'n')
    {
        g_StuApp->Sort(choose, false);
    }
    else
    {
        cout << "�������";
    }
}

void StuApp::Sort(int choose, bool bGreater /* = true */)
{
    if (m_Data.Empty())
    {
        cout << "û�����ݣ�" << endl;
        Pause();
        return;
    }
    cout << "������..." << endl;
    LinkedList<STUCOMP> stucmp;
    STUCOMP tmp;
    int counts = m_Data.Size();
    string *arrStr[8] = {NULL};
    for (int i = 0; i < counts; i++)
    {
        GetInfoIndex((STUINFO*)&m_Data.AtRef(i), (string**)arrStr);
        tmp.szInfo = arrStr[choose];
        tmp.ID = i;
        stucmp.Insert(tmp, bGreater);
    }
    LinkedList<STUINFO> newlist;
    for (int j = 0; j < counts; j++)
    {
        newlist.Append(m_Data.AtRef(stucmp.AtRef(j).ID));
    }
    m_Data = newlist;
    cout << "������ϣ�" << endl;
    Pause();
}