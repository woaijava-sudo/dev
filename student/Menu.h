#pragma once
#include "List.h"
#include "StuInfo.h"

typedef void (*MenuOptions)(int);

/*
Menu: 菜单类
  */
class Menu
{
public:
    // 菜单类型枚举
    enum MENU_TYPE
    {
        MT_OPT,
        MT_SUB,
    };

public:
    /* 构造函数
       第一个参数：菜单标题
       第二个参数：父菜单指针（若无则为NULL）
    */
    Menu(const char *szTitle = "菜单", const char *szBottom = "", Menu *pParent = NULL):
      m_Counts(0), m_Parent(pParent), m_Width(76), m_Horizen(60),
      m_Title(szTitle == NULL ? "菜单" : szTitle),
      m_Bottom(szBottom == NULL ? "" : szBottom){}
    ~Menu();

    /* 增加选项
       第一个参数：选项文字
       第二个参数：选项相应的函数指针
       返回值：选项ID
    */ 
    int AppendOption(const char *, MenuOptions);

    /* 增加子菜单
       第一个参数：子菜单标题
       返回值：菜单ID
    */
    int AppendMenu(const char *);

    // 得到子菜单的ID
    Menu& GetMenuById(int);

    // 得到选项的函数指针
    MenuOptions GetOptionById(int);

    // 得到总的菜单数
    const int GetMenuCounts() const {return m_Counts;}

    // 得到子菜单（选项）类型
    MENU_TYPE GetTypeById(int ID) {return m_Type.At(ID);}

    // 用户输入判断
    int MenuLoop(char);

    // 显示子菜单信息
    void DisplayComment();

    // 循环判断开始
    void Run();

    // 清屏
    void Cls() {system("cls");}

    // 暂停
    void Pause() {system("pause");}

    // 打印界面头
    void PrintHeader(const char *szHeader = NULL);

    // 打印界面
    void PrintBottom(const char *szBottom = NULL);
    
    // 打印空行
    void PrintBlank(int lines = 1);

    // 打印菜单
    void PrintMenu();

    // 打印水平线
    void PrintHorizen(int lines = 1);

    // 打印短水平线
    void PrintShortHorizen();

    // 返回宽度
    const int GetWidth() const {return m_Width;}

private:

    // 函数指针链表
    LinkedList<MenuOptions>     m_Options;

    // 子菜单链表
    LinkedList<Menu*>           m_SubMenus;

    // 选项ID索引
    LinkedList<int>             m_IndexOp;

    // 子菜单ID索引
    LinkedList<int>             m_IndexSub;

    // 菜单类型索引
    LinkedList<MENU_TYPE>       m_Type;

    // 菜单选项文字
    LinkedList<const char *>    m_Comment;

    // 菜单标题
    const char                 *m_Title;

    // 菜单底部署名
    const char                 *m_Bottom;

    // 父菜单指针
    Menu                       *m_Parent;

    // 总菜单数
    int                         m_Counts;

    // 宽度
    const int                   m_Width;

    // 分隔符宽度
    const int                   m_Horizen;
};
