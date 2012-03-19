// uiSplitContainer.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 6/1/2009
// Thong tin:
//

#include "uiSplitContainer.h"
#include "uiInclude.h"

using namespace uiClass;

uiSplitContainer::uiSplitContainer(LPWSTR lpTitle,int x, int y, int w, int h, uiWindow* pParent, DWORD numRow, DWORD numCol)
	:uiWindow(lpTitle,x,y,w,h, pParent)
{	

	assert( numRow );
	assert( numCol );

	zm_row = numRow;
	zm_col = numCol;
	
	// Kich thuoc expander
	zm_nExpanderSize = 3;

	// Kich thuoc minsize
	zm_nMinSize = 20;

	// Tinh kich thuoc trung binh
	DWORD sizeCol = w/numCol - zm_nExpanderSize/2;
	DWORD sizeRow = h/numRow - zm_nExpanderSize/2;

	// Danh sach cac hang
	for (DWORD i = 0; i < numRow; i++)
	{		
		// Them hang
		uiArrayWindows a;
		zm_listChild.push_back( a );		
		
		// Them cot
		for (DWORD j = 0; j < numCol; j++)		
			zm_listChild[i].push_back( NULL );			
	}	

	// Thiet lap kich thuoc mac dinh...
	// Thiet lap kich thuoc cho hang
	for (DWORD i = 0; i < numRow; i++)
		zm_rowSize.push_back( sizeRow );

	// Thiet lap kich thuoc cho cot
	for (DWORD i = 0; i < numCol; i++)
		zm_colSize.push_back( sizeCol );
	
	zm_isMouseDown = false;
	zm_isDrag = false;
	zm_bErase = false;
}

uiSplitContainer::uiSplitContainer()
{

}

uiSplitContainer::~uiSplitContainer(void)
{
}

// getObject
// Tra ve doi tuong UIOBJECT_CTLSPLIT
DWORD uiSplitContainer::getObject()
{
	return UIOBJECT_CTLSPLIT;
}

// addWindow
// Them 1 cua so vao splitContainer
void uiSplitContainer::setWindow( uiWindow* p, DWORD row, DWORD col )
{
	if ( row >= zm_row || col >= zm_col)
		return;

	zm_listChild[row][col] = p;
}
		
// removeWindow
// Loai bo cua so
void uiSplitContainer::removeWindow( DWORD row, DWORD col )
{
	if ( row >= zm_row || col >= zm_col)
		return;

	zm_listChild[row][col] = NULL;
}

// getWindow
// Lay cua so
uiWindow* uiSplitContainer::getWindow( DWORD row, DWORD col )
{
	if ( row >= zm_row || col >= zm_col)
		return NULL;
	
	return zm_listChild[row][col];
}

// getRows
// Tra ve so hang
DWORD uiSplitContainer::getRows()
{
	return zm_row;
}
		
// Tra ve so cot
DWORD uiSplitContainer::getCols()
{
	return zm_col;
}

// setColSize
// Thiet lap size cho col
void uiSplitContainer::setColSize(DWORD col, DWORD size)
{
	if (col >= zm_col)
		return;
	
	zm_colSize[col] = size;
}
		
// setRowSize
// Thiet lap size cho hang
void uiSplitContainer::setRowSize(DWORD row, DWORD size)
{
	if (row >= zm_row)
		return;
	
	zm_rowSize[row] = size;
}

// getColSize
// Lay kich thuoc cua cot
DWORD uiSplitContainer::getColSize(DWORD col)
{
	if (col >= zm_col)
		return 0;

	return zm_colSize[col];
}

// getRowSize
// Lay kich thuoc cua hang
DWORD uiSplitContainer::getRowSize(DWORD row)
{
	if (row >= zm_row)
		return 0;

	return zm_rowSize[row];
}


// setExpanderSize
// Thiet lap kich thuoc cho Expander
void uiSplitContainer::setExpanderSize(DWORD size)
{
	zm_nExpanderSize = size;
}

// getExpanderSize
// Lay kich thuoc Expander
DWORD uiSplitContainer::getExpanderSize()
{
	return zm_nExpanderSize;
}

// setMinSize
// Thiet lap kich thuoc nho nhat cua cua so
void uiSplitContainer::setMinSize(DWORD size)
{
	zm_nMinSize = size;
}

// getMinSize
// Lay kich thuoc nho nhat cua cua so
DWORD uiSplitContainer::getMinSize()
{
	return zm_nMinSize;
}


