#pragma once
#include <string>
#include <IOSTREAM>

using namespace std;

/*
STUINFO: 学生信息结构体
  */

struct STUINFO
{
    STUINFO(){}
    STUINFO(
        string _szID,
        string _szName,
        string _szSex,
        string _szBirth,
        string _szMajor,
        string _szClass,
        string _szAddress,
        string _szPhone)
        :szID(_szID),
        szName(_szName),
        szSex(_szSex),
        szBirth(_szBirth),
        szMajor(_szMajor),
        szClass(_szClass),
        szAddress(_szAddress),
        szPhone(_szPhone){}
    string   szID;
    string   szName;
    string   szSex;
    string   szBirth;
    string   szMajor;
    string   szClass;
    string   szAddress;
    string   szPhone;
};


/*
STUINFO: 学生信息比较 结构体
  */

struct STUCOMP
{
    STUCOMP():szInfo(NULL), ID(0){}
    STUCOMP(string *info, int id = 0):szInfo(info), ID(id){}
    string  *szInfo;
    int      ID;

    friend const bool operator == (const STUCOMP &c1, const STUCOMP &c2) {return *(c1.szInfo) == *(c2.szInfo);}
    friend const bool operator >= (const STUCOMP &c1, const STUCOMP &c2) {return *(c1.szInfo) >= *(c2.szInfo);}
    friend const bool operator <= (const STUCOMP &c1, const STUCOMP &c2) {return *(c1.szInfo) <= *(c2.szInfo);}
    friend const bool operator > (const STUCOMP &c1, const STUCOMP &c2) {return *(c1.szInfo) > *(c2.szInfo);}
    friend const bool operator < (const STUCOMP &c1, const STUCOMP &c2) {return *(c1.szInfo) < *(c2.szInfo);}
};

