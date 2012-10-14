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

    struct S2DRectDebug
    {
        core::recti rect;
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
	S2DRectDebug m_2dRect[MAX_DEBUGGEOMETRY];
    
	int			m_nLine;
	int			m_nBox;
    int         m_n2DRect;
public:
	CGameDebug();
	virtual ~CGameDebug();

	void addDrawLine( core::line3df line, SColor color );
	void addBoudingBox( core::aabbox3df box, SColor color );
	void add2DRect( core::recti rect, SColor color );
    
	inline void clearLines()
	{
		m_nLine = 0;
	}

	inline void clearBoxs()
	{
		m_nBox = 0;
	}

    inline void clear2DRects()
    {
        m_n2DRect = 0;
    }
    
	inline int getLinesCount()
	{
		return m_nLine;
	}

	inline int getBoxCount()
	{
		return m_nBox;
	}

    inline int getRectCount()
    {
        return m_n2DRect;
    }
    
	inline const SLineDebug& getLine(int i)
	{
		return m_lines[i];
	}

	inline const SBoxDebug& getBox(int i)
	{
		return m_boxs[i];
	}
    
    inline const S2DRectDebug& getRect(int i)
    {
        return m_2dRect[i];
    }
    
    void draw2D();

};

#endif