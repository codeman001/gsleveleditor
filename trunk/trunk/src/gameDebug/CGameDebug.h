#ifndef _GAMEDEBUG_
#define _GAMEDEBUG_

#include "stdafx.h"

#define MAX_DEBUGGEOMETRY	2000

class CGameDebug: public uiClass::uiSingleton<CGameDebug>
{
public:
	struct SLineDebug
	{
		core::line3df line;
		SColor color;
	};

	struct SBoxDebug
	{
		core::aabbox3df box;
		SColor color;
	};

protected:
	SLineDebug	m_lines[MAX_DEBUGGEOMETRY];
	SBoxDebug	m_boxs[MAX_DEBUGGEOMETRY];

	int			m_nLine;
	int			m_nBox;

public:
	CGameDebug();
	virtual ~CGameDebug();

	void addDrawLine( core::line3df line, SColor color );
	void addBoudingBox( core::aabbox3df box, SColor color );

	inline void clearLines()
	{
		m_nLine = 0;
	}

	inline void clearBoxs()
	{
		m_nBox = 0;
	}

	inline int getLinesCount()
	{
		return m_nLine;
	}

	inline int getBoxCount()
	{
		return m_nBox;
	}

	inline const SLineDebug& getLine(int i)
	{
		return m_lines[i];
	}

	inline const SBoxDebug& getBox(int i)
	{
		return m_boxs[i];
	}

};

#endif