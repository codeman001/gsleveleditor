// uiSplitContainer.cpp

// Lap trinh vien: Pham Hong Duc
// Email: duc.phamhong@gameloft.com
// Ngay viet: 12/10/2011
// Thong tin:
//

#include "uiListProperty.h"
#include "uiListPropertyGroup.h"
#include "uiListPropertyRow.h"
#include "uiInclude.h"

using namespace uiClass;

uiListProperty::uiListProperty(LPWSTR lpTitle,int x, int y, int w, int h, uiWindow* pParent, int col)	
	:uiWindow(lpTitle,x,y,w,h, pParent),
	zm_cursor( MAKEINTRESOURCEW(UICURSOR_ARROW), true )
{
	zm_scrollY = 0;
	zm_col = col;

	zm_hitItem = NULL;
	zm_activeItem = NULL;
	zm_mouseDown = false;

	zm_oldX = 0;
	zm_oldY = 0;
	zm_hItemDraw = 0;
	zm_eraseDrag = false;

	zm_showVScroll = false;
	zm_needUpdateScroll = false;

	zm_lastHeight = -1;

	// enable scroll bar
	changeStyle( WS_VSCROLL, NULL );

	setScrollPos(SB_VERT, 0);
	showScrollBarVer( false );

	// for look all event to this window
	uiWindow::listenThisAppEvent( this );
}

uiListProperty::~uiListProperty(void)
{
	// remove look event on this window
	uiWindow::unListenThisAppEvent( this );

	deleteAllItem();
}

bool uiListProperty::globalMessageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	if ( hWnd != this->getHandle() )
	{
		if ( 
				uMsg == WM_LBUTTONDOWN || 
				uMsg == WM_RBUTTONDOWN					
			)
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);	

			POINT p = {x,y};
			ClientToScreen(hWnd,&p);
			x = p.x;
			y = p.y;

			int winX = 0, winY = 0;
			this->getPointToScreen( &winX, &winY );
			
			int w,h;
			this->getClientWH( &w, &h );

			if ( x < winX || y < winY || x > (winX + w) || y > (winY + h ) )
				lostFocus();

		}
		else if ( uMsg == WM_KEYUP )
		{
			// if active row item
			if ( zm_activeItem && zm_activeItem->getObjectType() == 2 )
			{
				uiListPropertyRow *pRow = (uiListPropertyRow*) zm_activeItem;
							
				UINT keyCode = (UINT)wParam;

				// rever old data if press esc
				if ( keyCode == VK_ESCAPE )
				{
					// get old value
					vector<uiStringW> aText;
					WCHAR lpText[1024];

					for ( int i = 0; i < zm_col; i++ )
					{			
						pRow->getText( lpText, i );
						aText.push_back(lpText);
					}
					
					// lost focus
					pRow->setFocus(false);

					// revert data
					for ( int i = 0; i < zm_col; i++ )
					{			
						pRow->setText( aText[i].c(), i );
					}

					zm_activeItem = NULL;
					
				}
				// lost focus if press enter
				else if ( keyCode == VK_RETURN || keyCode == VK_TAB )
				{					
					uiListPropertyRow *pOldActive = pRow;

					lostFocus();
					
					bool focusNextItem = false;

					if ( keyCode == VK_TAB )
					{
						int col = pOldActive->getActiveCol();
						bool nextCol = true;
	
						// focus to next col
						for ( int i = col + 1; i < zm_col; i++ )
						{
							if ( pOldActive->getControl(i) )
							{
								zm_activeItem = pOldActive;

								pOldActive->setActiveCol( i );
								pOldActive->setFocus( true );

								reDrawInterface();

								nextCol = false;
							}
						}

						if ( nextCol == true )
							focusNextItem = true;
					}

					if ( keyCode == VK_RETURN || focusNextItem == true )
					{
						list<uiListPropertyItem*>::iterator i = zm_listItem.begin(), end = zm_listItem.end();
						
						while ( i != end )
						{
							if ( (*i) == pOldActive )
							{
								i++;
								break;
							}
							i++;
						}
						
						// get to begin if last
						if ( i == zm_listItem.end() )
							i = zm_listItem.begin();
						
						if ( (*i) != pOldActive )
						{							
							zm_activeItem = (*i);							
							zm_activeItem->setFocus( true );
							reDrawInterface();
						}
					}
				}
			}
		}
		else if ( uMsg == WM_MOUSEWHEEL )
		{
			//if ( zm_activeItem )
			//{
			//	int wheelPos = 0, min = 0, max = 0;

			//	int nDelta = (int) wParam;
			//	if (nDelta > 0)
			//	{
			//		// wheel up
			//		wheelPos--;
			//	}
			//	else
			//	{
			//		// wheel down
			//		wheelPos++;
			//	}

			//	if ( wheelPos < min )
			//		wheelPos = min;
			//	if ( wheelPos > max )
			//		wheelPos = max;
			//	
			//	uiScrollBarEvent scrollEvent(this, wheelPos, min, max);
			//	_OnVScroll( scrollEvent );
			//}
		}
	}
	return true;
}

