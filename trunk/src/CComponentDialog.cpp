#include "stdafx.h"
#include "CComponentDialog.h"
#include "CComponentFactory.h"

#define stringOfType(x)	CSerializable::s_stringType[ (int)x]

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
		beginRecord = m_componentTemplate->getCursorRecord();
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



	// add parameter
	pHeader = m_listControl->addGroup(L"Parameter:");
	pHeader->enableColText( true );
	pHeader->setColText(L"Param name:", 0);
	pHeader->setColText(L"Param type:", 1);
	pHeader->setColText(L"Default value:", 2);
		
	// add default 20 parameter
	int numParam = 20;
	bool isBuildIn = CComponentFactory::isBuildInComponent( m_componentTemplate );

	if ( isBuildIn )
		numParam = numPropertyRecord;

	for (int i = 0; i < numParam; i++ )
	{
		pRow = m_listControl->addRowItem( L"" );
		
		if ( m_componentTemplate && i < numPropertyRecord )
		{
			SSerializableRec* pRec = m_componentTemplate->readRawRecord();

			// set type text
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
				pListType = (uiComboBox*)pRow->setControl( UILISTPROPERTY_COMBOBOX, 1, NULL );
				for ( int i = 0; i < NSSerializable::numType; i++ )
				{
					uiString::convertUTF8ToUnicode( stringOfType(i), (unsigned short*) lpText );
					pListType->addString( lpText );
				}				
				pListType->selectItem( (DWORD) pRec->type );

				pRow->setControl( UILISTPROPERTY_EDIT, 2, NULL );
			}
		}
		else
		{
			pRow->setControl( UILISTPROPERTY_EDIT, 0, NULL );
			pListType = (uiComboBox*)pRow->setControl( UILISTPROPERTY_COMBOBOX, 1, NULL );
			for ( int i = 0; i < NSSerializable::numType; i++ )
			{
				uiString::convertUTF8ToUnicode( stringOfType(i), (unsigned short*) lpText );
				pListType->addString( lpText );
			}
			pListType->selectItem( 0 );

			pRow->setControl( UILISTPROPERTY_EDIT, 2, NULL );
		}
	}
	
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

// updateEditType
// update edit control for variable type (float, string, number...)
void CComponentDialog::updateEditType()
{
	int itemCount = m_listControl->getItemCount();
	for ( int i = 4; i < itemCount; i++ )
	{
		uiListPropertyItem* pItem = m_listControl->getItem(i);

		// row item
		if ( pItem->getObjectType() == 2 )
		{
			uiListPropertyRow *pRow = (uiListPropertyRow*)pItem;
			
			uiComboBox*				pCmbBox = (uiComboBox*)pRow->getControl( 1 );
			uiListPropertyEdit*		pEditBox = (uiListPropertyEdit*)pRow->getControl( 2 );

			if ( pCmbBox && pEditBox )
			{
				setEditMode( (NSSerializable::dataType) pCmbBox->getSelectIndex(), pEditBox );
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