#include "Menu.h"
#include <iostream>
#include <IOMANIP>

using namespace std;

// 检测用户输入的字符串
const char *NumberList = "1234567890";

Menu::~Menu()
{
    for (int i = 0; i < m_IndexSub.Size(); i++)
    {
        delete m_SubMenus.At(i);
    }
}

int Menu::AppendOption(const char *szComment, MenuOptions p)
{
    assert(m_Counts < 9 || !"Menu::AppendOption(MenuOptions) : 'm_Counts' exceeded!");
    m_Type.Append(MT_OPT);
    m_IndexOp.Append(m_Counts);
    m_Options.Append(p);
    m_Comment.Append(szComment);
    m_Counts++;
    return m_Counts - 1;
}

int Menu::AppendMenu(const char *szComment)
{
    assert(m_Counts < 9 || !"Menu::AppendMenu() : 'm_Counts' exceeded!");
    m_Type.Append(MT_SUB);
    m_IndexSub.Append(m_Counts);
    m_SubMenus.Append(new Menu(szComment, m_Bottom, this));
    m_Comment.Append(szComment);
    m_Counts++;
    return m_Counts - 1;
}

Menu& Menu::GetMenuById(int ID)
{
    assert(ID >= 0 && ID < m_Counts && "Menu::GetMenuById(int) : Parameter 'ID' fault!");
    for (int i = 0; i < m_IndexSub.Size(); i++)
    {
        if (m_IndexSub.AtRef(i) == ID)
        {
            break;
        }
    }
    return *m_SubMenus.At(i);
}

MenuOptions Menu::GetOptionById(int ID)
{
    assert(ID >= 0 && ID < m_Counts && "Menu::GetOptionById(int) : Parameter 'ID' fault!");
    for (int i = 0; i < m_IndexOp.Size(); i++)
    {
        if (m_IndexOp.AtRef(i) == ID)
        {
            break;
        }
    }
    return m_Options.AtRef(i);
}

int Menu::MenuLoop(char key)
{
    for (int i = 0; i < m_Counts; i++)
    {
        if (key == NumberList[i])
        {
            switch (m_Type.AtRef(i))
            {
            case MT_OPT:
                // 若是选项则调用相应函数
                GetOptionById(i)(i);
            	break;
            case MT_SUB:
                // 若是子菜单则进入子菜单的循环判断
                GetMenuById(i).Run();
                break;
            }
        }
    }
    if (key == NumberList[m_Counts])
    {
        return 0;
    }
    return 1;
}

void Menu::PrintHeader(const char *szHeader /* = NULL */)
{
    const char *lpszHeader = (szHeader == NULL) ? m_Title : szHeader;
    PrintHorizen();
    PrintBlank();
    cout << "丨"
        << setfill(' ') << setw((m_Width - 2 - strlen(lpszHeader) / 2 * 2) / 2) << ""
        << lpszHeader
        << setfill(' ') << setw((m_Width - 2 - strlen(lpszHeader)) / 2) << ""
        << "丨" << endl;
    PrintShortHorizen();
    PrintBlank();
}

void Menu::PrintShortHorizen()
{
    cout << "丨"
        << setfill(' ') << setw((m_Width - 2 - m_Horizen) / 2) << ""
        << setfill('_') << setw(m_Horizen) << ""
        << setfill(' ') << setw((m_Width - 2 - m_Horizen) / 2) << ""
        << "丨" << endl;
}

void Menu::PrintBottom(const char *szBottom /* = NULL */)
{
    cout << "丨" << setfill(' ') << setw(m_Width - 24) << ""
        << m_Bottom << "丨" << endl;
    cout << ends << setfill('_') << setw(m_Width) << "" << endl;
}

void Menu::PrintBlank(int lines /* = 1 */)
{
    if (lines <= 0)
    {
        return;
    }
    for (int i = 0; i < lines; i++)
    {
        cout << "丨" << setfill(' ') << setw(m_Width - 2) << "" << "丨" << endl;
    }
}

void Menu::PrintHorizen(int lines /* = 1 */)
{
    if (lines <= 0)
    {
        return;
    }
    for (int i = 0; i < lines; i++)
    {
        cout << ends << setfill('_') << setw(m_Width) << "" << endl;
    }
}

void Menu::PrintMenu()
{
    for (int i = 0; i < m_Counts; i++)
    {
        cout << "丨"
            << setfill(' ') << setw(4) << ""
            << NumberList[i] << " "
            << setfill('.') << setw(6) << "" << " "
            << setiosflags(ios::left) << setfill(' ') << setw(m_Width - 15)
            << m_Comment.AtRef(i)
            << "丨" << endl;
    }
    const char *szLastOptions = (m_Parent != NULL) ? "返回" : "退出";
    cout << "丨"
        << setfill(' ') << setw(4) << ""
        << NumberList[i] << " "
        << setfill('.') << setw(6) << "" << " "
        << setiosflags(ios::left) << setfill(' ') << setw(m_Width - 15)
        << szLastOptions
        << "丨" << endl;
}

void Menu::DisplayComment()
{
    Cls();

    // 打印界面头
    PrintHeader();

    PrintBlank();

    // 打印菜单
    PrintMenu();

    PrintBlank(2);

    // 打印界面尾
    PrintBottom(); 
    
    cout << "请输入序号: " << ends;
}

void Menu::Run()
{
    assert(m_Counts > 0 && "Menu::Run() : 'm_Counts' fault!");
    string title("title ");
    title.append(m_Title);
    system(title.c_str());
    Cls();
    char ch = '\0';
    while (true)
    {
        DisplayComment();
        cin >> ch;
        int ret = MenuLoop(ch);
        // 如果MenuLoop返回0则跳出循环
        if (ret == 0)
        {
            break;
        }
    }
    if (m_Parent == NULL)
    {
        cout << "再见！" << endl;
        Pause();
        return;
    }
    else
    {
        // 返回上层菜单
        m_Parent->Run();
    }
}