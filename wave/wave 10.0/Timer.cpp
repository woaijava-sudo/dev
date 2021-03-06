#include "stdafx.h"
#include "Timer.h"

Timer::Timer():           m_FPS(0),
				          m_TimeElapsed(0.0f),
				          m_FrameTime(0),
				          m_LastTime(0),
				          m_PerfCountFreq(0)
{
	QueryPerformanceFrequency( (LARGE_INTEGER*) &m_PerfCountFreq);
	m_TimeScale = 1.0f/m_PerfCountFreq;
}

Timer::Timer(float fps): m_FPS(fps),
						 m_TimeElapsed(0.0f),
						 m_LastTime(0),
						 m_PerfCountFreq(0)
{
	QueryPerformanceFrequency( (LARGE_INTEGER*) &m_PerfCountFreq);
	m_TimeScale = 1.0f/m_PerfCountFreq;
	m_FrameTime = (LONGLONG)(m_PerfCountFreq / m_FPS);
}

void Timer::Start()
{
	QueryPerformanceCounter( (LARGE_INTEGER*) &m_LastTime);
	m_NextTime = m_LastTime + m_FrameTime;
}

bool Timer::ReadyForNextFrame()
{
    if (!m_FPS)
    {
      MessageBox(NULL, "No FPS set in timer", "Doh!", 0);
      return false;
    }
  
    QueryPerformanceCounter( (LARGE_INTEGER*) &m_CurrentTime);
	if (m_CurrentTime > m_NextTime)
	{

		m_TimeElapsed	= (m_CurrentTime - m_LastTime) * m_TimeScale;
		m_LastTime		= m_CurrentTime;
		m_NextTime = m_CurrentTime + m_FrameTime;
		return true;
	}
	return false;
}

double Timer::TimeElapsed()
{
	QueryPerformanceCounter( (LARGE_INTEGER*) &m_CurrentTime);
	m_TimeElapsed	= (m_CurrentTime - m_LastTime) * m_TimeScale;
	m_LastTime		= m_CurrentTime;
	return m_TimeElapsed;
}