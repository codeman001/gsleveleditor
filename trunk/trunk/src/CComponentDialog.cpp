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

	uiListPropertyRow *pRow = NULL;
	uiComboBox *pListType = NULL;

	// add header name
	uiListPropertyGroup *pHeader = m_listControl->addGroup(L"Component:");
	pRow = m_listControl->addRowItem( L"Component name" );
	pRow->setControl( UILISTPROPERTY_EDIT, 1, NULL );

	// add parameter
	pHeader = m_listControl->addGroup(L"Parameter:");
	pHeader->enableColText( true );
	pHeader->setColText(L"Param name:", 0);
	pHeader->setColText(L"Param type:", 1);
	pHeader->setColText(L"Default value:", 2);
		
	// add default 15 parameter
	for (int i = 0; i < 20; i++ )
	{
		pRow = m_listControl->addRowItem( L"" );
		
		pRow->setControl( UILISTPROPERTY_EDIT, 0, NULL );
		pListType = (uiComboBox*)pRow->setControl( UILISTPROPERTY_COMBOBOX, 1, NULL );
		pRow->setControl( UILISTPROPERTY_EDIT, 2, NULL );
	}
	updateDock();
}

CComponentDialog::~CComponentDialog()
{
}