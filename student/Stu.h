#pragma once
#include "Menu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// IStu 抽象类接口
class IStu
{
public:
    IStu(){};
    virtual ~IStu(){};

public:
    virtual void Run() = 0;
    virtual void Pause() = 0;
    virtual void Cls() = 0;

    virtual bool Load() = 0;
    virtual bool Save() = 0;
    virtual void Print(LinkedList<STUINFO>* pData = NULL, int Index = -1, bool isOptions  = true) = 0;
    virtual void View() = 0;
    virtual void Add() = 0;
    virtual void Search(int, string&) = 0;
    virtual void Delete(int, string&) = 0;
    virtual void Update(int, int) = 0;
    virtual void Sort(int, bool bGreater = true) = 0;
};

// StuApp 实现类
class StuApp : public IStu
{
public:
    StuApp(const char *szTitle = NULL, const char *szPS = NULL);
    ~StuApp();

public:
    void Run();

public:
    void Cls() {m_Menu.Cls();}
    void Pause() {m_Menu.Pause();}
    bool Load();
    bool Save();
    void Print(LinkedList<STUINFO>* pData = NULL, int Index = -1, bool isOptions  = true); // 打印信息
    void View(); // 查看结构
    void Add();
    void Search(int, string&);
    void Delete(int, string&);
    void Update(int, int); // 编辑更新
    void Sort(int, bool bGreater = true); // 排序

public:
    // 静态函数（菜单选项）
    static void LoadFromFile(int);
    static void SaveToFile(int);
    static void ViewStructure(int); // 查看结构
    static void PrintInfo(int); // 逐条打印
    static void AppendPerson(int); // 添加记录
    static void SearchCallByIndex(int); // 调用查找函数
    static void DeleteCallByIndex(int); // 调用删除函数
    static void SortCallByIndex(int); // 调用排序函数
    static void EditPerson(int); // 调用编辑函数



protected:
    void GetInfoIndex(STUINFO*, string**); // 得到结构体各成员索引

protected:
    // 学生信息
    LinkedList<STUINFO>  m_Data;

    // 菜单对象
    Menu                 m_Menu;
};

void CreateStuApp(IStu **i);