// update
// Cap nhat kich thuoc 
void uiSplitContainer::updateSplit()
{
	DWORD i,j;
	DWORD w,h;
	DWORD x = 0,y = 0;


	for ( i = 0; i < zm_row; i++ )
	{
		// Kich thuoc hang thu i
		h = this->getRowSize(i);
		for ( j = 0; j < zm_col; j++ )
		{
			// Kich thuoc cot thu j
			w = this->getColSize(j);
			
			// Lay cua so
			uiWindow *pWin = this->getWindow( i,j );

			// Neu co ton tai cua so
			if ( pWin )
			{
				pWin->setParent( this );
				int l = 0, r = 0, t = 0, b = 0;
				pWin->getMargin(&l, &r, &t, &b);
				pWin->moveWindow( x + l, y + t, w - r , h - b );
			}
			
			x += this->getColSize(j);
			x += this->getExpanderSize();
		}

		y += this->getRowSize(i);
		y += this->getExpanderSize();
		
		x = 0;		
	}

}

// drawPatternInvert
// Ve vien mo khi di chuyen chuot
void uiSplitContainer::drawPatternInvert(int x, int y, int w, int h)
{
	uiGraphics g;

	WORD dotPatternBmp[] = {0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055};
	uiBitmap bm(8,8,1,1, dotPatternBmp);
	uiBrush b;
	b.createPatternBrush( &bm );

	HGDIOBJ obj = g.selectObject( &b );

	g.drawPatternInvert( x,y, w,h );

	g.selectObject (obj);
}

// getClient
// Lay kich thuoc cua o
bool uiSplitContainer::getCellRect(int row, int col, int countR, int countC, RECT *rect)
{
	assert( countR );
	assert( countC );

	if ( ((DWORD)row >= zm_row) || ( (DWORD)col >= zm_col) || 
		( (DWORD)(row + countR) > zm_row) ||  ( (DWORD)(col + countC) > zm_col) )
		return false;

	DWORD x = 0,y = 0, w = 0, h = 0;
	DWORD r,c,i,j;
	
	// Duyet cac row de tinh toa do y
	for ( r = 0; r < zm_row; r++ )
	{
		if ( r == row )
			break;

		y += this->getRowSize(r);
		y += this->getExpanderSize();		
	}
	
	// Duyet cac col de tinh toa do x
	for ( c = 0; r < zm_col; c++ )
	{
		if ( c == col )
			break;

		x += this->getColSize(r);
		x += this->getExpanderSize();		
	}

	// Tinh chieu cao H
	for ( i = r; i < r + countR; i ++ )
	{
		if ( this->getWindow( i, col ) )
			h += this->getRowSize(i);
		else	
		{
			h = this->getClientHeight() - y;
			break;
		}
		if ( i != r + countR - 1 )
			h += this->getExpanderSize();
	}
	
	// Tinh chieu rong W
	for ( j = c; j < c + countC; j++ )
	{
		if ( this->getWindow( row, j ) )
			w += this->getColSize(j);
		else
		{
			w = this->getClientWidth() - x;
			break;
		}

		if ( j != c + countC - 1 )
			w += this->getExpanderSize();
	}
	
	rect->left		= x;
	rect->top		= y;
	rect->right		= x + w;
	rect->bottom	= y + h;

	return true;
}

// getHitest
// Kiem tra chuot dang o tren expander nao
// Tham so:
//		x,y: Toa do chuot
//		r,c: Ket qua tra ve hang, cot
// Return:
//		-1: Loi
//		0: expander ngang
//		1: expander doc
//		2: expander trung tam
int uiSplitContainer::getHittest(int x, int y, DWORD *r, DWORD *c)
{
	bool isHoverH=false, isHoverV=false;
	DWORD row = 0, col = 0;
	DWORD current = 0;
	DWORD i;
	
	// Kiem tra hang
	for ( i = 0; i < zm_row; i++ )
	{
		current += this->getRowSize(i);
		
		// Kiem tra vi tri chuot so voi expander
		if ( (DWORD)y >= current && (DWORD)y <= current + this->getExpanderSize() )
		{
			isHoverV = true;
			row = i;
			break;
		}

		current += this->getExpanderSize();
	}

	// Kiem tra cot
	current = 0;

	for ( i = 0; i < zm_col; i++ )
	{
		current += this->getColSize(i);

		// Kiem tra vi tri chuot so voi expander
		if ( (DWORD)x >= current && (DWORD)x <= current + this->getExpanderSize() )
		{
			isHoverH = true;
			col = i;
			break;
		}

		current += this->getExpanderSize();
	}
	
	*r = row;
	*c = col;

	// Lua chon o expander trung tam
	if ( isHoverH && isHoverV )
		return 2;

	// Expander doc
	if ( isHoverH )
		return 1;

	// Expander ngang
	if ( isHoverV )
		return 0;

	return -1;
}