// lostFocus
// cancel edit cac property
void uiListProperty::lostFocus()
{
	eventOnDeActive(this);

	if ( zm_activeItem )
	{
		zm_activeItem->setFocus(false);
		zm_activeItem = NULL;
		eventOnUpdateProperty( this );
	}	
}

// onCmbChange
// su kien khi cmb tren property change
void uiListProperty::_onCmbChange( uiObject *pSender )
{
	eventOnCmbChange( pSender );
}

// _onEditChange
// su kien khi edit text thay doi
void uiListProperty::_onEditChange( uiObject *pSender )
{
	eventOnEditChange( pSender );
}

// reDrawInterface
// ve lai doi tuong
void uiListProperty::reDrawInterface()
{
	_OnPaint( &uiGraphics(this) );
}

// _OnPaint
// Wzm_PAINT
void uiListProperty::_OnPaint( uiGraphics* pG )
{
	int height	= this->getClientHeight();
	int width	= this->getClientWidth();

	if ( zm_lastHeight != height )
	{
		zm_scrollY = 0;
		zm_needUpdateScroll = true;
		zm_lastHeight = height;
		setScrollPos( SB_VERT, 0 );
	}

	uiGraphics* pBuffer = pG->createBackBuffer( width, height );
	
	uiBrush b( uiColor::fromDWORD(0x00FFFFFF) );
	pBuffer->drawFillRectangle( 0,0, this->getClientWidth(), this->getClientHeight(), &b );
	
	int y = zm_scrollY;
	int drawHeight = 0;

	// draw all group
	list<uiListPropertyItem*>::iterator i = zm_listItem.begin(), end = zm_listItem.end();

	while ( i != end )
	{
		if ( (*i)->isVisible() == true )
		{			
			(*i)->drawItem( pBuffer, y );

			y += uiListPropertyItem::sm_itemHeight;
			drawHeight += uiListPropertyItem::sm_itemHeight;
		}
		i++;
	}
	
	if ( drawHeight >= height )
	{				
		if ( zm_showVScroll == false )
		{
			showScrollBarVer( true );
			zm_showVScroll = true;
			zm_needUpdateScroll = true;
		}
		
		// set range
		// update min & max need to scroll
		if ( zm_needUpdateScroll )
		{
			int nItemsHide = ((y - height)/ uiListPropertyItem::sm_itemHeight) + 1;
			setScrollRange( SB_VERT, 0, nItemsHide );
			zm_needUpdateScroll = false;
		}
	}
	else
	{
		setScrollPos(SB_VERT, 0);
		showScrollBarVer( false );
		zm_showVScroll = false;
		zm_needUpdateScroll = true;
		zm_scrollY = 0;
	}

	pG->swapBuffer(0,0, this->getClientWidth(), this->getClientHeight(), pBuffer, 0,0,this->getClientWidth(), this->getClientHeight(), SRCCOPY );
	pBuffer->releaseGraphics();
}

