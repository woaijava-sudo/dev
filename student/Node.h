#pragma once
#define NULL 0
#include <ASSERT.H>

/*
LinkedNode<T>: 链表结点模板类
  */
template<class T>
class LinkedNode
{
public:
    LinkedNode();
    LinkedNode(const T &);
    LinkedNode(const LinkedNode &);
    void SetData(const T &);
    void SetNext(const LinkedNode &);
    void SetNextNull();
    void ReleaseData(); // 删除数据
    void ReleaseNext();
    void ReleaseNext(int ID);
    void ReleaseNextAll();
    T& GetData();
    const T& GetDataRef() const; // 得到数据的常引用
    LinkedNode& GetNext();
    LinkedNode& GetNext(int ID);
    LinkedNode& GetLast();
    bool HasNext() const;
    ~LinkedNode();
        
private:
    T               *m_Data;
    LinkedNode      *m_Next;
};

template<class T>
LinkedNode<T>::LinkedNode():
m_Data(NULL), m_Next(NULL)
{

}

template<class T>
LinkedNode<T>::LinkedNode(const T &t):
m_Data(new T(t)), m_Next(NULL)
{
    
}

template<class T>
LinkedNode<T>::LinkedNode(const LinkedNode &l)
{
    m_Data = new T(l.GetDataRef());
    if (!l.HasNext())
    {
        m_Next = NULL;
        return;
    }

    LinkedNode *pSrc = (LinkedNode*)&l;
    LinkedNode *pDest = (LinkedNode*)this;

    while(true)
    {
        pDest->SetNext(*(new LinkedNode(pSrc->GetDataRef())));
        if (!pSrc->HasNext())
        {
            break;
        }
        pSrc = &(pSrc->GetNext());
        pDest = &(pDest->GetNext());
    }
}

template<class T>
void LinkedNode<T>::SetData(const T &t)
{
    m_Data = new T(t);
}

template<class T>
void LinkedNode<T>::SetNext(const LinkedNode &l)
{
    m_Next = (LinkedNode*)&l;
}

template<class T>
void LinkedNode<T>::SetNextNull()
{
    m_Next = NULL;
}

template<class T>
void LinkedNode<T>::ReleaseData()
{
    if (m_Data != NULL)
    {
        delete m_Data;
        m_Data = NULL;
    }
}

template<class T>
void LinkedNode<T>::ReleaseNext()
{
    assert(m_Next != NULL || !"LinkedNode<T>::ReleaseNext() : 'm_Next' fault!");
    LinkedNode *p = &(m_Next->GetNext());
    delete p;
    p = NULL;
    m_Next = NULL;    
}

template<class T>
void LinkedNode<T>::ReleaseNext(int ID)
{
    assert(m_Next != NULL || !"LinkedNode<T>::ReleaseNext(int) : 'm_Next' fault!");
    assert(ID > 0 || !"LinkedNode<T>::ReleaseNext(int) : Parameter 'ID' fault!");
    LinkedNode *p = NULL;
    for (int i = 0; i < ID; i++)
    {
        p = &(m_Next->GetNext());
        assert(p != NULL || !"LinkedNode<T>::ReleaseNext(int) : Variable 'p' fault!");
    }
    delete p;
    p = NULL;
    m_Next = NULL;
}

template<class T>
void LinkedNode<T>::ReleaseNextAll()
{
    if (m_Next == NULL)
    {
        return;
    }
    LinkedNode *pCurr = m_Next;
    LinkedNode *pNext = NULL;
    while (true)
    {
        if (!pCurr->HasNext())
        {
            break;
        }
        else
        {
            pNext = &(pCurr->GetNext());
            delete pCurr;
            pCurr = pNext;
        }
    }
    delete pCurr;
    pCurr = NULL;
    pNext = NULL;
}

template<class T>
T& LinkedNode<T>::GetData()
{
    assert(m_Data != NULL || !"LinkedNode<T>::GetData() : 'm_Data' fault!");
    return (*m_Data);
}

template<class T>
const T& LinkedNode<T>::GetDataRef() const
{
    assert(m_Data != NULL || !"LinkedNode<T>::GetData() : 'm_Data' fault!");
    return (*m_Data);
}

template<class T>
LinkedNode<T>& LinkedNode<T>::GetNext()
{
    assert(m_Next != NULL || !"LinkedNode<T>::GetNext() : 'm_Next' fault!");
    return (*m_Next);
}

template<class T>
LinkedNode<T>& LinkedNode<T>::GetNext(int ID)
{
    assert(ID >= 0 || !"LinkedNode<T>::GetNext(int) : Parameter 'ID' fault!");
    if (ID == 0)
    {
        return (*this);
    }
    LinkedNode *p = (LinkedNode*)this;
    for (int i = 0; i < ID; i++)
    {
        p = &(p->GetNext());
        assert(p != NULL || !"LinkedNode<T>::GetNext(int) : Variable 'p' fault!");
    }
    return (*p);
}

template<class T>
LinkedNode<T>& LinkedNode<T>::GetLast()
{
    if (m_Next == NULL)
    {
        return (*this);
    }
    LinkedNode *p = (LinkedNode*)this;
    while(true)
    {
        if (p->HasNext())
        {
            p = &(p->GetNext());
        }
        else
        {
            break;
        }
    }
    return *p;
}

template<class T>
bool LinkedNode<T>::HasNext() const
{
    if (m_Next != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template<class T>
LinkedNode<T>::~LinkedNode()
{
    ReleaseData();
    m_Next = NULL;
}