// setCursorIcon
// Thiet lap icon cho mouse
void uiSplitContainer::setCursorIcon( int r )
{
	static uiCursor c( UICURSOR_ARROW, true );

	// Thanh ngang
	if ( r == 0 )
	{
		c.loadFromResource( UICURSOR_SIZENS );
		this->setCursor( &c );
		return;
	}

	// Danh doc
	if ( r == 1 )
	{
		c.loadFromResource( UICURSOR_SIZEWE );
		this->setCursor( &c );
		return;
	}
	
	// Vi tri giua ( cursor hinh dau + )
	if ( r == 2)
	{
		c.loadFromResource( UICURSOR_SIZEALL );
		this->setCursor( &c );
		return;
	}

}

// getLimitRect
// Lay vi tri point bi gioi han khi keo
void uiSplitContainer::getLimitRect( int *px, int *py )
{
	int x = zm_clientExpander.left;
	int y = zm_clientExpander.top;
	int w = zm_clientExpander.right - zm_clientExpander.left;
	int h = zm_clientExpander.bottom - zm_clientExpander.top;
	
	int minSize = (int)this->getMinSize();
	this->getPointToScreen( &x, &y );

	if ( *px <= x + minSize )
		*px = x + minSize;

	if ( *py <= y + minSize )
		*py = y + minSize;

	if ( *px >= x + w - minSize )
		*px = x + w - minSize;

	if ( *py >= y + h - minSize )
		*py = y + h - minSize;
}


void uiSplitContainer::_OnPaint( uiGraphics* pG )
{
	uiWindow::_OnPaint( pG );
}

void uiSplitContainer::_OnSize( uiSizeEvent e, int w, int h )
{
	DWORD i=0,j=0;
	DWORD currentWidth=0,currentHeight=0;

	// Tinh tong kich thuoc cac o
	for (i = 0; i < zm_row-1; i++)
	{
		currentHeight += this->getRowSize(i);
		currentHeight += this->getExpanderSize();

		for (j = 0; j < zm_col-1; j++)
		{
			currentWidth += this->getColSize(j);
			currentWidth += this->getExpanderSize();			
		}
	}
	
	// Truong hop chi co 1 hang
	if ( zm_row == 1 )
	{
		for (j = 0; j < zm_col-1; j++)
		{
			currentWidth += this->getColSize(j);
			currentWidth += this->getExpanderSize();
		}
	}

	// Thiet lap kich thuoc cho hang va cot cuoi cung
	this->setRowSize(i, h - currentHeight);
	this->setColSize(j, w - currentWidth);
	this->updateSplit();
	
}

void uiSplitContainer::_OnMouseMove(uiMouseEvent e, int MouseX, int MouseY)
{	
	// Kiem tra vi tri chuot
	if ( !zm_isMouseDown )
	{
		zm_nHit = this->getHittest( MouseX,MouseY, &zm_nCurrentRow, &zm_nCurrentCol );
		
		// Loi!!! ....
		if ( zm_nHit == -1 && !zm_isMouseDown)
			return;		

		// Thiet lap icon cho mouse tuong ung
		this->setCursorIcon( zm_nHit );
	}	

	// Neu keo chuot
	if ( zm_isMouseDown )
	{
		int x,y;
		zm_isDrag = true;

		// Kiem tra trang thai keo chuot - doc hay ngang .. 
		// Keo ngang
		if ( zm_nHit == 0 )
			this->getCellRect( zm_nCurrentRow,zm_nCurrentCol, 2,1, &zm_clientExpander );
		// Keo doc
		else if ( zm_nHit == 1 )
			this->getCellRect( zm_nCurrentRow,zm_nCurrentCol, 1,2, &zm_clientExpander );
		// Keo tat ca
		else if ( zm_nHit == 2 )
			this->getCellRect( zm_nCurrentRow,zm_nCurrentCol, 2,2, &zm_clientExpander );
		else
			return;
		///////////////////////////////////////////////////////


		// Kich thuoc cua cua so gioi han keo split ///////////
		x = zm_clientExpander.left;
		y = zm_clientExpander.top;	
		///////////////////////////////////////////////////////

		// Chuyen toa do tu cua so ra man hinh ////////////////
		this->getPointToScreen( &x, &y );
		this->getPointToScreen( &MouseX, &MouseY );					
		///////////////////////////////////////////////////////

		// Chinh sua kich thuoc gioi han split ////////////////
		this->getLimitRect( &MouseX, &MouseY );		
		///////////////////////////////////////////////////////

		// Keo ngang ... //////////////////////////////////////
		if ( zm_nHit == 0 || zm_nHit == 2)						
			this->drawPatternInvert( x, MouseY, this->getClientWidth(), this->getExpanderSize());		

		// Keo doc  ... ////////////////////////////////////////
		if ( zm_nHit == 1 || zm_nHit == 2 )
			this->drawPatternInvert( MouseX, y, this->getExpanderSize(), this->getClientHeight());
		
		///////////////////////////////////////////////////////


		// Xoa vien cu
		this->eraseBg(zm_nHit);

		// Luu toa do
		zm_oldX = MouseX;
		zm_oldY = MouseY;

		// Cho phep len sau erase
		zm_bErase = true;
	}
}

