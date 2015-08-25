#ifndef TIMER_H
#define TIMER_H

#include "stdafx.h"
#include <windows.h>

class Timer
{
private:

	LONGLONG	m_CurrentTime,
	            m_LastTime,
				m_NextTime,
				m_FrameTime,
				m_PerfCountFreq;

	double		m_TimeElapsed,
				m_TimeScale;

	float		m_FPS;

public:

	Timer();
	Timer(float fps);

    void    Start();
    bool    ReadyForNextFrame();
	double	GetTimeElapsed(){return m_TimeElapsed;}
	double	TimeElapsed();
};

#endif

	
