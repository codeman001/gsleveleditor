#ifndef _BASEDIALOG_
#define _BASEDIALOG_

#include "stdafx.h"

class CBaseDialog: public uiDialog
{
protected:
	int		m_headerHeight;
	int		m_bottomHeight;

	uiFont		m_titleFont;
	uiStringW	m_titleString;
public:
	CBaseDialog(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);

	virtual ~CBaseDialog(void);
	
	virtual void onButtonCommand( uiObject *pSender );

	inline void setTitleString( wchar_t* lpString )
	{
		m_titleString = lpString;
	}

	inline void setHeaderHeight( int h )
	{
		m_headerHeight = h;
	}

	inline void setBottomHeight( int h )
	{
		m_bottomHeight = h;
	}

public:
	virtual void _onPaint( uiGraphics *pG );
	
};

#endif