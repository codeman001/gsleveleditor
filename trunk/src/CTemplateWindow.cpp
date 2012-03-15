#include "stdafx.h"
#include "CTemplateWindow.h"
#include "CTemplateDialog.h"

CTemplateWindow::CTemplateWindow( WCHAR *lpString, uiWindow *p )
	:CAddEditWindow(lpString, p)
{
	reloadList();
}

CTemplateWindow::~CTemplateWindow()
{
}
	
void CTemplateWindow::_OnPaint( uiGraphics *pG )
{
	uiWindow::_OnPaint( pG );
	drawTitleBackground(pG, 0,0, this->getWidth(), 30, TEXT("Game object template"));
}

void CTemplateWindow::onAddButton()
{
	CTemplateDialog dialog(L"Add, modify object template", 0, 0, 500, 400, this);
	dialog.setPositionCenterOfScreen();
	
	uiApplication::getRoot()->doModal( &dialog );

	reloadList();
}

void CTemplateWindow::onModifyButton()
{
	uiComboBoxItem *pItem = m_comboList->getSelectItem();
	if ( pItem )
	{
		CObjectTemplate *p = (CObjectTemplate*) pItem->getData();
		if ( p )
		{
			CTemplateDialog dialog(L"Add, modify object template", 0, 0, 500, 400, this, p);
			dialog.setPositionCenterOfScreen();
			
			uiApplication::getRoot()->doModal( &dialog );

			reloadList();
		}
	}
}

void CTemplateWindow::onDelButton()
{
	uiComboBoxItem *pItem = m_comboList->getSelectItem();
	if ( pItem )
	{
		CObjectTemplate *p = (CObjectTemplate*) pItem->getData();

		if ( p->getRef() > 0 )
		{
			alert( L"Can not delete this template because it is used on level!", NULL);
			return;
		}

		if ( p )
		{		
			WCHAR msg[1024];
			swprintf( msg, 1024, L"Do you want to delete this template '%s'", p->getObjectTemplateName() );
			if ( question( msg, NULL) == true )
			{
				CObjTemplateFactory::removeTemplate( p->getTemplateID() );
				CObjTemplateFactory::saveAllObjectTemplate();
				reloadList();
			}
		}
	}
}

// reloadList
// reload list of item
void CTemplateWindow::reloadList()
{
	m_comboList->clearItem();

	ArrayTemplateIter i = CObjTemplateFactory::s_objectTemplate.begin(), 
		end = CObjTemplateFactory::s_objectTemplate.end();
	
	while ( i != end )
	{
		CObjectTemplate *p = &(*i);

		uiComboBoxItem *pComboboxItem =	m_comboList->addString( p->getObjectTemplateName() );
		pComboboxItem->setData( (uiObject*) p );

		i++;
	}
}