#ifndef _CMAPPROPERTY_WIN_
#define _CMAPPROPERTY_WIN_

#include "CBaseWindow.h"
#include "CGameObject.h"

class CObjPropertyWindow: public CBaseWindow
{
protected:
	CGameObject			*m_currentObject;
	uiListProperty		*m_propertyWnd;
public:
	CObjPropertyWindow( WCHAR* lpString, uiWindow *p );
	virtual ~CObjPropertyWindow();
	
	virtual void _OnPaint( uiGraphics *pG );

	// setObject
	// set object to view property
	virtual void setObject( CGameObject *pObject );

	// _OnUpdatedProperty
	// need set data to object
	virtual void _OnUpdatedProperty( uiObject *pSender );
};

#endif