#pragma once
#include "List.h"
#include "StuInfo.h"

typedef void (*MenuOptions)(int);

/*
Menu: �˵���
  */
class Menu
{
public:
    // �˵�����ö��
    enum MENU_TYPE
    {
        MT_OPT,
        MT_SUB,
    };

public:
    /* ���캯��
       ��һ���������˵�����
       �ڶ������������˵�ָ�루������ΪNULL��
    */
    Menu(const char *szTitle = "�˵�", const char *szBottom = "", Menu *pParent = NULL):
      m_Counts(0), m_Parent(pParent), m_Width(76), m_Horizen(60),
      m_Title(szTitle == NULL ? "�˵�" : szTitle),
      m_Bottom(szBottom == NULL ? "" : szBottom){}
    ~Menu();

    /* ����ѡ��
       ��һ��������ѡ������
       �ڶ���������ѡ����Ӧ�ĺ���ָ��
       ����ֵ��ѡ��ID
    */ 
    int AppendOption(const char *, MenuOptions);

    /* �����Ӳ˵�
       ��һ���������Ӳ˵�����
       ����ֵ���˵�ID
    */
    int AppendMenu(const char *);

    // �õ��Ӳ˵���ID
    Menu& GetMenuById(int);

    // �õ�ѡ��ĺ���ָ��
    MenuOptions GetOptionById(int);

    // �õ��ܵĲ˵���
    const int GetMenuCounts() const {return m_Counts;}

    // �õ��Ӳ˵���ѡ�����
    MENU_TYPE GetTypeById(int ID) {return m_Type.At(ID);}

    // �û������ж�
    int MenuLoop(char);

    // ��ʾ�Ӳ˵���Ϣ
    void DisplayComment();

    // ѭ���жϿ�ʼ
    void Run();

    // ����
    void Cls() {system("cls");}

    // ��ͣ
    void Pause() {system("pause");}

    // ��ӡ����ͷ
    void PrintHeader(const char *szHeader = NULL);

    // ��ӡ����
    void PrintBottom(const char *szBottom = NULL);
    
    // ��ӡ����
    void PrintBlank(int lines = 1);

    // ��ӡ�˵�
    void PrintMenu();

    // ��ӡˮƽ��
    void PrintHorizen(int lines = 1);

    // ��ӡ��ˮƽ��
    void PrintShortHorizen();

    // ���ؿ��
    const int GetWidth() const {return m_Width;}

private:

    // ����ָ������
    LinkedList<MenuOptions>     m_Options;

    // �Ӳ˵�����
    LinkedList<Menu*>           m_SubMenus;

    // ѡ��ID����
    LinkedList<int>             m_IndexOp;

    // �Ӳ˵�ID����
    LinkedList<int>             m_IndexSub;

    // �˵���������
    LinkedList<MENU_TYPE>       m_Type;

    // �˵�ѡ������
    LinkedList<const char *>    m_Comment;

    // �˵�����
    const char                 *m_Title;

    // �˵��ײ�����
    const char                 *m_Bottom;

    // ���˵�ָ��
    Menu                       *m_Parent;

    // �ܲ˵���
    int                         m_Counts;

    // ���
    const int                   m_Width;

    // �ָ������
    const int                   m_Horizen;
};
