#pragma once
#include "Menu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// IStu ������ӿ�
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

// StuApp ʵ����
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
    void Print(LinkedList<STUINFO>* pData = NULL, int Index = -1, bool isOptions  = true); // ��ӡ��Ϣ
    void View(); // �鿴�ṹ
    void Add();
    void Search(int, string&);
    void Delete(int, string&);
    void Update(int, int); // �༭����
    void Sort(int, bool bGreater = true); // ����

public:
    // ��̬�������˵�ѡ�
    static void LoadFromFile(int);
    static void SaveToFile(int);
    static void ViewStructure(int); // �鿴�ṹ
    static void PrintInfo(int); // ������ӡ
    static void AppendPerson(int); // ��Ӽ�¼
    static void SearchCallByIndex(int); // ���ò��Һ���
    static void DeleteCallByIndex(int); // ����ɾ������
    static void SortCallByIndex(int); // ����������
    static void EditPerson(int); // ���ñ༭����



protected:
    void GetInfoIndex(STUINFO*, string**); // �õ��ṹ�����Ա����

protected:
    // ѧ����Ϣ
    LinkedList<STUINFO>  m_Data;

    // �˵�����
    Menu                 m_Menu;
};

void CreateStuApp(IStu **i);

