#include "stdafx.h"

#include "IObjectComponent.h"
#include "CComponentFactory.h"
#include "CObjTemplateFactory.h"

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
	
	int numComponent = CComponentFactory::s_compTemplate.size();

	if ( numComponent == 0 )
		return;

	wchar_t lpValue[512];

	// add build in template
	for ( int i = (int)IObjectComponent::AnimMesh; i < numComponent; i++ )
	{
		CSerializable *p = &CComponentFactory::s_compTemplate[i];

		// read first record
		int pos = p->getCursorRecord();
		SSerializableRec *r = p->readRawRecord();

		if ( r == NULL || r->type != NSSerializable::groupInfo )
			continue;

		// convert value
		uiString::convertUTF8ToUnicode(r->name, lpValue);
		uiComboBoxItem *pItem =	m_comboList->addItem( lpValue, lpValue );
		
		// tag type mesh to list property
		pItem->setData( (uiObject*)i );
		
		p->setCursorRecord( pos );
	}

	// select last template
	m_comboList->selectItem ( m_comboList->getItemCount() - 1 );
}


void CComponentWindow::onAddButton()
{
	CComponentDialog dialog(L"Add, modify component", 0, 0, 500, 400, this);
	dialog.setPositionCenterOfScreen();
	
	uiApplication::getRoot()->doModal( &dialog );

	reloadList();
}

void CComponentWindow::onModifyButton()
{
	uiComboBoxItem *pItem =	m_comboList->getSelectItem();
	if ( pItem )
	{
		int i = (int) pItem->getData();
		CSerializable *p = &CComponentFactory::s_compTemplate[i];

		CComponentDialog dialog(L"Add, modify component", 0, 0, 500, 400, this, p);
		dialog.setPositionCenterOfScreen();
		
		uiApplication::getRoot()->doModal( &dialog );
	}
}

void CComponentWindow::onDelButton()
{
	uiComboBoxItem *pItem =	m_comboList->getSelectItem();
	if ( pItem )
	{
		int i = (int) pItem->getData();		
		CSerializable *p = &CComponentFactory::s_compTemplate[i];
		char *lpName = p->getAllRecord()->front().name;

		if ( CComponentFactory::isBuildInComponent( p ) == true )
		{
			alert(L"Can not delete build in component\n",NULL);
			return;
		}
		else
		{
			// check it use
			ArrayTemplateIter it = CObjTemplateFactory::s_objectTemplate.begin(), 
				end = CObjTemplateFactory::s_objectTemplate.end();

			while ( it != end )
			{
				if ( (*it).containComponent(lpName) == true )
				{
					alert(L"Can not delete this component because it use on template\n",NULL);
					return;
				}
				it++;
			}
			
			CComponentFactory::removeComponent( p );
			CComponentFactory::saveAllTemplate();
			reloadList();
		}
	}
}