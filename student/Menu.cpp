#include "Menu.h"
#include <iostream>
#include <IOMANIP>

using namespace std;

// ����û�������ַ���
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
                // ����ѡ���������Ӧ����
                GetOptionById(i)(i);
            	break;
            case MT_SUB:
                // �����Ӳ˵�������Ӳ˵���ѭ���ж�
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
    cout << "ح"
        << setfill(' ') << setw((m_Width - 2 - strlen(lpszHeader) / 2 * 2) / 2) << ""
        << lpszHeader
        << setfill(' ') << setw((m_Width - 2 - strlen(lpszHeader)) / 2) << ""
        << "ح" << endl;
    PrintShortHorizen();
    PrintBlank();
}

void Menu::PrintShortHorizen()
{
    cout << "ح"
        << setfill(' ') << setw((m_Width - 2 - m_Horizen) / 2) << ""
        << setfill('_') << setw(m_Horizen) << ""
        << setfill(' ') << setw((m_Width - 2 - m_Horizen) / 2) << ""
        << "ح" << endl;
}

void Menu::PrintBottom(const char *szBottom /* = NULL */)
{
    cout << "ح" << setfill(' ') << setw(m_Width - 24) << ""
        << m_Bottom << "ح" << endl;
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
        cout << "ح" << setfill(' ') << setw(m_Width - 2) << "" << "ح" << endl;
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
        cout << "ح"
            << setfill(' ') << setw(4) << ""
            << NumberList[i] << " "
            << setfill('.') << setw(6) << "" << " "
            << setiosflags(ios::left) << setfill(' ') << setw(m_Width - 15)
            << m_Comment.AtRef(i)
            << "ح" << endl;
    }
    const char *szLastOptions = (m_Parent != NULL) ? "����" : "�˳�";
    cout << "ح"
        << setfill(' ') << setw(4) << ""
        << NumberList[i] << " "
        << setfill('.') << setw(6) << "" << " "
        << setiosflags(ios::left) << setfill(' ') << setw(m_Width - 15)
        << szLastOptions
        << "ح" << endl;
}

void Menu::DisplayComment()
{
    Cls();

    // ��ӡ����ͷ
    PrintHeader();

    PrintBlank();

    // ��ӡ�˵�
    PrintMenu();

    PrintBlank(2);

    // ��ӡ����β
    PrintBottom(); 
    
    cout << "���������: " << ends;
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
        // ���MenuLoop����0������ѭ��
        if (ret == 0)
        {
            break;
        }
    }
    if (m_Parent == NULL)
    {
        cout << "�ټ���" << endl;
        Pause();
        return;
    }
    else
    {
        // �����ϲ�˵�
        m_Parent->Run();
    }
}