#ifndef _VIEWWIN_
#define _VIEWWIN_

#include "IView.h"
#include "CBaseWindow.h"

class CViewWindow: public CBaseWindow, IView
{
public:
	CViewWindow( WCHAR* lpString, uiWindow *p );
	virtual ~CViewWindow();

	virtual void _OnSize(	uiSizeEvent sizeEvent, int nWidth, int nHeight);
	virtual void _OnPaint( uiGraphics *pG );

	virtual void _OnMouseMove	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnMouseWheel	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnLButtonUp	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnLButtonDown	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnMButtonUp	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnMButtonDown	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnRButtonUp	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnRButtonDown	( uiMouseEvent mouseEvent, int x, int y );	
	
public:
	void createIrrDevice();
public:
	virtual void idleUpdateWindow();

	virtual void alertError( wchar_t *lpString );
	virtual bool yesNoQuestion( wchar_t *lpString );
	virtual void setStatusText( int part, wchar_t *lpString );

public:

	// setObjectProperty
	// set object property on listview window
	virtual void setObjectProperty(CGameObject *pObj);

	// getCurrentObjectTemplate
	// get current name
	virtual wchar_t* getCurrentObjectTemplate();

	// objTemplate
	// set current template
	virtual void setCurrentObjectTemplate( wchar_t *objTemplate );

	// getGameObjectOfCurrentTemplate
	// get current obj
	virtual CGameObject* getGameObjectOfCurrentTemplate();

	// getCurrentZone
	// get current zone
	virtual CZone* getCurrentZone();

	// setCurrentZone
	// get current zone
	virtual void setCurrentZone(CZone *pZone);

	// newDocument
	// renew document
	virtual void newDocument();
	
	// getDocumentTreeView
	// get tree view control of document
	virtual uiTreeView* getDocumentTreeView();
protected:
	
	uiStringW		m_objTemplate;
	CGameObject*	m_pObjTemplate;
	CGameObject*	m_pZone;
};

#endif