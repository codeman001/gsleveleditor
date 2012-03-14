#include "stdafx.h"
#include "CTemplateDialog.h"

CTemplateDialog::CTemplateDialog(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, CObjectTemplate *objectTemplate)
	:CBaseDialog(lpTitle, x, y, w, h, pParent)
{
	m_titleString = L"Insert, edit object template";
	
	uiTabControl *pTab = ref<uiTabControl>( new uiTabControl(L"objTemplateTab", 0,0,w,20, this) );
	pTab->setMargin( 0, 0, m_headerHeight, m_bottomHeight );
	pTab->setDock( this, UIDOCK_FILL );
	pTab->showWindow(true);

	m_componentControl	= ref<uiListProperty>( new uiListProperty(L"listComponent", 0,0,w,20, pTab, 2 ));
	m_propertyControl	= ref<uiListProperty>( new uiListProperty(L"listProperty", 0,0,w,20, pTab, 2 ));

	pTab->addTab( L"Components",	m_componentControl	);
	pTab->addTab( L"Property",		m_propertyControl	);

	// add template group
	uiListPropertyGroup *pHeader	= m_componentControl->addGroup(L"Object template:");
	uiListPropertyRow	*pRow		= m_componentControl->addRowItem( L"Template name" );

	// set control edit name template
	pRow->setControl( UILISTPROPERTY_EDIT, 1, NULL );

	wchar_t lpText[1024] = {0};	

	// add component group
	pHeader = m_componentControl->addGroup(L"Components:");

	// add list component
	int numComponents = CComponentFactory::s_compTemplate.size();
	for ( int i = 0; i < numComponents; i++ )
	{
		CSerializable *p = &CComponentFactory::s_compTemplate[i];
		
		int beginPos = p->getCursorRecord();
		SSerializableRec *r = p->readRawRecord();
		p->setCursorRecord( beginPos );

		if ( r == NULL || r->type != NSSerializable::groupInfo )
			continue;

		// add row with name component
		uiString::convertUTF8ToUnicode( r->name, (unsigned short*) lpText );		
		m_componentControl->addRowItem(lpText);

	}

	// update windows dock
	updateDock();
}

CTemplateDialog::~CTemplateDialog()
{
}