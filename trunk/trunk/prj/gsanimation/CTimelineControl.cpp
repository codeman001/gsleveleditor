#include "stdafx.h"
#include "CTimelineControl.h"

CTimelineControl::CTimelineControl(uiWindow* parent, int x, int y, int w, int h)
	:uiWindow(L"timeLineControl", x, y, w, h, parent)
{
	changeWindowStyle( UISTYLE_CHILD );
	showWindow(true);

	m_timeLength	= 0.0f;
	m_timeView		= 0.0f;

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

void CTimelineControl::paintControl( uiGraphics *pG )
{
	if ( m_needSortValue == true )
	{
		sortValue(m_valueX);
		sortValue(m_valueY);
		sortValue(m_valueZ);
		m_needSortValue = false;
	}

	int nWidth = getClientWidth();
	int nHeight = getClientHeight();
	
	uiBrush bgGrey( uiColor(0x9f9f9f) );

	uiGraphics *graphics = pG->createBackBuffer( nWidth, nHeight );

	// fill background
	graphics->drawFillRectangle(0,0, nWidth, nHeight, &bgGrey);
	

	
	pG->swapBuffer(0,0,nWidth, nHeight, graphics, 0,0,nWidth,nHeight, SRCCOPY);
	graphics->releaseGraphics();	
}