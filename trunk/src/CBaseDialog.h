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

	uiButton*	m_okButton;
	uiButton*	m_cancelButton;
public:
	CBaseDialog(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);

	virtual ~CBaseDialog(void);
	
	virtual void onButtonCommand( uiObject *pSender );
	
	virtual bool onOKButton()
	{
		return true;
	}

	virtual bool onCancelButton()
	{
		return true;
	}

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
	virtual void _OnPaint( uiGraphics *pG );
	
	virtual void _OnIdle();

	// messageMap	
	LRESULT messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);	
};

#endif