#include "stdafx.h"

#include "CComponentDialog.h"
#include "CComponentFactory.h"
#include "CObjTemplateFactory.h"

#define stringOfType(x)	CSerializable::s_stringType[(int)x]
#define NUM_PARAMS	20

CComponentDialog::CComponentDialog(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, CSerializable *componentTemplate)
	:CBaseDialog(lpTitle, x, y, w, h, pParent)
{
	m_titleString = L"Insert, edit component with property name, property type, default variable";
	
	m_listControl = ref<uiListProperty>( new uiListProperty(L"listParam", 0,0,w,20, this, 3 ));	
	m_listControl->setMargin( 0, 0, m_headerHeight, m_bottomHeight );
	m_listControl->setDock( this, UIDOCK_FILL );
	m_listControl->showWindow(true);

	uiListPropertyRow *pRow = NULL;
	uiComboBox *pListType = NULL;

	m_componentTemplate = componentTemplate;

	int beginRecord = -1;
	int numPropertyRecord = -1;

	// save position
	if ( m_componentTemplate )
	{
		beginRecord			= m_componentTemplate->getCursorRecord();
		numPropertyRecord	= (int)m_componentTemplate->getAllRecord()->size() - 1;
	}

	// add header name
	uiListPropertyGroup *pHeader = m_listControl->addGroup(L"Component:");
	pRow = m_listControl->addRowItem( L"Component name" );
	
	wchar_t lpText[1024] = {0};

	if ( m_componentTemplate )
	{
		uiString::convertUTF8ToUnicode( m_componentTemplate->readRawRecord()->name, (unsigned short*) lpText );
		pRow->setText( lpText, 1 );
	}
	else
		pRow->setControl( UILISTPROPERTY_EDIT, 1, NULL );


	// add parameter header
	pHeader = m_listControl->addGroup(L"Parameter:");
	pHeader->enableColText( true );
	pHeader->setColText(L"Param name:", 0);
	pHeader->setColText(L"Param type:", 1);
	pHeader->setColText(L"Default value:", 2);
		
	int numParam = NUM_PARAMS;	
	bool isBuildIn = false;
	
	if ( m_componentTemplate )
		isBuildIn = CComponentFactory::isBuildInComponent( m_componentTemplate );

	if ( isBuildIn )
		numParam = numPropertyRecord;

	// add param to list property
	for (int i = 0; i < numParam; i++ )
	{		
		pRow = m_listControl->addRowItem( L"" );
		
		// if edit component
		if ( m_componentTemplate && i < numPropertyRecord )
		{
			SSerializableRec* pRec = m_componentTemplate->readRawRecord();

			// set type variable text at column 1
			uiString::convertUTF8ToUnicode( stringOfType(pRec->type), (unsigned short*) lpText );
			pRow->setText( lpText, 1 );
			
			if ( pRec->data != NULL )
			{
				// set name property text
				uiString::convertUTF8ToUnicode( pRec->name, (unsigned short*) lpText );
				pRow->setText( lpText, 0 );

				// set value property text
				uiString::convertUTF8ToUnicode( pRec->data, (unsigned short*) lpText );
				pRow->setText( lpText, 2 );
			}

			if ( isBuildIn )
			{
				// only modify value property on value
				uiListPropertyEdit *pEdit = (uiListPropertyEdit*)pRow->setControl( UILISTPROPERTY_EDIT, 2, NULL );
				setEditMode( pRec->type, pEdit );
			}
			else
			{
				// set combobox with list type on column 1
				pListType = (uiComboBox*)pRow->setControl( UILISTPROPERTY_COMBOBOX, 1, NULL );
				for ( int i = 0; i < NSSerializable::numType; i++ )
				{
					uiString::convertUTF8ToUnicode( stringOfType(i), (unsigned short*) lpText );
					pListType->addString( lpText );
				}
				pListType->selectItem( (DWORD) pRec->type );
				pListType->setEventOnSelectChange<CComponentDialog, &CComponentDialog::onComboboxTypeChange> (this);

				// set control value property is edit (column 2)
				pRow->setControl( UILISTPROPERTY_EDIT, 2, NULL );
			}
		}
		// if create new component
		else
		{
			// column 0 is edit
			pRow->setControl( UILISTPROPERTY_EDIT, 0, NULL );

			// column 1 is combbox
			pListType = (uiComboBox*)pRow->setControl( UILISTPROPERTY_COMBOBOX, 1, NULL );
			for ( int i = 0; i < NSSerializable::numType; i++ )
			{
				uiString::convertUTF8ToUnicode( stringOfType(i), (unsigned short*) lpText );
				pListType->addString( lpText );
			}
			pListType->selectItem( 0 );
			pListType->setEventOnSelectChange<CComponentDialog, &CComponentDialog::onComboboxTypeChange> (this);

			// column 2 is edit
			pRow->setControl( UILISTPROPERTY_EDIT, 2, NULL );
		}
	}
	
	// set event update for listcontrol
	m_listControl->setEventOnUpdateProperty	<CComponentDialog, &CComponentDialog::onListPropertyChange> (this );
	m_listControl->setEventOnDeActive		<CComponentDialog, &CComponentDialog::onListPropertyChange> (this );

	// reload position
	if ( m_componentTemplate )	
		m_componentTemplate->setCursorRecord( beginRecord );

	// update edit type
	updateEditType();

	// update dock
	updateDock();
}

