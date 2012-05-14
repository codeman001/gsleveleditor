#ifndef _CTIMELINE_CONTROL_
#define _CTIMELINE_CONTROL_

#include "stdafx.h"

struct STimelineValue
{
	float	time;
	float	value;
};

class CTimelineControl: public uiWindow
{
protected:
	float				m_timeLength;
	int					m_lengthPixel;
	
	bool				m_needSortValue;
	
	vector<STimelineValue>	m_valueX;
	vector<STimelineValue>	m_valueY;
	vector<STimelineValue>	m_valueZ;
	
	float				m_timeView;
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

	void addValue( int type, float time, float value )
	{
		STimelineValue s;
		s.time = time;
		s.value = value;
		
		if ( type == 0 )
			m_valueX.push_back( s );
		else if ( type == 1 )
			m_valueY.push_back( s );
		else
			m_valueZ.push_back( s );		

		m_needSortValue = true;
	}

	void sortValue( vector<STimelineValue>& value );
};

#endif