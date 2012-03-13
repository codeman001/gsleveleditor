#include "stdafx.h"

#include "IObjectComponent.h"
#include "CComponentFactory.h"

#include "CComponentWindow.h"
#include "CComponentDialog.h"

CComponentWindow::CComponentWindow( WCHAR *lpString, uiWindow *pParent )
	:CAddEditWindow(lpString, pParent)
{
	reloadList();
}

CComponentWindow::~CComponentWindow()
{
}

void CComponentWindow::_OnPaint( uiGraphics *pG )
{
	uiWindow::_OnPaint( pG );
	drawTitleBackground(pG, 0,0, this->getWidth(), 30, TEXT("Game component"));
}

// reloadList
// reload list of item
void CComponentWindow::reloadList()
{
	m_comboList->clearItem();
	
	wchar_t lpValue[512];

	// add build in template
	for ( int i = (int)IObjectComponent::AnimMesh; i < (int)IObjectComponent::NumComponent; i++ )
	{
		CSerializable *p = &CComponentFactory::s_compTemplate[i];

		// read first record
		SSerializableRec *r = p->readRawRecord();
		
		if ( r == NULL || r->type != NSSerializable::groupInfo )
			continue;

		// convert value
		uiString::convertUTF8ToUnicode( r->name, (unsigned short*)lpValue );
		m_comboList->addItem( lpValue, lpValue );
	}
}


void CComponentWindow::onAddButton()
{
	CComponentDialog dialog(L"Add, modify component", 0, 0, 500, 400, this);
	dialog.setPositionCenterOfScreen();
	
	uiApplication::getRoot()->doModal( &dialog );
}

void CComponentWindow::onModifyButton()
{
}

void CComponentWindow::onDelButton()
{
}