CComponentDialog::~CComponentDialog()
{
	
}

// onComboboxTypeChange
// change combobox
void CComponentDialog::onComboboxTypeChange(uiObject *pSender)
{
	updateEditType();
}

// onListPropertyChange
// change data
void CComponentDialog::onListPropertyChange(uiObject *pSender)
{
	updateEditType();
}

// updateEditType
// update edit control for variable type (float, string, number...)
void CComponentDialog::updateEditType()
{
	int itemCount = m_listControl->getItemCount();
	for ( int i = 3; i < itemCount; i++ )
	{
		uiListPropertyItem* pItem = m_listControl->getItem(i);

		if ( pItem->getObjectType() == 2 )
		{
			uiListPropertyRow *pRow = (uiListPropertyRow*)pItem;
			
			uiComboBox*				pCmbBox		= (uiComboBox*)pRow->getControl( 1 );
			uiListPropertyEdit*		pEditBox	= (uiListPropertyEdit*)pRow->getControl( 2 );

			if ( pCmbBox && pEditBox )
			{
				NSSerializable::dataType varType = (NSSerializable::dataType) pCmbBox->getSelectIndex();
				setEditMode( varType, pEditBox );

				// set null string
				if ( varType == NSSerializable::unknownType )
					pRow->setText(L"", 1);
			}
		}
	}
}

void CComponentDialog::setEditMode( NSSerializable::dataType type, uiListPropertyEdit *p )
{
	p->setNumberValueOnly( false );
	p->setNumberFloatValueOnly( false );
	
	switch ( type )
	{
		case NSSerializable::intType:
		case NSSerializable::longType:
		{
			p->setNumberValueOnly( true );
			break;
		}
		case NSSerializable::floatType:
		case NSSerializable::doubleType:
		{
			p->setNumberFloatValueOnly( true );
			break;
		}
	}
}

// onOKButton
// on OK button click
bool CComponentDialog::onOKButton()
{	
	uiListPropertyRow *pRow = (uiListPropertyRow*)m_listControl->getItem(1);

	wchar_t lpString[1024];
	char	lpNameCom[1024];
	char	lpValue[1024];
	char	lpName[1024];

	pRow->getText( lpString, 1 );
	uiString::trim<WCHAR>( lpString );

	if ( uiString::length<WCHAR>( lpString ) == 0 )
	{
		alert(L"Please type component name", MB_OK);
		return false;
	}

	CSerializable *p = NULL;
	CSerializable s;

	if ( m_componentTemplate )
	{
		m_componentTemplate->clear();
		p = m_componentTemplate;
	}
	else
	{
		p = &s;
	}	

	// add group
	uiString::convertUnicodeToUTF8((unsigned short*)lpString, lpNameCom);
	p->addGroup( lpNameCom );
	
	int n = m_listControl->getItemCount();

	for ( int i = 3; i < n; i++ )
	{
		pRow = (uiListPropertyRow*)m_listControl->getItem( i);
		
		// get property name
		pRow->getText( lpString, 0 );
		uiString::trim<wchar_t>( lpString );
		uiString::convertUnicodeToUTF8( (unsigned short*)lpString, lpName );
		if ( strlen( lpName ) == 0 )
			continue;

		// get property type
		pRow->getText( lpString, 1 );
		uiString::trim<wchar_t>( lpString );
		uiString::convertUnicodeToUTF8( (unsigned short*)lpString, lpValue );
		NSSerializable::dataType type = CSerializable::getType( lpValue );
		if ( type == NSSerializable::unknownType )
			continue;

		// get property value
		pRow->getText( lpString, 2 );
		uiString::trim<wchar_t>( lpString );
		uiString::convertUnicodeToUTF8( (unsigned short*)lpString, lpValue );

		// add record
		SSerializableRec r;
		strcpy (r.name, lpName );
		strcpy (r.data, lpValue );
		r.type = type;
		r.readOnly = false;

		// add row to serializable
		p->addRow( &r );
	}

	// add component
	if ( m_componentTemplate == NULL )
		CComponentFactory::addComponent( p );
	else
	{
		// also need modify infomation on all template object
		ArrayTemplateIter it =	CObjTemplateFactory::s_objectTemplate.begin(), end = CObjTemplateFactory::s_objectTemplate.end();
		while ( it != end )
		{
			CObjectTemplate *p = &(*it);
			
			CSerializable *pSerializable =	p->getComponent( lpNameCom );

			// modify all template because component has changed!
			if ( pSerializable != NULL )
			{
				// set dirty for object template
				p->setDirty(true);

				// clear and update new serializable
				CSerializable old = *pSerializable;				
				pSerializable->clear();

				// add all row data
				ArraySerializableRec *arrayRec = m_componentTemplate->getAllRecord();
				ArraySerializableRecIter iRec = arrayRec->begin(), iRecEnd = arrayRec->end();
				while ( iRec != iRecEnd )
				{
					SSerializableRec newRec = (*iRec);
					SSerializableRec *p = old.getProperty( newRec.name );
					
					// copy from old data to new serializable
					if ( p )
						strcpy( newRec.data, p->data );
					
					pSerializable->addRow( &newRec );
					iRec++;
				}
			}
			++it;
		}
	}

	return true;
}