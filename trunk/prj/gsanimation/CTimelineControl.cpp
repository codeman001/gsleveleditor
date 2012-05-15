#include "stdafx.h"
#include "CTimelineControl.h"

CTimelineControl::CTimelineControl(uiWindow* parent, int x, int y, int w, int h)
	:uiWindow(L"timeLineControl", x, y, w, h, parent)
{
	changeWindowStyle( UISTYLE_CHILD );
	showWindow(true);

	m_timeLength	= 0.0f;
	m_timeView		= 0.0f;
	m_maxValue = 0.0f;
	m_minValue = 0.0f;

	m_lengthPixel	= 10;	
	m_needSortValue	= true;	
}

CTimelineControl::~CTimelineControl()
{

}

void CTimelineControl::_OnPaint( uiGraphics *pG )
{
	uiWindow::_OnPaint(pG);
	paintControl(pG);
}

void CTimelineControl::sortValue( vector<STimelineValue>& value )
{
	int len = (int)value.size();
	
	for (int i = 1; i <= len - 1; i++)
	{
		STimelineValue s = value[i];
		
		int j = i - 1;
		int done = 0;

		do
		{
			if ( value[j].time > s.time )
			{
				value[j + 1] = value[j];

				j = j - 1;
				if ( j < 0 )
					done = 1;
			}
			else
				done = 1;
		}
		while (done == 0);
		
		value[j + 1] = s;		
	}
}

void CTimelineControl::calcMinMax( vector<STimelineValue>& value )
{
	m_maxValue = 0.0f;
	m_minValue = 0.0f;

	int len = (int)value.size();
	if ( len > 0 )
	{
		m_maxValue = value[0].x;
		m_minValue = value[0].x;
	}

	for ( int i = 1; i < len; i++ )
	{
		if ( m_minValue > value[i].x )
			m_minValue = value[i].x;
		if ( m_minValue > value[i].y )
			m_minValue = value[i].y;
		if ( m_minValue > value[i].z )
			m_minValue = value[i].z;

		if ( m_maxValue < value[i].x )
			m_maxValue = value[i].x;
		if ( m_maxValue < value[i].y )
			m_maxValue = value[i].y;
		if ( m_maxValue < value[i].z )
			m_maxValue = value[i].z;
	}
}

void CTimelineControl::paintControl( uiGraphics *pG )
{
	if ( m_needSortValue == true )
	{
		sortValue(m_value);	
		calcMinMax(m_value);
		m_needSortValue = false;
	}

	int nWidth = getClientWidth();
	int nHeight = getClientHeight();
	
	uiBrush bgGrey( uiColor(0x9f9f9f) );

	uiGraphics *graphics = pG->createBackBuffer( nWidth, nHeight );

	// fill background
	graphics->drawFillRectangle(0,0, nWidth, nHeight, &bgGrey);	
	
	if ( m_maxValue > 0.0f && m_minValue > 0.0f )
	{		
		float midValue = (m_maxValue - m_minValue)*0.5f;	
		
		uiPen pen(1, PS_SOLID, uiColor(0x888888));
		uiPen penX(2, PS_SOLID, uiColor(0x0000ff));
		uiPen penY(2, PS_SOLID, uiColor(0x00ff00));
		uiPen penZ(2, PS_SOLID, uiColor(0xff0000));

		graphics->selectObject(&pen);

		// draw center line
		graphics->drawLine(0, getY(midValue), nWidth, getY(midValue));
		
		// draw limit line
		graphics->drawLine(0, getY(m_minValue), nWidth, getY(m_minValue));
		graphics->drawLine(0, getY(m_maxValue), nWidth, getY(m_maxValue));
		
		int len = (int)m_value.size();
		for ( int i = 1; i < len; i++ )
		{
			int x0 = getX( m_value[i-1].time );
			int x1 = getX( m_value[i].time );
									
			int y0 = getY( m_value[i-1].x );
			int y1 = getY( m_value[i].x );
			graphics->selectObject(&pen);
			graphics->drawLine(x1,0, x1,nHeight);

			graphics->selectObject(&penX);
			graphics->drawLine(x0, y0, x1, y1);
			
			y0 = getY( m_value[i-1].y );
			y1 = getY( m_value[i].y );
			graphics->selectObject(&penY);
			graphics->drawLine(x0, y0, x1, y1);

			y0 = getY( m_value[i-1].z );
			y1 = getY( m_value[i].z );
			graphics->selectObject(&penZ);
			graphics->drawLine(x0, y0, x1, y1);
		}
		
		for ( int i = 1; i < len; i++ )
		{
			int r = 4;
			int x0 = getX( m_value[i-1].time );
			int x1 = getX( m_value[i].time );									
			int y0 = getY( m_value[i-1].x );
			int y1 = getY( m_value[i].x );
			
			graphics->selectObject(&penX);			
			graphics->drawRectangle( x1-r,y1-r,x1+r,y1+r );
			
			y0 = getY( m_value[i-1].y );
			y1 = getY( m_value[i].y );
			graphics->selectObject(&penY);
			graphics->drawRectangle( x1-r,y1-r,x1+r,y1+r );

			y0 = getY( m_value[i-1].z );
			y1 = getY( m_value[i].z );
			graphics->selectObject(&penZ);
			graphics->drawRectangle( x1-r,y1-r,x1+r,y1+r );
		}

		graphics->selectObject(&penX);
		
	}

	pG->swapBuffer(0,0,nWidth, nHeight, graphics, 0,0,nWidth,nHeight, SRCCOPY);
	graphics->releaseGraphics();	
}

int CTimelineControl::getX( float v )
{
	if ( v <= 0 || m_timeLength == 0.0f)
		return 0;
	
	float timeWidth = m_lengthPixel*m_timeLength;
	if ( v >= m_timeLength )
		return (int)timeWidth;

	return (int)(v*timeWidth/m_timeLength);
}

int CTimelineControl::getY( float v )
{
	float paddingY = 10.0f;
	float height = (float)getClientHeight() - paddingY*2;
	
	if ( v <= m_minValue )
		return (int)(paddingY + height);
	else if ( v >= m_maxValue )
		return (int)paddingY;

	float posY = paddingY + (m_maxValue - v)*height/( m_maxValue - m_minValue );	
	return (int)posY;
}