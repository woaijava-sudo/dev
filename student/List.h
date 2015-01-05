#pragma once
#include "Node.h"


template<class T, class Comp>
const bool TCompare(const T& t1, const T& t2, Comp comp)
{
    return comp(t1, t2);
}

// Greater 仿函数
struct TGreater
{
    template<class T>
    bool operator()(T const& t1,T const& t2)
    {
        return t1 > t2;
    }
};


/*
LinkedList<T>: 链表模板类
  */
template<class T>
class LinkedList
{
public:
    LinkedList();
    LinkedList(const LinkedList &);
    ~LinkedList();

    // 从小到大排列： 如果第一参数比第二参数大，则应返回真
	//typedef bool (*ListCmpFunc)(const T&, const T&);
    //const bool ListCmp(const T& t1, const T& t2, ListCmpFunc cmp) const {return cmp(t1, t2);}

    void        Append(const T&);
    void        Insert(const T&, bool bGreater = true); // 按顺序插入
    void        Delete(int);
    void        Clear(); // 清空
    const int   Size() const; // 返回链表结点个数
    const bool  Empty() const;
    const T&    AtRef(int); // 返回常引用
    T&          At(int);    // 得到第n个结点的引用

    // 赋值重载
    LinkedList& operator = (LinkedList &);
    
private:
    LinkedNode<T>   *m_pHead;
    int              m_iCounts;
};

template<class T>
LinkedList<T>::LinkedList():
m_pHead(NULL), m_iCounts(0)
{
   
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList &l)
{
    m_iCounts = 0;
    for (int i = 0; i < l.Size(); i++)
    {
        Append(l.AtRef(i));
    }
}

template<class T>
LinkedList<T>::~LinkedList()
{
    Clear();
}

template<class T>
void LinkedList<T>::Append(const T &t)
{
    if (m_pHead == NULL)
    {
        m_pHead = new LinkedNode<T>(t);
        assert(m_pHead != NULL && "LinkedList<T>::Append(const T &) : operator new failed!");
    }
    else
    {
        LinkedNode<T> *p = new LinkedNode<T>(t);
        assert(p != NULL && "LinkedList<T>::Append(const T &) : operator new failed!");
        m_pHead->GetLast().SetNext(*p);
    }
    m_iCounts++;
}

template<class T>
void LinkedList<T>::Insert(const T& t, bool bGreater /* = true */)
{
    if (m_pHead == NULL)
    {
        Append(t);
        return;
    }
    LinkedNode<T> *p = m_pHead;
    bool bRet = false;
    while (true)
    {
        if (bGreater)
        {
            bRet = TCompare(p->GetDataRef(), t, TGreater());
        }
        else
        {
            bRet = !(TCompare(p->GetDataRef(), t, TGreater()));
        }
        
        if (bRet)
        {
            LinkedNode<T> *newp = new LinkedNode<T>;
            newp->SetData(p->GetDataRef());
            if (p->HasNext())
            {
                newp->SetNext(p->GetNext());
            }
            else
            {
                newp->SetNextNull();
            }
            p->SetData(t);
            p->SetNext(*newp);
            m_iCounts++;
            break;
        }
        
        if (p->HasNext())
        {
            p = &p->GetNext();
        }
        else
        {
            Append(t);
            break;
        }
    }
}

template<class T>
void LinkedList<T>::Delete(int ID)
{
    assert(ID >= 0 && ID < m_iCounts && "LinkedList<T>::Delete(int) : Parameter 'ID' fault!");
	assert(m_pHead != NULL || !"LinkedList<T>::Delete(int) : 'm_pHead' fault!");
	if (ID == 0)
	{
        LinkedNode<T> *p = m_pHead;
        if (!p->HasNext())
        {
            p->ReleaseData();
            p = NULL;
        }
        else
        {
            m_pHead = &(p->GetNext());
            delete p;
            p = NULL;
        }
        return;
	}
    m_pHead->ReleaseNext(ID);	
}

template<class T>
void LinkedList<T>::Clear()
{
    if (m_pHead != NULL)
    {
        m_pHead->ReleaseNextAll();
        delete m_pHead;
        m_pHead = NULL;
    }
    m_iCounts = 0;
}

template<class T>
const int LinkedList<T>::Size() const
{
    return m_iCounts;
}

template<class T>
const bool LinkedList<T>::Empty() const
{
    if (Size() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template<class T>
const T& LinkedList<T>::AtRef(int ID)
{
    assert(ID >= 0 && ID < m_iCounts || !"LinkedList<T>::Delete(int) : Parameter 'ID' fault!");
    return m_pHead->GetNext(ID).GetDataRef();
}

template<class T>
T& LinkedList<T>::At(int ID)
{
    assert(ID >= 0 && ID < m_iCounts || !"LinkedList<T>::Delete(int) : Parameter 'ID' fault!");
    return m_pHead->GetNext(ID).GetData();
}

template<class T>
LinkedList<T>& LinkedList<T>::operator = (LinkedList<T> &l)
{
    Clear();
    m_iCounts = 0;
    for (int i = 0; i < l.Size(); i++)
    {
        Append(l.AtRef(i));
    }
    return *this;
}