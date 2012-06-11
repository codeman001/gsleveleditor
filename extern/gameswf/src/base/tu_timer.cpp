#include "stdafx.h"
// tu_timer.cpp	-- by Thatcher Ulrich <tu@tulrich.com>

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Utility/profiling timer.
#ifndef _PS3
#include <ctime>	// [ANSI/System V]
using namespace std;
#else
#include <time.h>	// [ANSI/System V]
#endif
#include "base/tu_timer.h"
#include "base/utility.h"
#include "base/container.h"

namespace gameswf
{

Uint64 tu_timer::get_systime()
// Returns the time as seconds elapsed since midnight, January 1, 1970.
{
	time_t ltime;
	time(&ltime);
	return ltime;
}

int tu_timer::get_date(Uint64 t)
// Returns the day of the month (an integer from 1 to 31)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_mday;
}

int tu_timer::get_day(Uint64 t)
// Returns the day of the week (0 for Sunday, 1 for Monday, and so on)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_wday;
}

int tu_timer::get_hours(Uint64 t)
// Returns the hour (an integer from 0 to 23)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_hour;
}

int tu_timer::get_fullyear(Uint64 t)
// Returns the full year (a four-digit number, such as 2000)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_year + 1900;
}

int tu_timer::get_milli(Uint64 t)
// Returns the milliseconds (an integer from 0 to 999)
{
	return 0;	// TODO
}

int tu_timer::get_month(Uint64 t)
// Returns the month (0 for January, 1 for February, and so on)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_mon;
}

int tu_timer::get_minutes(Uint64 t)
// Returns the minutes (an integer from 0 to 59)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_min;
}

int tu_timer::get_seconds(Uint64 t)
// Returns the seconds (an integer from 0 to 59)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_sec;
}

int tu_timer::get_year(Uint64 t)
// Returns the seconds (an integer from 0 to 59)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_year;
}

Uint64 tu_timer::get_time(Uint64 t)
// Returns the number of milliseconds since midnight January 1, 1970, universal time
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return t * 1000;	// TODO: add milliseconds
}


#if defined(WIN32) || defined(_XBOX)

#if defined(WIN32)
#include <windows.h>
#include <Mmsystem.h>
#elif defined(_XBOX)
#include <xtl.h>
#endif


uint64 tu_timer::get_ticks()
{

	#if defined(WIN32)
		return timeGetTime();
	#elif defined(_XBOX)
		return GetTickCount();
	#elif defined(_PS3)
		return clock() / (CLOCKS_PER_SEC / 1000);
    #elif defined(_LINUX)
		return clock() / (CLOCKS_PER_SEC / 1000);
	#endif
}


double tu_timer::ticks_to_seconds(uint64 ticks)
{
	return ticks * (1.0f / 1000.f);
}

double tu_timer::ticks_to_seconds()
{
	return get_ticks() * (1.0f / 1000.f);
}


void tu_timer::sleep(int milliseconds)
{
	Sleep(milliseconds);
}


uint64	tu_timer::get_profile_ticks()
{
	// @@ use rdtsc?

	LARGE_INTEGER	li;
	QueryPerformanceCounter(&li);

	return li.QuadPart;
}


double	tu_timer::profile_ticks_to_seconds(uint64 ticks)
{
	LARGE_INTEGER	freq;
	QueryPerformanceFrequency(&freq);

	double	seconds = (double) ticks;
	seconds /= (double) freq.QuadPart;

	return seconds;
}

#elif defined RVL_OS

uint64 tu_timer::get_ticks()
{
	return OSGetTime();
}


double tu_timer::ticks_to_seconds(uint64 ticks)
{
	return ticks * (1.0f / 1000.f);
}

double tu_timer::ticks_to_seconds()
{
	return get_ticks() * (1.0f / 1000.f);
}


void tu_timer::sleep(int milliseconds)
{
	//::Sleep(milliseconds);
}


uint64	tu_timer::get_profile_ticks()
{
	return OSGetTime();
}


double	tu_timer::profile_ticks_to_seconds(uint64 ticks)
{
	return get_profile_ticks() * (1.0f / 1000.f);
}

#else	// not WIN32


#include <sys/time.h>
#ifndef _PS3
#include <unistd.h>
#endif

// The profile ticks implementation is just fine for a normal timer.


uint64 tu_timer::get_ticks()
{
#if defined(_PS3)
	return clock() / (CLOCKS_PER_SEC / 1000);
#else
	timeval tv;
	gettimeofday(&tv, 0);
	return (uint64)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
}


double tu_timer::ticks_to_seconds(uint64 ticks)
{
	return profile_ticks_to_seconds(ticks);
}


void tu_timer::sleep(int milliseconds)
{
	sleep(milliseconds * 1000);
}


uint64	tu_timer::get_profile_ticks()
{
	// @@ TODO prefer rdtsc when available?

	// Return microseconds.
	//struct timeval tv;
	//uint64 result;
	//
	//gettimeofday(&tv, 0);

	//result = tv.tv_sec * 1000000;
	//result += tv.tv_usec;
	//
	//return result;

	return 0;
}


double	tu_timer::profile_ticks_to_seconds(uint64 ticks)
{
	// ticks is microseconds.  Convert to seconds.
	return ticks / 1000000.0;
}

double	tu_timer::profile_ticks_to_milliseconds(uint64 ticks)
{
	// ticks is microseconds.  Convert to milliseconds.
	return ticks / 1000.0;
}

