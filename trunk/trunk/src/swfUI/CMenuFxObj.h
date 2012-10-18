#ifndef _MENUFXOBJ_H_
#define _MENUFXOBJ_H_

#include "CGameUI.h"

class CMenuFxObj
{
protected:
	gameswf::character	*m_character;
	CMenuFx				*m_menu;
public:
	CMenuFxObj()
	{
		m_character = NULL;
		m_menu = NULL;
	}

	CMenuFxObj(	CMenuFx* menufx, gameswf::character* ch );		
	virtual ~CMenuFxObj();	

	// findObj
	// find obj on flash menu
	CMenuFxObj findObj( const char *name );

	// setpos xy
	void setPosition( int x, int y );
	void setAbsolutePosition( int x, int y );

	// getBound
	// get bound
	void getBound(int *x, int *y, int *w, int *h);

	// getAbsoluteBound
	void getAbsoluteBound(int *x, int *y, int *w, int *h);

	// setVisible
	// set visible
	void setVisible( bool b );

	// setText
	// set text on edit
	void setText( const char *lpString );

	// setText
	// set tex with unicode
	void setText( const wchar_t *lpString );

	// getText
	// get current text on edit
	const char *getText();

	// setMaskClip
	// set mask clip
	void setMaskClip( CMenuFxObj clip );

	// gotoFrame
	// goto frame
	void gotoFrame( const char *label, bool play );
	void gotoFrame( int frame, bool play );

	// getFrameCount
	// get number of frame
	int getFrameCount();
	
	// getMenu
	// get flash file manage this obj
	inline CMenuFx* getMenu()
	{
		return m_menu;
	}

    bool isNull()
    {
        return m_character == NULL;
    }
    
	// invokeASCallback
	// call action script func
	void invokeASCallback( const char* funcName, const gameswf::as_value* arguments, int argumentCount );
};

#endif