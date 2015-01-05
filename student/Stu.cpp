#include "Stu.h"

void Example(int)
{
    cout << "hello!" << endl;
    system("pause");
}

IStu *g_StuApp = new StuApp("学生信息管理系统  V1.0", "By -- 陈晨 22121912   ");
const char* const g_arrInfo[8] = 
{
    "学号",
    "姓名",
    "性别",
    "出生年月",
    "专业",
    "班级",
    "家庭住址",
    "宿舍号码"
};


// 创建实例对象
void CreateStuApp(IStu **i)
{
    assert(i);
    *i = g_StuApp;
}

StuApp::StuApp(const char *szTitle /* = NULL */, const char *szPS /* = NULL */):
m_Menu(szTitle, szPS)
{
    // 编辑菜单
    int hr = 0, i = 0;
    m_Menu.AppendOption("读取文件", LoadFromFile);
    m_Menu.AppendOption("保存文件", SaveToFile);
    m_Menu.AppendOption("查看数据结构", ViewStructure);
    hr = m_Menu.AppendMenu("打印信息...");
    m_Menu.GetMenuById(hr).AppendOption("按个人打印", PrintInfo);
    m_Menu.AppendOption("增加记录", AppendPerson);
    hr = m_Menu.AppendMenu("编辑记录...");
    for (i = 0; i < 8; i++)
    {
        m_Menu.GetMenuById(hr).AppendOption(g_arrInfo[i], EditPerson);
    }
    hr = m_Menu.AppendMenu("查找记录...");
    for (i = 0; i < 8; i++)
    {
        m_Menu.GetMenuById(hr).AppendOption(g_arrInfo[i], SearchCallByIndex);
    }
    hr = m_Menu.AppendMenu("删除记录...");
    for (i = 0; i < 8; i++)
    {
        m_Menu.GetMenuById(hr).AppendOption(g_arrInfo[i], DeleteCallByIndex);
    }
    hr = m_Menu.AppendMenu("排序整理...");
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
    // 显示菜单
    m_Menu.Run();
}

// p是一个指针 p->string数组指针 {string* Q[];p=&Q;}
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
    cout << "打开文件中..." << endl;
    bool bRet = g_StuApp->Load();
    if (bRet)
    {
        cout << "打开文件成功！" << endl;
    }
    else
    {
        cout << "打开文件失败！文件不存在！" << endl;
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
    cout << "保存文件中..." << endl;
    bool bRet = g_StuApp->Save();
    if (bRet)
    {
        cout << "保存文件成功！" << endl;
    }
    else
    {
        cout << "保存文件失败！" << endl;
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
        cout << "文件为空！" << endl;
        return false;
    }
    if (m_Data.Empty())
    {
        cout << "没有数据！" << endl;
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

    m_Menu.PrintHeader("数据结构");
    m_Menu.PrintBlank();

    for (int i = 0; i < 8; i++)
    {
        cout << "丨"
            << setfill(' ') << setw(4) << ""
            << i << " "
            << setfill('_') << setw(6) << "" << " "
            << setiosflags(ios::left) << setfill(' ') << setw(16)
            << g_arrInfo[i]
            << " ======> "
            << setiosflags(ios::left) << setfill(' ') << setw(m_Menu.GetWidth()-16-24)
            << szComment[i]
            << "丨" << endl;
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
        cout << "没有找到数据！" << endl;
        Pause();
        return;
    }
    Cls();
    string* ppStr[8] = {NULL};
    string szSubTitle("");
    if (Index >=0 && Index < 8)
    {
        szSubTitle = "查找结果: ";
        szSubTitle.append(g_arrInfo[Index]);
        szSubTitle.append("   ");
    }
    for (int i = 0; i < counts; i++)
    {
        Cls();
        char szTitle[100] = {0};
        sprintf(szTitle, "%s第 %d 条 / 共 %d 条", szSubTitle.c_str(), i+1, counts);
        m_Menu.PrintHeader(szTitle);
        m_Menu.PrintBlank();
        GetInfoIndex((STUINFO*)&(pData->AtRef(i)), (string**)ppStr);
        
        for (int j = 0; j < 8; j++)
        {
            cout << "丨" << setfill(' ') << setw(12) << "" << setw(24) << g_arrInfo[j] << setw(m_Menu.GetWidth() - 38)
            << *(ppStr[j]) << "丨" << endl;
        }

        m_Menu.PrintShortHorizen();
        m_Menu.PrintBlank();

        cout << "丨" << setfill(' ') << setw(12) << "" << setw(24) << "索引编号(*)" << setw(m_Menu.GetWidth() - 38)
             << i << "丨" << endl;        
        
        m_Menu.PrintBlank(2);
        m_Menu.PrintBottom();

        if (isOptions)
        {
            cout << "输入N返回，G查看任意记录，其他则继续: ";
            char ch = '\0';
            cin >> ch;
            if (ch == 'N' || ch == 'n')
            {
                break;
            }
            else if (ch == 'G' || ch == 'g')
            {
                int ID = 0;
                cout << "输入索引编号: ";
                cin >> ID;
                if (ID >= 0 && ID < counts)
                {
                    cout << "跳转至: " << i << endl;
                    i = ID - 1;
                }
                else
                {
                    cout << "无此编号！" << endl;
                }
            }
        }
    }
    cout << "查看完毕！" << endl;
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
    sprintf(szTitle, "新建第 %d 条记录", counts+1);
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
            cout << "丨" << setfill(' ') << setw(12) << "" << setw(24) << g_arrInfo[j] << setw(m_Menu.GetWidth() - 38)
            << *(ppStr[j]) << "丨" << endl;
        }
        
        m_Menu.PrintBlank(2);
        m_Menu.PrintBottom();

        if (i == 8)
        {
            break;
        }
        
        cout << "请输入" << g_arrInfo[i] << ": ";
        cin >> *(ppStr[i]);

        i++;
    }
    m_Data.Append(si);
    cout << "信息已录入！" << endl;
    Pause();
}