// eraseBg
// Xoa dau vet da draw luc truoc
void uiSplitContainer::eraseBg(int hit)
{
	if ( zm_bErase )
	{
		int x = zm_clientExpander.left;
		int y = zm_clientExpander.top;		
		
		this->getPointToScreen( &x, &y );

		if ( hit == 0 || hit == 2)
			this->drawPatternInvert( x, zm_oldY, this->getClientWidth(), this->getExpanderSize());

		if ( hit == 1 || hit == 2 )
			this->drawPatternInvert( zm_oldX, y, this->getExpanderSize(), this->getClientHeight() );
			
	}
}

void uiSplitContainer::_OnLButtonUp(uiMouseEvent e, int x, int y)
{	
	zm_isMouseDown = false;
	
	// Neu chuot chua duoc keo (click va tha ra)
	if ( !zm_isDrag )
	{
		// Giai phong chuot
		this->releaseCapture();
		return;
	}

	zm_isDrag = false;

	this->eraseBg(zm_nHit);
	zm_bErase = false;

	int w,h;

	// Keo ngang ... ////////////////////////////////////////
	if ( zm_nHit == 0 )
	{
		this->getCellRect( zm_nCurrentRow,zm_nCurrentCol, 2,1, &zm_clientExpander );
		x = zm_oldX;
		y = zm_oldY;
		
		w = zm_clientExpander.right - zm_clientExpander.left;
		h = zm_clientExpander.bottom - zm_clientExpander.top;

		// Chuyen ve toa do
		this->getPointFromScreen( &x,&y );

		// Thiet lap kich thuoc cho hang
		int newH = y - zm_clientExpander.top;

		this->setRowSize( zm_nCurrentRow, newH );
		this->setRowSize( zm_nCurrentRow+1, h - newH - this->getExpanderSize() );		
	}
	
	// Keo doc ... ////////////////////////////////////////
	if ( zm_nHit == 1 )
	{
		this->getCellRect( zm_nCurrentRow,zm_nCurrentCol, 1,2, &zm_clientExpander );
		x = zm_oldX;
		y = zm_oldY;

		w = zm_clientExpander.right - zm_clientExpander.left;
		h = zm_clientExpander.bottom - zm_clientExpander.top;

		// Chuyen ve toa do
		this->getPointFromScreen( &x,&y );

		int newW = x - zm_clientExpander.left;

		// Thiet lap kich thuoc cho hang
		this->setColSize( zm_nCurrentCol, newW );
		this->setColSize( zm_nCurrentCol+1, w - newW - this->getExpanderSize() );	

	}
	
	// Keo ca 2 huong .. ///////////////////////////////////
	if ( zm_nHit == 2 )
	{
		this->getCellRect( zm_nCurrentRow,zm_nCurrentCol, 2,2, &zm_clientExpander );
		x = zm_oldX;
		y = zm_oldY;

		w = zm_clientExpander.right - zm_clientExpander.left;
		h = zm_clientExpander.bottom - zm_clientExpander.top;

		// Chuyen ve toa do
		this->getPointFromScreen( &x,&y );

		int newH = y - zm_clientExpander.top;
		int newW = x - zm_clientExpander.left;

		// Thiet lap kich thuoc
		this->setRowSize( zm_nCurrentRow, newH );
		this->setRowSize( zm_nCurrentRow+1, h - newH - this->getExpanderSize() );	

		this->setColSize( zm_nCurrentCol, newW );
		this->setColSize( zm_nCurrentCol+1, w - newW - this->getExpanderSize() );	
	}

	// Giai phong chuot
	this->releaseCapture();
	// Cap nhat kich thuoc
	this->updateSplit();	

	this->rePaint();
}

void uiSplitContainer::_OnLButtonDown(uiMouseEvent e, int x, int y)
{
	zm_isMouseDown = true;

	zm_oldX = x;
	zm_oldY = y;

	this->setCapture();
}