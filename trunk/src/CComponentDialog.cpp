#include "stdafx.h"
#include "CComponentDialog.h"

CComponentDialog::CComponentDialog(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
	:CBaseDialog(lpTitle, x, y, w, h, pParent)
{
	m_titleString = L"Insert, edit component with property name, property type, default variable";

	m_listControl = ref<uiListProperty>( new uiListProperty(L"listParam", 0,0,w,20, this, 3 ));
	
	m_listControl->setMargin( 0, 0, m_headerHeight, m_bottomHeight );
	m_listControl->setDock( this, UIDOCK_FILL );
	m_listControl->showWindow(true);

	uiListPropertyGroup *pHeader = m_listControl->addGroup(L"Header");
	pHeader->enableColText( true );
	pHeader->setColText(L"Param name:", 0);
	pHeader->setColText(L"Param type:", 1);
	pHeader->setColText(L"Default value:", 2);

	WCHAR lpString[512];
	
	uiListPropertyRow *pRow = NULL;
	uiComboBox *pListType = NULL;

	for (int i = 0; i < 20; i++ )
	{		
		swprintf(lpString, 512, L"Enter param name", i);
		
		pRow = m_listControl->addRowItem( lpString );
		
		pRow->setControl( UILISTPROPERTY_EDIT, 0, NULL );
		pListType = (uiComboBox*)pRow->setControl( UILISTPROPERTY_COMBOBOX, 1, NULL );
		pRow->setControl( UILISTPROPERTY_EDIT, 2, NULL );



	}

	updateDock();
}

CComponentDialog::~CComponentDialog()
{
}