void StuApp::SearchCallByIndex(int i)
{
    cout << "请输入" << g_arrInfo[i] << ":  ";
    string str;
    cin >> str;
    g_StuApp->Search(i, str);
}

void StuApp::Search(int choose, string& str)
{
    int counts = m_Data.Size();
    if (counts == 0)
    {
        cout << "没有数据！" << endl;
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
                // 选中找到的合适项
                stu.Append(m_Data.AtRef(index));
            }
        }
        Print(&stu, choose);
    }
    else
    {
        cout << "参数错误！" << endl;
    }
}

void StuApp::DeleteCallByIndex(int i)
{
    cout << "请输入" << g_arrInfo[i] << ":  ";
    string str;
    cin >> str;
    g_StuApp->Delete(i, str);
}

void StuApp::Delete(int choose, string& str)
{
    int counts = m_Data.Size();
    if (counts == 0)
    {
        cout << "没有数据！" << endl;
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
                // 选中不合适项，以添加到新链表
                stu.Append(m_Data.AtRef(index));
            }
        }
        if (counts == stu.Size())
        {
            cout << "未找到匹配项！" << endl;
        }
        else
        {
            cout << "删除成功！" << ends;
            cout << "删除 " << counts - stu.Size() << " 条记录" << endl;
            m_Data = stu;
        }
        Pause();
    }
    else
    {
        cout << "参数错误！" << endl;
    }
}

void StuApp::EditPerson(int choose)
{
    cout << "请输入该学生的索引编号:  ";
    int index;
    cin >> index;
    g_StuApp->Update(index, choose);
}

void StuApp::Update(int index, int choose)
{
    if (m_Data.Empty())
    {
        cout << "没有数据！" << endl;
        Pause();
        return;
    }
    if (index < 0 || index >= m_Data.Size())
    {
        cout << "不存在该索引！" << endl;
        return;
    }
    LinkedList<STUINFO> one;
    one.Append(m_Data.AtRef(index));

    Print(&one, choose, false);
    
    cout << "确认编辑? Y/N  ";
    char ch = '\0';
    cin >> ch;
    if (ch == 'N' || ch == 'n')
    {
        cout << "放弃编辑！" << endl;
        Pause();
        return;
    }
    else if (ch == 'Y' || ch == 'y')
    {
        string newinfo;
        cout << "请输入新的" << g_arrInfo[choose] << ": ";
        cin >> newinfo;
        string *pArr[8] = {NULL};
        GetInfoIndex((STUINFO*)&m_Data.AtRef(index), (string**)pArr);
        cout << *(pArr[choose]) << " 已经更改为 " << newinfo << endl;
        *pArr[choose] = "";
        pArr[choose]->append(newinfo);
        Pause();
    }
}

void StuApp::SortCallByIndex(int choose)
{
    cout << "Y: 由小到大排序 N:由大到小排序 请输入:  ";
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
        cout << "输入错误！";
    }
}

void StuApp::Sort(int choose, bool bGreater /* = true */)
{
    if (m_Data.Empty())
    {
        cout << "没有数据！" << endl;
        Pause();
        return;
    }
    cout << "排序中..." << endl;
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
    cout << "排序完毕！" << endl;
    Pause();
}