#endif	// not WIN32

namespace profiler
{
#if GAMESWF_ENABLE_PROFILER

	#define PROFILER_COMPUTE_SUM	0

	#define PROFILER_MAX_EVENTS			(2048*4)
	#define PROFILER_MAX_STACK			256
	#define PROFILER_MAX_EXPORTBUFFER	1024
	#define PROFILER_EXPORT_THRESHOLD	0 // event time >= PROFILER_EXPORT_THRESHOLD (ms)
	#define PROFILER_EXPORT_DEPTH		20 // event depth < PROFILER_EXPORT_DEPTH

	typedef struct ProfilerEvent
	{
		char name[32];
		uint64 begin;
		uint64 end;
		short depth;
	} ProfilerEvent;

	ProfilerEvent m_profilerEvents[PROFILER_MAX_EVENTS];

	short	m_profiler_eventStack[PROFILER_MAX_STACK];
	int		m_profiler_eventCount = 0;
	int		m_profiler_eventStackIndex = 0;
	bool	m_profiler_recording = false;
	bool	m_profiler_enabled = false;

	void set_enabled(bool enabled)
	{
		m_profiler_enabled = enabled;
	}

	// Mark the beginning of an event.
	void begin_event(const char* name)
	{
		if(!m_profiler_enabled)
			return;

		if(!m_profiler_recording)
			m_profiler_recording = true;

		if(m_profiler_recording)
		{
			assert(m_profiler_eventCount < PROFILER_MAX_EVENTS);
			assert(m_profiler_eventStackIndex < PROFILER_MAX_STACK);
			
			if(m_profiler_eventCount < PROFILER_MAX_EVENTS)
			{
				ProfilerEvent& event = m_profilerEvents[m_profiler_eventCount];

				int size = imin(strlen(name), 30);
				memcpy(event.name, name, size);
				event.name[size] = '\0';
				event.begin = tu_timer::get_profile_ticks();
				event.end = event.begin;
				event.depth = (short)m_profiler_eventStackIndex;
				
				m_profiler_eventStack[m_profiler_eventStackIndex] = (short)m_profiler_eventCount;
				m_profiler_eventCount++;
				m_profiler_eventStackIndex++;
			}
			else
			{
				dump();
			}
		}
	}

	// Mark the end of an event.
	void end_event()
	{
		if(!m_profiler_enabled)
			return;

		if(m_profiler_recording)
		{
			m_profiler_eventStackIndex--;

			assert(m_profiler_eventStackIndex >= 0 && m_profiler_eventStackIndex < PROFILER_MAX_EVENTS);

			m_profilerEvents[m_profiler_eventStack[m_profiler_eventStackIndex]].end = tu_timer::get_profile_ticks();

			if(m_profiler_eventStackIndex == 0)
			{
				m_profiler_recording = false;
				dump();
			}
		}
	}

	// dump
	void dump()
	{
		if(!m_profiler_enabled || m_profiler_eventCount == 0)
			return;

		uint64 totalTime = (m_profiler_eventCount > 0 ? m_profilerEvents[0].end - m_profilerEvents[0].begin : 0);

#if PROFILER_COMPUTE_SUM
		hash<const char*, uint64> sum;
		hash<const char*, int> calls;

		for(int i = 0; i < m_profiler_eventCount; i++)
		{
			ProfilerEvent& event = m_profilerEvents[i];
			sum[event.name] += (int)(event.end - event.begin);
			calls[event.name]++;
		}
#endif // PROFILER_COMPUTE_SUM

		char buffer[512];

		for(int i = 0; i < m_profiler_eventCount; i++)
		{
			ProfilerEvent& event = m_profilerEvents[i];

			if(event.depth >= PROFILER_EXPORT_DEPTH)
				continue;

			// indentation
			//int x = event.depth * 20;

			uint64 time = (event.end - event.begin);
			int call = 1;

#if PROFILER_COMPUTE_SUM
			call = 0;
			time = 0;
			if(!sum.get(event.name, &time) || time == 0)
				continue;
			sum[event.name] = 0;
			if(!calls.get(event.name, &call) || call == 0)
				continue;
#endif

			float time_ms = (float)tu_timer::profile_ticks_to_seconds(time) * 1000;
			if(time_ms >= PROFILER_EXPORT_THRESHOLD)
			{
				float totalPercent = (float)(totalTime > 0 ? 100.0f*(double)time/totalTime : 0);

				buffer[0] = '\0';
				for(int i = 0; i < event.depth; i++)
					strcat(buffer, "   ");

#if PROFILER_COMPUTE_SUM
				sprintf(buffer + strlen(buffer), "%s:%.2f (%.1f%%) %dx", event.name, time_ms, totalPercent, call);
#else
				sprintf(buffer + strlen(buffer), "%s:%.2f (%.1f%%)", event.name, time_ms, totalPercent);
#endif
				assert(strlen(buffer) < sizeof(buffer));

//#if GAMESWF_ENABLE_DEBUGGER
//				debugger::profile_trace(buffer);
//#else
				printf("%s\n", buffer);
//#endif
			}
		}
	}
#else
	void set_enabled(bool enabled) {}
	void begin_event(const char* name) {}
	void end_event() {}
	void dump() {}
#endif
}; // namespace profiler

}; // namespace gameswf

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
