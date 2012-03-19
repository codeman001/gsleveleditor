#include "uiListPropertyRow.h"
#include "uiListPropertyItem.h"
#include "uiListProperty.h"
#include "uiInclude.h"

using namespace uiClass;

uiListPropertyRow::uiListPropertyRow( uiListProperty *pControl, uiListPropertyItem* pParent, LPWSTR lpLabel, int nColumn )
	:uiListPropertyItem(pControl, pParent, nColumn),
	zm_colPos(nColumn),
	zm_font(13,L"Tahoma"),
	zm_colWindow(nColumn),
	zm_colText(nColumn),
	zm_colUsercontrol(nColumn)
{
	zm_paddingColor = uiColor::fromDWORD(0x00DFDFDF);
	zm_paddingColorH = uiColor::fromSystem(COLOR_MENUHILIGHT);
	zm_lineColor = uiColor::fromDWORD(0x00DFDFDF);

	zm_padding = 20;
	zm_paddingText = 20;

	zm_colClick = -1;
	zm_activeCol = -1;
	zm_objectType = 2;

	for ( int i = 0; i < nColumn; i++ )
	{
		zm_colWindow[i] = NULL;
		zm_colText[i] = L"";
		zm_colUsercontrol[i] = false;
	}

	setText( lpLabel, 0 );
}

uiListPropertyRow::~uiListPropertyRow()
{
	int nColumn = (int)zm_colWindow.size();

	for ( int i = 0; i < nColumn; i++ )
	{
		if ( zm_colWindow[i] != NULL && zm_colUsercontrol[i] == false )
		{
			delete zm_colWindow[i];
			zm_colWindow[i] = NULL;
		}
	}
}