// Wzm_VSCROLL
void uiListProperty::_OnVScroll( uiScrollBarEvent scrollEvent )
{
	int pos = scrollEvent.getPos();
	zm_scrollY = -pos * uiListPropertyItem::sm_itemHeight;
	
	if ( zm_activeItem && zm_activeItem->getObjectType() == 2 )
	{
		((uiListPropertyRow*)zm_activeItem)->setFocus( false );
	}

	// update interface
	reDrawInterface();
}

// Wzm_MOUSEMOVE
void uiListProperty::_OnMouseMove( uiMouseEvent e, int x, int y)
{
	// drag
	if ( zm_mouseDown == true )
	{
		if ( zm_hitItem )
		{
			if ( zm_hitItem->getObjectType() == 2 )
			{
				uiListPropertyRow *pRow = (uiListPropertyRow*) zm_hitItem;

				int colDrag = pRow->getColClick();
				if ( colDrag >= 0 )
				{
					
					int px = pRow->getColumnPos( colDrag );
					int w1 = pRow->getColumnWidth( colDrag )		- 20;
					int w2 = pRow->getColumnWidth( colDrag + 1 )	+ 20;

					// Neu keo ra khoi pham vi can thiet
					if ( x < (px - w1) || x > (px + w2) )
						return;										

					int padding = pRow->getColumnPos(0) - pRow->getColumnWidth(0);
					int delta = x - px + padding;

					int width1 = w1 + delta;
					int width2 = w2 - delta;

					// New kich thuoc qua nho
					if ( width1 < 20 || width2 < 20 )
						return;

					// Thiet lap kich thuoc cho cot
					this->setItemColSize( colDrag, width1 );
					this->setItemColSize( colDrag + 1, width2 );	
					
					
					this->reDrawInterface();
				}
			}
		}
	}
	// mouse over
	else
	{		
		zm_hitItem = getHitTest( x,y );

		// Load cursor kieu Arrow
		zm_cursor.loadFromResource( IDC_ARROW );

		if ( zm_hitItem )
		{	
			int colDrag = -1;
			if ( zm_hitItem->getObjectType() == 2 )
			{
				uiListPropertyRow *pRow = (uiListPropertyRow*) zm_hitItem;
				colDrag = pRow->getColClick();
			}

			// Neu dang overmouse len column line thi chuyen chuot thanh <->
			if ( colDrag != -1 )		
				zm_cursor.loadFromResource( IDC_SIZEWE );	
		}
		
		this->setCursor( &zm_cursor );		
	}

}

// getItemColSize
// lay chieu rong cua cot
int uiListProperty::getItemColSize( int col )
{
	list<uiListPropertyItem*>::iterator it = zm_listItem.begin(), end = zm_listItem.end();
	while ( it != end )
	{				
		if ( (*it)->getObjectType() == 2 )
		{
			return ((uiListPropertyRow*) (*it))->getColumnWidth( col );
		}
		it++;
	}

	return 0;
}

void uiListProperty::setItemColSize( int col, int size )
{	
	list<uiListPropertyItem*>::iterator it = zm_listItem.begin(), end = zm_listItem.end();
	while ( it != end )
	{		
		(*it)->setColSize( col, size );	

		if ( (*it)->getObjectType() == 2 )
			((uiListPropertyRow*) (*it))->setActiveCol( -1 );
		it++;
	}
}

// getHitTest
// lay doi tuong dang click
uiListPropertyItem* uiListProperty::getHitTest(int x, int y)
{
	list<uiListPropertyItem*>::iterator it = zm_listItem.begin(), end = zm_listItem.end();
	while ( it != end )
	{
		if ( (*it)->checkHit( x,y ) == true )
		{
			return (*it);
		}
		it++;
	}
	return NULL;
}

