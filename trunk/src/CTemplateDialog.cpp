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

	m_isEditObjTemplate = true;
	m_objectTemplate	= NULL;

	if ( objectTemplate == NULL )
	{
		m_isEditObjTemplate = false;
		m_objectTemplate	= new CObjectTemplate();
	}

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
		uiListPropertyRow *pRow = m_componentControl->addRowItem(lpText);

		// set control on column 1 is combobox
		uiComboBox *pComboBox =	(uiComboBox*)pRow->setControl( UILISTPROPERTY_COMBOBOX, 1, NULL );
		pComboBox->addString( L"disable" );
		pComboBox->addString( L"enable" );

		// set event when combobox change
		pComboBox->setEventOnSelectChange<CTemplateDialog, &CTemplateDialog::onComboboxEnableComponent> (this);

	}

	// set event on change component
	m_componentControl->setEventOnUpdateProperty<CTemplateDialog, &CTemplateDialog::onUpdateListComponent> ( this );

	// update windows dock
	updateDock();
}

CTemplateDialog::~CTemplateDialog()
{
	if ( m_isEditObjTemplate == false )
		delete m_objectTemplate;
}

// onComboboxEnableComponent
// when user change combobox enable, disable component
void CTemplateDialog::onComboboxEnableComponent( uiObject *pSender )
{
	onUpdateListComponent( pSender );
}

// onUpdateListComponent
// when user update list control component
void CTemplateDialog::onUpdateListComponent(uiObject *pSender )
{
	wchar_t lpComponentName[1024] = {0};
	char	lpComponentNameA[1024] = {0};
	
	bool hasChangeInfo = false;
	int itemCount = m_componentControl->getItemCount();	

	for ( int i = 2; i < itemCount; i++ )
	{
		uiListPropertyItem *pItem = m_componentControl->getItem(i);

		if ( pItem->getObjectType() == 2 )
		{
			uiListPropertyRow	*pRow		= (uiListPropertyRow*)pItem;			
			uiComboBox			*pCmbBox	= (uiComboBox*)pRow->getControl( 1 );
			
			// get name of property
			pRow->getText( lpComponentName, 0 );
			uiString::convertUnicodeToUTF8( (unsigned short*)lpComponentName, lpComponentNameA );

			int		comID				= CComponentFactory::getComponentID( lpComponentNameA );
			bool	objHasComponent		= m_objectTemplate->containComponent(comID);

			if ( pCmbBox->getSelectIndex() == 0 )
			{
				// select disable
				pRow->setText(L"",1);
				
				if ( objHasComponent == true )
				{
					m_objectTemplate->removeComponent( comID );
					hasChangeInfo = true;
				}
			}
			else
			{								
				// select enable
				// add template to object
				if ( comID > 0 && objHasComponent == false )
				{
					m_objectTemplate->addComponent( comID );
					hasChangeInfo = true;
				}

			}
		}
	}

	// update for list property
	if ( hasChangeInfo )
		setListPropertyValue();
}

// onUpdateListProperty
// when user update list control property
void CTemplateDialog::onUpdateListProperty(uiObject *pSender )
{

}

// setListPropertyValue
// add value from list component to property
void CTemplateDialog::setListPropertyValue()
{
	m_propertyControl->deleteAllItem();	
	
	ArraySerializable *allProperty = m_objectTemplate->getAllComponentProperty();
	ArraySerializable::iterator i = allProperty->begin(), end = allProperty->end();
	while ( i != end )
	{
		addSerializableToProperty( &(*i) );
		i++;
	}
}

// addSerializableToProperty
// add serializable
void CTemplateDialog::addSerializableToProperty( CSerializable *p )
{
	ArraySerializableRecIter iRec = p->getAllRecord()->begin(); 
	ArraySerializableRecIter iEnd = p->getAllRecord()->end();
	
	wchar_t lpName[512]		= {0};
	wchar_t lpValue[512]	= {0};
	uiListPropertyRow *pRow = NULL;

	while ( iRec != iEnd )
	{
		SSerializableRec &r = (*iRec);
		
		// convert name
		uiString::convertUTF8ToUnicode( r.name, (unsigned short*)lpName );
		
		// erase tag object
		r.tagObject = NULL;

		// add object
		if ( r.type == NSSerializable::groupInfo )
		{
			// add group
			m_propertyControl->addGroup( lpName );
		}
		else if ( r.type != NSSerializable::arrayByte )
		{
			// add row
			pRow = m_propertyControl->addRowItem( lpName );
			
			// tag the control to row property
			r.tagObject = pRow;
			
			// convert value
			uiString::convertUTF8ToUnicode( r.data, (unsigned short*)lpValue );
			pRow->setText( lpValue, 1 );

			// enable edit
			if ( r.readOnly == false )
			{
				if ( r.type == NSSerializable::boolType )
				{
				}
				else if ( r.type == NSSerializable::stringType || r.type == NSSerializable::filePathType )
				{
					pRow->setControl( UILISTPROPERTY_EDIT, 1, NULL );
				}	
				else
				{
					uiEdit *pEditText = (uiEdit*)pRow->setControl( UILISTPROPERTY_EDIT, 1, NULL );

					if ( r.type == NSSerializable::intType || r.type == NSSerializable::longType )
						pEditText->setNumberValueOnly( true );
					else
						((uiListPropertyEdit*)pEditText)->setNumberFloatValueOnly( true );
				}
			}
		}
		iRec++;
	}
}