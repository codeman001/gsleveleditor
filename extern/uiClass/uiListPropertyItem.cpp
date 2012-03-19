#include "uiListProperty.h"
#include "uiListPropertyItem.h"
#include "uiInclude.h"

using namespace uiClass;

uiListPropertyItem::uiListPropertyItem(uiListProperty* control, uiListPropertyItem *pParent, int nCol )
	:zm_nColumSize(nCol)
{
	zm_control = control;
	zm_parent = pParent;

	zm_nColumn = nCol;

	int width = control->getClientWidth();
	int aWidth = width / nCol;

	for ( int i = 0; i < nCol; i++ )
		zm_nColumSize[i] = aWidth;

	zm_itemHeight = sm_itemHeight;	// default height
	zm_visible = true;
	zm_focus = false;
	zm_objectType = 0;
}

uiListPropertyItem::~uiListPropertyItem()
{
}

// setLabel
// Dat label cho group item
void uiListPropertyItem::setLabel( LPWSTR lpLabel )
{
	zm_label = lpLabel;
}

// getLabel
// Lay chuoi ky tu label
LPWSTR uiListPropertyItem::getLabel()
{
	return zm_label.c();
}

// setHeight
// thiet lap chieu cao cho height
void uiListPropertyItem::setHeight( int height )
{
	zm_itemHeight = height;
}

// getHeight
// lay chieu cao
int uiListPropertyItem::getHeight()
{
	return zm_itemHeight;
}

// setParent
// set parent
void uiListPropertyItem::setParent( uiListPropertyItem* parent )
{
	zm_parent = parent;
}

// setRect
// thiet lap rect
void uiListPropertyItem::setRect( int x, int y, int w, int h )
{
	zm_rect.left = x;
    zm_rect.top = y;
    zm_rect.right = x + w;
    zm_rect.bottom = y + h;
}

void uiListPropertyItem::setVisible(bool b)
{
	zm_visible = b;
}

bool uiListPropertyItem::isVisible()
{
	if ( zm_visible == false )
		return false;

	uiListPropertyItem *p = zm_parent;
	
	while ( p )
	{
		if ( p->zm_visible == false )
			return false;

		p = p->zm_parent;
	}

	return true;
}

// setFocus
// thiet lap focus cho item
void uiListPropertyItem::setFocus(bool b)
{
	zm_focus = b;
}

// isFocus
// kiem tra item co focus hay ko
bool uiListPropertyItem::isFocus()
{
	return zm_focus;
}

bool uiListPropertyItem::checkHit( int x, int y )
{
	POINT point;
	point.x = x;
	point.y = y;

	bool bHit = (bool)(PtInRect(&zm_rect,point) != 0 );

	return bHit;
}

// setColSize
// thiet lap chieu cao cho item
void uiListPropertyItem::setColSize(int col, int size)
{
	if ( col < 0 || col >= zm_nColumn)
		return;

	zm_nColumSize[col] = size;
}