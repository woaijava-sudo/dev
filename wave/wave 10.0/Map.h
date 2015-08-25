#ifndef MAP_H
#define MAP_H

#include <vector>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define FPS 600
#define NUMBERS_X 180
#define NUMBERS_Y 92
#define CUBE_SIZE 7.5000
//#define NUMBERS_X 360
//#define NUMBERS_Y 184
//#define CUBE_SIZE 3.75000
#define AXIS 1
#define SECURE 0.001
#define INIT_DEPTH 0
#define INIT_DOT_X NUMBERS_X/2
#define INIT_DOT_Y NUMBERS_Y/2
#define INIT_DOT_D 0x800
#define BETHA 0//INIT_DEPTH //边框数值
#define ZEBRA 7.800//7.999
#define OP 0//1为正负 0 为非
#define SETUP 0//1为中心放点0不放
#define ANOTH 0//1为8处理 0为4
#define SECONDANCE 1//1为二次刷新处理
#define CAST 1e150 //最大范围
#define DEST -CAST //二次取值
#define MODE 0//1原始0 rand()
#define MAXLEN 0x400//0x400
#define ERT 1e5 

using namespace std;

struct Cell
{
    double depth, depth2, buf;
    RECT cell;
    Cell(int xmin, int xmax, int ymin, int ymax, double _depth):
        depth(_depth),
		depth2(_depth),
        buf(_depth)
    {
        cell.left   = xmin;
        cell.right  = xmax;
        cell.top    = ymin;
        cell.bottom = ymax;
    }

    void Reset()
    {
        depth = 0.0;
		depth2 = 0.0;
        buf = 0.0;
    }

    void Set(double _depth)
    {
		depth2 = _depth;
    }
};

class Map
{
private:
    vector<vector<Cell> > m_2DCells;

    int     m_NumCellsX;
    int     m_NumCellsY;

    double  m_dCellSize;
    COLORREF INT2RGB_Convert(double depth);
    void AutoComputeValue(Cell& center, Cell& edge);

public:
    Map():
            m_NumCellsX(0),
            m_NumCellsY(0),
            m_dCellSize(0.0)
    {}

    void    Init(int NumCellsX, int NumCellsY, double CellSize, double depth);
    void    Render(HDC surface);
    void    Reset();
    void    Update();
    void    SetCellDepth(int CellX, int CellY, double depth);
};
#endif