// Wzm_LBUTTONDOWN
void uiListProperty::_OnLButtonDown ( uiMouseEvent e, int x, int y )
{
	if ( zm_activeItem == NULL )
		eventOnActive(this);

	// tim item duoc click
	zm_hitItem = getHitTest(x,y);
		
	// in active this item
	if ( zm_activeItem )
	{
		zm_activeItem->setFocus(false);
		eventOnUpdateProperty(this);
	}

	// new click vao 1 item nao do
	if ( zm_hitItem )
	{
		int colDrag  = -1;
		
		// new click len colum
		if ( zm_hitItem->getObjectType() == 2 )
		{
			uiListPropertyRow *pRow = (uiListPropertyRow*) zm_hitItem;
			colDrag = pRow->getColClick();
		}

		if ( colDrag == -1 )
		{
			zm_activeItem = zm_hitItem;		
			zm_activeItem->setFocus(true);
		}		
	}
	
	// repaint
	this->setCapture();
	this->reDrawInterface();

	zm_mouseDown = true;
}

// Wzm_SIZE
void uiListProperty::_OnSize( uiSizeEvent e, int w, int h )
{
	list<uiListPropertyItem*>::iterator it = zm_listItem.begin(), end = zm_listItem.end();
	while ( it != end )
	{
		if ( (*it)->getObjectType() == 2 )
			((uiListPropertyRow*) (*it))->setActiveCol( -1 );
		it++;
	}

	this->reDrawInterface();
}

// Wzm_LBUTTONUP
void uiListProperty::_OnLButtonUp( uiMouseEvent e, int x, int y )
{	
	this->releaseCapture();
	zm_mouseDown = false;
	zm_hitItem = NULL;
}

// getGroupCount
// lay so luong group
int uiListProperty::getItemCount()
{
	return (int)zm_listItem.size();
}

// getGroup
// lay group thu i
uiListPropertyItem* uiListProperty::getItem(int i)
{
	if ( i >= (int)zm_listItem.size() || i < 0 )
		return NULL;

	list<uiListPropertyItem*>::iterator it = zm_listItem.begin(), end = zm_listItem.end();
	int nPos = 0;

	while ( it != end )
	{
		if ( nPos == i )
		{
			return (*it);			
		}

		nPos++;
		it++;
	}

	return NULL;
}

// addGroup
// add group thu i
uiListPropertyGroup* uiListProperty::addGroup(LPWSTR lpLabel)
{
	uiListPropertyGroup *pGroup = new uiListPropertyGroup( this, NULL, lpLabel, zm_col );
	
	zm_listItem.push_back( pGroup );

	return pGroup;
}


// addRowItem
// them row 
uiListPropertyRow*	uiListProperty::addRowItem(LPWSTR lpLabel)
{
	uiListPropertyRow *pRow = new uiListPropertyRow( this, NULL, lpLabel, zm_col );
	
	zm_listItem.push_back( pRow );

	return pRow;
}

// deleteAllGroup
// xoa toan bo group
void uiListProperty::deleteAllItem()
{
	list<uiListPropertyItem*>::iterator i = zm_listItem.begin(), end = zm_listItem.end();
	
	while ( i != end )
	{
		delete (*i);
		i++;
	}
	
	zm_listItem.clear();

	setScrollPos(SB_VERT, 0);
	showScrollBarVer( false );
	zm_showVScroll = false;	
	zm_scrollY = 0;
}

// deleteItem
// xoa item thu i
void uiListProperty::deleteItem( int i )
{
	if ( i >= (int)zm_listItem.size() || i < 0 )
		return;

	list<uiListPropertyItem*>::iterator it = zm_listItem.begin(), end = zm_listItem.end();
	int nPos = 0;

	while ( it != end )
	{
		if ( nPos == i )
		{
			delete (*it);
			zm_listItem.erase( it );
			return;
		}

		nPos++;
		it++;
	}
}

// getActiveItem
// lay item dang focus
uiListPropertyItem* uiListProperty::getActiveItem()
{
	return zm_activeItem;
}
