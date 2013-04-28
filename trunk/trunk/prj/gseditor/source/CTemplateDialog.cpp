#include "stdafx.h"
#include "CTemplateDialog.h"
#include "CGameObject.h"

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
	else
	{
		m_isEditObjTemplate = true;
		m_objectTemplate	= objectTemplate;
	}

	// add template group
	uiListPropertyGroup *pHeader	= m_componentControl->addGroup(L"Object template:");
	uiListPropertyRow	*pRow		= m_componentControl->addRowItem( L"Template name" );
	
	// set name of template
	if ( m_isEditObjTemplate == true )	
		pRow->setText( m_objectTemplate->getObjectTemplateName(), 1 );	

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
		uiString::convertUTF8ToUnicode(r->name, lpText);		
		uiListPropertyRow *pRow = m_componentControl->addRowItem(lpText);

		// set control on column 1 is combobox
		uiComboBox *pComboBox =	(uiComboBox*)pRow->setControl( UILISTPROPERTY_COMBOBOX, 1, NULL );
		pComboBox->addString( L"disable" );
		pComboBox->addString( L"enable" );
		
		// set enable if this template has component
		if ( m_isEditObjTemplate == true )
		{
			if ( m_objectTemplate->containComponent( r->name ) == true )
			{
				pRow->setText(L"enable", 1);
				pComboBox->selectItem(1);
			}
		}
		
		// set event when combobox change
		pComboBox->setEventOnSelectChange<CTemplateDialog, &CTemplateDialog::onComboboxEnableComponent> (this);
	}

	// set event on change component
	m_componentControl->setEventOnUpdateProperty<CTemplateDialog, &CTemplateDialog::onUpdateListComponent> ( this );
	m_propertyControl->setEventOnUpdateProperty<CTemplateDialog, &CTemplateDialog::onUpdateListProperty> (this);
	m_propertyControl->setEventOnCmbChange<CTemplateDialog, &CTemplateDialog::onUpdateListProperty> (this);

	if ( m_isEditObjTemplate == true )
		addObjectTemplateProperty();

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
			uiString::convertUnicodeToUTF8(lpComponentName, lpComponentNameA );

			bool	objHasComponent		= m_objectTemplate->containComponent( lpComponentNameA );

			if ( pCmbBox->getSelectIndex() == 0 )
			{
				// select disable
				pRow->setText(L"",1);
				
				if ( objHasComponent == true )
				{
					m_objectTemplate->removeComponent( lpComponentNameA );
					hasChangeInfo = true;
				}
			}
			else
			{								
				// select enable
				// add template to object
				if ( objHasComponent == false )
				{
					m_objectTemplate->addComponent( lpComponentNameA );
					hasChangeInfo = true;
				}

			}
		}
	}

	// update for list property
	if ( hasChangeInfo )
		addObjectTemplateProperty();
}

// onUpdateListProperty
// when user update list control property
void CTemplateDialog::onUpdateListProperty(uiObject *pSender )
{
	ArraySerializable *allComponent = m_objectTemplate->getAllComponentProperty();
	ArraySerializable::iterator i = allComponent->begin(), end = allComponent->end();

	WCHAR	lpText[1024];

	// load all component
	while ( i != end )
	{
		CSerializable *p = &(*i);

		ArraySerializableRecIter iRec = p->getAllRecord()->begin(); 
		ArraySerializableRecIter iEnd = p->getAllRecord()->end();
		
		// load all property of component
		while (iRec != iEnd)
		{
			SSerializableRec *pRec = &(*iRec);
			if ( pRec->tagObject != NULL )
			{
				// get row on list control
				uiListPropertyRow *pRow = (uiListPropertyRow*)pRec->tagObject;

				// copy data from control to serializable
				pRow->getText( lpText, 1 );
				uiString::convertUnicodeToUTF8(lpText, pRec->data );

			}
			iRec++;
		}

		i++;
	}
}

// addObjectTemplateProperty
// add value from list component to property
void CTemplateDialog::addObjectTemplateProperty()
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
		uiString::convertUTF8ToUnicode(r.name, lpName);
		
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
			uiString::convertUTF8ToUnicode(r.data, lpValue);
			pRow->setText( lpValue, 1 );

			// enable edit
			if ( r.readOnly == false )
			{
				if ( r.type == NSSerializable::boolType )
				{
					uiComboBox *pComboBox =	(uiComboBox*)pRow->setControl( UILISTPROPERTY_COMBOBOX, 1, NULL );
					pComboBox->addString(L"true");
					pComboBox->addString(L"false");
					
					if ( strcmp(r.data, "true") == 0 )
						pComboBox->selectItem( 0 );
					else
						pComboBox->selectItem( 1 );
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


// onOKButton
// event when press OK
bool CTemplateDialog::onOKButton()
{
	// get name template
	uiListPropertyRow *pRow = (uiListPropertyRow*)m_componentControl->getItem(1);

	WCHAR lpName[1024];
	pRow->getText( lpName, 1 );
	uiString::trim<WCHAR>( lpName );

	if ( uiString::length<WCHAR>( lpName ) == 0 )
	{
		alert(L"Please type template name", MB_OK);
		return false;
	}
	
	m_objectTemplate->setObjectTemplateName( lpName );

	// add to list
	if ( m_isEditObjTemplate == false )
	{
		CObjTemplateFactory::addTemplate( m_objectTemplate );
	
		// spawn test object
		CGameObject *pObj = CObjTemplateFactory::spawnObject( lpName );
		pObj->setObjectState( CGameObject::Move );
		CObjTemplateFactory::addTemplateObject( pObj );
	}

	// save template
	CObjTemplateFactory::saveAllObjectTemplate();

	return true;
}