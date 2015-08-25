#ifndef COLOR_H
#define COLOR_H

#define AXIS 100
//以对应256*8种颜色为正负
//FF0000-g增加-FFFF00-r减少-00FF00-b增加-00FFFF-g减少 |||-0000FF-r增加-FF00FF-b减少-FF0000
//    0           256         256          256       |||    256          256          256
//256*3=768
//
//
//赤橙黄绿蓝靛紫
/*
赤 R:255 G:0 B:0 
橙 R:245 G:124 B:32 
黄 R:255 G:255 B:0 
绿 R:0 G:255 B:0 
蓝 R:255 G:255 B:255 
靛 R:255 G:0 B:255 
紫 R:255 G:0 B:255
*/
/*
000000
0000FF
00FFFF
00FF00
7FFF00 *0
FFFF00
FF0000
FF00FF
FFFFFF
0,1,3,2,6,7,5,4
*/
COLORREF INT2RGB_Convert(float depth)
{
    int deep = (int)(depth * AXIS);
    if(abs(deep) <= 7*0x100/2)
    {
        deep += 7*0x100/2;
        int a = deep % 0x100;
        int b = deep / 0x100;
        switch(a)
        {
            case 0: return b;
            case 1: return (0x0000FF | (b<<8));
            case 2: return (0x00FF00 | (0xFF-b));
            case 3: return (0x00FF00 | (b<<16));
            case 4: return (0xFFFF00 | b);
            case 5: return (0xFF00FF | (0xFF-b)<<8);
            case 6: return (0xFF0000 | (0xFF-b));
        }
    }
    else
    {
        if(deep > 0)
            return 0xFF0000;
        else
            return 0;
    }
    return (COLORREF)deep;
}

#endif