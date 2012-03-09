#include "stdafx.h"
#include "CObjPropertyWin.h"
#include "CSerializable.h"

CObjPropertyWindow::CObjPropertyWindow( WCHAR* lpString, uiWindow *p )
	:CBaseWindow(lpString, p)
{
	m_propertyWnd = ref<uiListProperty>( new uiListProperty(TEXT("propertyWnd"), 0,0,10,10, this, 2) );

	m_propertyWnd->setMargin(0,0,30,0);
	m_propertyWnd->setDock( this, UIDOCK_FILL );
	m_propertyWnd->showWindow(true);

	// set event on change property
	m_propertyWnd->setEventOnCmbChange		<CObjPropertyWindow, &CObjPropertyWindow::_OnUpdatedProperty>( this );
	m_propertyWnd->setEventOnDeActive		<CObjPropertyWindow, &CObjPropertyWindow::_OnUpdatedProperty>( this );
	m_propertyWnd->setEventOnUpdateProperty	<CObjPropertyWindow, &CObjPropertyWindow::_OnUpdatedProperty>( this );

	m_currentObject = NULL;
}

CObjPropertyWindow::~CObjPropertyWindow()
{	
}

void CObjPropertyWindow::_OnPaint( uiGraphics *pG )
{
	uiWindow::_OnPaint( pG );
	drawTitleBackground(pG, 0,0, this->getWidth(), 30, TEXT("Object property") );	
}

// setObject
// set object to view property
void CObjPropertyWindow::setObject( CGameObject *pObject )
{
	m_currentObject = pObject;

	// delete all items
	m_propertyWnd->deleteAllItem();

	if ( m_currentObject )
	{
		// add new property
		CSerializable objectInfo;
		
		// get object info
		m_currentObject->saveData( &objectInfo );

		ArraySerializableRecIter iRec = objectInfo.getAllRecord()->begin(), 
			iEnd = objectInfo.getAllRecord()->end();
		
		wchar_t lpName[512]		= {0};
		wchar_t lpValue[512]	= {0};
		uiListPropertyRow *pRow = NULL;

		while ( iRec != iEnd )
		{
			SSerializableRec &r = (*iRec);
			
			// convert name
			uiString::convertUTF8ToUnicode( r.name, (unsigned short*)lpName );
			
			// add object
			if ( r.type == NSSerializable::groupInfo )
			{
				// add group
				m_propertyWnd->addGroup( lpName );
			}
			else if ( r.type != NSSerializable::arrayByte )
			{
				// add row
				pRow = m_propertyWnd->addRowItem( lpName );
			
				// convert value
				uiString::convertUTF8ToUnicode( r.data, (unsigned short*)lpValue );
				pRow->setText( lpValue, 1 );
			}

			iRec++;
		}

	}

	// update list property
	m_propertyWnd->reDrawInterface();

}

// _OnUpdatedProperty
// need set data to object
void CObjPropertyWindow::_OnUpdatedProperty( uiObject *pSender )
{
}