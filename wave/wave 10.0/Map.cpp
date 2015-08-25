#include "stdafx.h"
#include "Map.h"

void Map::Init(int NumCellsX, int NumCellsY, double CellSize, double depth)
{
    m_dCellSize = CellSize;
    m_NumCellsX = NumCellsX;
    m_NumCellsY = NumCellsY;
    vector<Cell> temp;
    int x, y;
    for(x=0; x<m_NumCellsX; ++x)
    {
        temp.clear();
        for (y=0; y<m_NumCellsY; ++y)
            temp.push_back(Cell(x*m_dCellSize, (x+1)*m_dCellSize, y*m_dCellSize, (y+1)*m_dCellSize, 
#if OP == 1
                (((x+y)%2)<<1-1)*
#endif
                    depth));
        m_2DCells.push_back(temp);
    }
    
    for(x=0; x<m_NumCellsX; ++x)
    {
        m_2DCells[x][m_NumCellsY-1].depth = 
        m_2DCells[x][0].depth = BETHA;
    }

    for(y=1; y<m_NumCellsY-1; ++y)
    {
        m_2DCells[m_NumCellsX-1][y].depth = 
        m_2DCells[0][y].depth = BETHA;
    }

	srand(time(NULL));
}

void Map::Render(HDC surface)
{
    HBRUSH brush;
    for (int x=0; x<m_NumCellsX; ++x)
    {
        for (int y=0; y<m_NumCellsY; ++y)
        {
            brush = CreateSolidBrush(INT2RGB_Convert((m_2DCells[x][y].depth)));
            FillRect(surface, &m_2DCells[x][y].cell, brush);
            DeleteObject(brush);
        }
    }
}

void Map::Reset()
{
    for (int x=0; x<m_NumCellsX; ++x)
        for (int y=0; y<m_NumCellsY; ++y)
            m_2DCells[x][y].Reset();
}

void Map::Update()
{
    int x, y;
    for (x=1; x<m_NumCellsX-1; ++x)
        for (y=1; y<m_NumCellsY-1; ++y)
            m_2DCells[x][y].depth = m_2DCells[x][y].depth2;

/*
m_2DCells[70][30].buf = 1e44;
m_2DCells[36][21].buf = ERT;
m_2DCells[53][22].buf = -ERT*1e7;
m_2DCells[42][12].buf = -ERT;
//m_2DCells[152][38].buf *= -4;
m_2DCells[100][20].buf *= -1e40;
m_2DCells[72][24].buf = -ERT;
m_2DCells[62][27].buf = ERT;
m_2DCells[93][30].buf = -ERT;
m_2DCells[80][34].buf = ERT*1e7;
*/

    for (x=1; x<m_NumCellsX-1; ++x)
        for (y=1; y<m_NumCellsY-1; ++y)
        {
            /*AutoComputeValue(m_2DCells[x][y], m_2DCells[x+1][y]);
            AutoComputeValue(m_2DCells[x][y], m_2DCells[x-1][y]);
            AutoComputeValue(m_2DCells[x][y], m_2DCells[x][y+1]);
            AutoComputeValue(m_2DCells[x][y], m_2DCells[x][y-1]);*/

			double avg = m_2DCells[x + 1][y].depth +
				m_2DCells[x - 1][y].depth +
				m_2DCells[x][y + 1].depth +
				m_2DCells[x][y - 1].depth;
			avg *= 0.25;
			m_2DCells[x][y].buf += (avg - m_2DCells[x][y].depth)*2.0;
			m_2DCells[x][y].buf *= 0.99 + (rand() % 5) / 1000.0;
			m_2DCells[x][y].depth2 += m_2DCells[x][y].buf;
			/*if (m_2DCells[x][y].depth2 != 0.0)
			{
				__asm int 3;
			}*/

#if ANOTH == 1
            AutoComputeValue(m_2DCells[x][y], m_2DCells[x-1][y-1]);
            AutoComputeValue(m_2DCells[x][y], m_2DCells[x-1][y+1]);
            AutoComputeValue(m_2DCells[x][y], m_2DCells[x+1][y-1]);
            AutoComputeValue(m_2DCells[x][y], m_2DCells[x+1][y+1]);
#endif
        }
}

void Map::SetCellDepth(int CellX, int CellY, double depth)
{
    if(CellX<1 || CellX>m_NumCellsX || CellY<1 || CellY>m_NumCellsY)
        return;
    m_2DCells[CellX][CellY].Set(depth);
}

COLORREF Map::INT2RGB_Convert(double  depth)
{
    int deep = (int)(depth * AXIS);
    if(abs(deep) <= MAXLEN)
    {
        deep += MAXLEN;
        //deep %= 0x400;
        int a = deep / 0xFF;
        int b = deep % 0xFF + 1;
        switch(a)
        {
            case 0: return b;                          //0000XX
            case 1: return (0x0000FF | (b<<8));        //00XXXX
            case 2: return (0x00FF00 | (0xFF-b));      //00XX00
            case 3: return (0x00FF00 | (b<<16));       //XXXX00
            case 4: return (0xFFFF00 | b);             //XXXXXX
            case 5: return (0xFF00FF | (0xFF-b)<<8);   //XX00XX
            case 6: return (0xFF0000 | (0xFF-b));      //XX0000
            case 7: return (0xFF-b)<<16;               //000000
        }
    }
    else
    {
        if(deep>0)
            return 0xFFFFFF;
        else
            return 0;
    }
    return deep;
}

void Map::AutoComputeValue(Cell& center, Cell& edge)
{
    static double temp;
    if(fabs(center.buf-edge.buf) < 1e-4)
        return;
#if MODE == 1
    temp = (center.buf-edge.buf)/ZEBRA;
#else
    temp = rand()%2 * (center.buf-edge.buf)/ (ZEBRA + 3*(rand()%2 - 0.5));
        //temp = atan(center.buf-edge.buf)*84;
#endif
    if(fabs(temp) > CAST)
    {
        if(temp>0)
            temp = DEST;
        else
            temp = -DEST;
    }
    center.depth -= temp;
    edge.depth += temp;
    center.depth /= 1.01;
    edge.depth /= 1.01;
}
