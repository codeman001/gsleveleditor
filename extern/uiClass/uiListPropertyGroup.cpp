#include "uiListPropertyGroup.h"
#include "uiListPropertyItem.h"
#include "uiListProperty.h"
#include "uiInclude.h"

using namespace uiClass;

// mapPropertyGroupItem
// Constructor
uiListPropertyGroup::uiListPropertyGroup( uiListProperty *pControl, uiListPropertyItem* pParent, LPWSTR lpLabel, int nColumn )
	:uiListPropertyItem(pControl, pParent, nColumn),
	zm_font(13,L"Tahoma"),
	zm_colText(nColumn)
{
	setLabel( lpLabel );

	// set background color
	zm_bgColor = uiColor::fromDWORD(0x00DFDFDF);
	zm_textColor = uiColor::fromDWORD(0x00656565);

	zm_font.setFontBold();

	zm_textPadding = 20;

	zm_objectType = 1;

	zm_enableColText = false;
	for ( int i = 0; i < nColumn; i++ )
	{
		zm_colText[i] = L"";
	}
}

uiListPropertyGroup::~uiListPropertyGroup()
{
}

// drawGroup
// ve group
void uiListPropertyGroup::drawItem( uiGraphics* pG, int y )
{
	uiBrush brush( zm_bgColor );
	
	int width = zm_control->getClientWidth();

	// To mau nen
	pG->drawFillRectangle( 0,y, width,y + zm_itemHeight, &brush );

	pG->setTextBkTransparent();
	
	pG->setTextColor( zm_textColor );
	pG->selectObject( &zm_font );

	if ( zm_enableColText == false )	
		pG->drawTextInRect( zm_textPadding, y, width, zm_itemHeight, getLabel(), DT_LEFT|DT_VCENTER|DT_SINGLELINE );
	else
	{
		int x = zm_textPadding;
		int paddingX = 10;

		for ( int i = 0; i < zm_nColumn; i++ )
		{
			pG->drawTextInRect( x + paddingX, y, zm_nColumSize[i] - paddingX, zm_itemHeight, zm_colText[i].c(), DT_LEFT|DT_VCENTER|DT_SINGLELINE );
			x += zm_nColumSize[i];
		}
	}

	// Xac dinh rect -> De check hittest
	zm_rect.left	= 0;
	zm_rect.top		= y;
	zm_rect.right	= width;
	zm_rect.bottom	= y + zm_itemHeight;
}

// enableColText
// thiet lap group hien thi text theo column
void uiListPropertyGroup::enableColText( bool b )
{
	zm_enableColText = b;
}

// setColText
// thiet lap text cho column
void uiListPropertyGroup::setColText( WCHAR *lpString, int col )
{
	zm_colText[col] = lpString;
}