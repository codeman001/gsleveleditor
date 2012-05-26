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
	
	bool					m_changeTime;
	float					m_currentTime;
	int						m_crollX;
	
	float					m_maxValue;
	float					m_minValue;

	int						m_timerID;

	bool					m_lbuttonDown;
	bool					m_rbuttonDown;
	int						m_x;
	int						m_y;
public:
	CTimelineControl(uiWindow* parent, int x, int y, int w, int h);

	virtual ~CTimelineControl();
	
	virtual void _OnPaint( uiGraphics *pG );
	
	virtual void _OnLButtonDown( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnLButtonUp( uiMouseEvent mouseEvent, int x, int y );

	virtual void _OnMouseMove( uiMouseEvent mouseEvent, int x, int y );

	virtual void _OnRButtonDown( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnRButtonUp( uiMouseEvent mouseEvent, int x, int y );

	void paintControl( uiGraphics *pG );

	void update();

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

	void setCurrentTime(float f)
	{
		m_currentTime = f;
	}

	float getCurrentTime()
	{
		return m_currentTime;
	}

	void sortValue( vector<STimelineValue>& value );
	
	void calcMinMax( vector<STimelineValue>& value );	

	void clearAllValue()
	{
		m_value.clear();
		m_timeLength = 0.0f;
		m_needSortValue = true;
	}

	int getX( float v );
	int getY( float v );
	float getTimeValue( int v );

	void enableChangeTime( bool b )
	{
		m_changeTime = b;
	}
	
};

#endif