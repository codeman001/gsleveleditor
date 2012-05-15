#ifndef _CTIMELINE_CONTROL_
#define _CTIMELINE_CONTROL_

#include "stdafx.h"

struct STimelineValue
{
	float	time;
	float	x;
	float	y;
	float	z;
};

class CTimelineControl: public uiWindow
{
protected:
	float				m_timeLength;
	int					m_lengthPixel;
	
	bool				m_needSortValue;
	
	vector<STimelineValue>	m_value;
	
	float					m_timeView;
	
	float					m_maxValue;
	float					m_minValue;

public:
	CTimelineControl(uiWindow* parent, int x, int y, int w, int h);
	virtual ~CTimelineControl();

	virtual void _OnPaint( uiGraphics *pG );

	void paintControl( uiGraphics *pG );

	void setTimeLength( float f )
	{
		m_timeLength = f;
	}

	void setLengthPixel( int pixel )
	{
		m_lengthPixel = pixel;
	}

	void addValue( float time, float x, float y, float z )
	{
		STimelineValue s;
		s.time = time;
		s.x = x;
		s.y = y;
		s.z = z;
		
		m_value.push_back( s );	

		m_needSortValue = true;
	}

	void sortValue( vector<STimelineValue>& value );
	
	void calcMinMax( vector<STimelineValue>& value );	

	void clearAllValue()
	{
		m_value.clear();
		m_needSortValue = true;
	}

	int getX( float v );
	int getY( float v );
	
};

#endif