// drawGroup
// ve group
void uiListPropertyRow::drawItem( uiGraphics* pG, int y )
{
	uiBrush brush( zm_paddingColor );	
	uiBrush focusbrush( zm_paddingColorH );	

	uiPen pen( 1, PS_SOLID, zm_lineColor );
	
	// Neu duoc focus
	if ( zm_focus )	
		pG->drawFillRectangle( 0,y, zm_padding, y + zm_itemHeight, &focusbrush );	
	else		
		pG->drawFillRectangle( 0,y, zm_padding, y + zm_itemHeight, &brush );


	pG->selectObject( &pen );

	WORD x = zm_padding;

	// Ve cot cua cung property
	for (WORD i = 0; i < zm_nColumn - 1; i++ )
	{
		if ( zm_colWindow[i] && zm_colWindow[i]->isShow() == false )
		{
			if ( zm_colWindow[i]->getObject() == UIOBJECT_COMBOBOX )
				zm_colWindow[i]->moveWindow( x,y, zm_nColumSize[i], 100 );
			else
				zm_colWindow[i]->moveWindow( x,y, zm_nColumSize[i], zm_itemHeight );
		}

		x += zm_nColumSize[i];
		pG->drawLine( x, y, x, y + zm_itemHeight );
		
		zm_colPos[i] = x;
	}
	
	// Thiet lap kich thuoc cho column cuoi cung
	int lastCol		= zm_nColumSize[zm_nColumn - 1];
	int lastWidth	= zm_control->getClientWidth() - x;

	zm_nColumSize[zm_nColumn - 1] = lastWidth;

	if ( zm_colWindow[zm_nColumn - 1] && zm_colWindow[zm_nColumn - 1]->isShow() == false )
	{
		if ( zm_colWindow[zm_nColumn - 1]->getObject() == UIOBJECT_COMBOBOX )
			zm_colWindow[zm_nColumn - 1]->moveWindow( x,y, lastWidth, 100 );
		else
		{
			zm_colWindow[zm_nColumn - 1]->moveWindow( x,y, lastWidth, zm_itemHeight );	
		}
	}

	// Ve duong line
	pG->drawLine(0, y + zm_itemHeight, zm_control->getClientWidth(),y + zm_itemHeight);
		

	// Ghi text
	pG->selectObject( &zm_font );
	x = zm_padding;
	for (WORD i = 0; i < zm_nColumn; i++ )
	{		
		int widthRect = zm_nColumSize[i];
		if ( i == 0 )
			widthRect -= 10;

		pG->drawTextInRect( x+10,y,widthRect, zm_itemHeight,  *(zm_colText[i]), DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		x += zm_nColumSize[i];
	}

	zm_rect.left	= zm_padding;
	zm_rect.top		= y;
	zm_rect.right	= zm_control->getClientWidth() ;
	zm_rect.bottom	= y + zm_itemHeight;
}

bool uiListPropertyRow::checkHit( int x, int y )
{
	zm_activeCol = -1;

	if ( zm_visible == false )
		return false;

	if ( isHitOnCol(x,y) == -1 )
	{
		int pos = zm_padding;
		int cellW = 0;

		// check cell hit
		for ( int i = 0; i < zm_nColumn; i++ )
		{
			cellW = zm_nColumSize[i];

			if ( x >= pos && x <= pos + cellW )
			{
				zm_activeCol = i;
				break;
			}
		
			pos += cellW;
		}
	}

	return uiListPropertyItem::checkHit(x,y);
}

// isHitOnColum
// kiem tra xem co click tren col hay ko
int uiListPropertyRow::isHitOnCol(int x, int y)
{
	zm_colClick = -1;	
	for ( int i = 0; i < zm_nColumn; i++ )
	{
		if ( (x <= zm_colPos[i] + 2) && (x >= zm_colPos[i] - 2) )
		{
			zm_colClick = i;			
			break;
		}
	}
	return zm_colClick;
}

int uiListPropertyRow::getColClick()
{
	return zm_colClick;
}

// getColumnPos
// lay vi tri col hien tai
int uiListPropertyRow::getColumnPos( int col )
{
	if ( col < 0 || col >= zm_nColumn )
		return 0;

	return zm_colPos[ col ];
}

// getColumnWidth
// lay kich thuoc cot
int uiListPropertyRow::getColumnWidth( int col )
{
	if ( col < 0 || col >= zm_nColumn )
		return 0;

	return zm_nColumSize[ col ];
}

// setActiveCol
// thiet lap col duoc active
void uiListPropertyRow::setActiveCol( int col )
{
	zm_activeCol = col;
}

// getActiveCol
// lay col duoc active
int uiListPropertyRow::getActiveCol()
{
	return zm_activeCol;
}

// setFocus
// thiet lap focus cho item
void uiListPropertyRow::setFocus(bool b)
{
	if ( b == zm_focus )
		return;

	uiListPropertyItem::setFocus( b );

	// auto focus
	if ( zm_activeCol == -1 )
	{
		for ( int i = 0; i < zm_nColumn; i++ )
		{
			if ( zm_colWindow[i] )
			{
				zm_activeCol	= i;
				break;
			}
		}
	}

	for ( int i = 0; i < zm_nColumn; i++ )
	{
		if ( zm_colWindow[i] )
		{			
			uiEdit* pEdit = NULL;
			uiComboBox* pComboBox = NULL;	

			if ( zm_colWindow[i]->getObject() == UIOBJECT_EDIT )			
				pEdit = (uiEdit*)zm_colWindow[i];
			else if ( zm_colWindow[i]->getObject() == UIOBJECT_COMBOBOX )
				pComboBox = (uiComboBox*)zm_colWindow[i];

			if ( b == true && zm_activeCol == i )
			{			
				zm_colWindow[i]->showWindow(true);
				zm_colWindow[i]->setFocus();
				
				if ( pEdit )
					pEdit->setText( zm_colText[i].c() );	

			}
			else
			{
				zm_colWindow[i]->showWindow(false);
				if ( pEdit )
				{
					WCHAR lpText[1024] = {0};
					pEdit->getText(lpText, 1024);
					zm_colText[i] = lpText;
				}
				if ( pComboBox )
				{
					WCHAR lpText[1024] = {0};
					pComboBox->getSelectString( lpText );
					zm_colText[i] = lpText;
				}

			}
		}
	}	

}

void uiListPropertyRow::setText( LPWSTR lpString, int col )
{
	if ( col < 0 || col >= zm_nColumn )
		return;

	// set string for this row
	zm_colText[ col ] = lpString;
}

void uiListPropertyRow::getText( LPWSTR lpString, int col )
{
	if ( col < 0 || col >= zm_nColumn )
	{
		lpString[0] = 0;
		return;
	}
	
	uiString::copy<WCHAR,WCHAR>( lpString, zm_colText[ col ].c() );

}

uiWindow* uiListPropertyRow::getControl( int col )
{
	if ( col < 0 || col >= zm_nColumn )
		return NULL; 
	
	return zm_colWindow[col];
}

// setControl
// thiet lap control tai cot vi tri
// controlType = 0 (textEdit)
// controlType = 1 (comboBox)
// controlType = 3 (custom control)
uiWindow* uiListPropertyRow::setControl( int controlType, int col, uiWindow *pControl )
{
	if ( col < 0 || col >= zm_nColumn )
		return NULL; 

	if ( zm_colWindow[col] != NULL )
		delete zm_colWindow[col];

	switch ( controlType )
	{
	case UILISTPROPERTY_EDIT:
		{
			uiEdit *pEdit = new uiListPropertyEdit( zm_control);
			pEdit->setEventOnChange<uiListProperty, &uiListProperty::_onEditChange>( zm_control );
			pEdit->setText( (LPWSTR)zm_colText[ col ].c() );

			zm_colWindow[col] = pEdit;
			zm_colUsercontrol[col] = false;			
			
			zm_colWindow[col]->showWindow( false );			
			return zm_colWindow[col];
		}		
	case UILISTPROPERTY_COMBOBOX:
		{
			uiListPropertyComboBox *pCmbBox = new uiListPropertyComboBox( zm_control );
			pCmbBox->setEventOnSelectChange<uiListProperty, &uiListProperty::_onCmbChange>( zm_control );
			pCmbBox->setCaption( (LPWSTR)zm_colText[ col ].c() );

			zm_colUsercontrol[col] = false;
			zm_colWindow[col] = pCmbBox;
			zm_colWindow[col]->showWindow( false );
			return zm_colWindow[col];
		}		
	case UILISTPROPERTY_CUSTOM:
	default:
		{
			zm_colWindow[col] = pControl;
			pControl->setCaption( (LPWSTR)zm_colText[ col ].c() );

			zm_colUsercontrol[col] = true;
			return pControl;
		}		
	};
}