// uiGraphics.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 16/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"
#include "uiString.h"

using namespace uiClass;

uiGraphics::uiGraphics()
{
	zm_hDC = GetDC( NULL );
	zm_hWnd = NULL;
	zm_bIsBackBuffer = false;
}

// uiGraphics
// Khoi tao doi tuong graphics
// Tham so:		- Cua so can ve
//				- pWin = NULL ve len man hinh
uiGraphics::uiGraphics(uiWindow* pWin)
{
	if (pWin)
		zm_hWnd = pWin->getHandle();
	else
		zm_hWnd = NULL;

	zm_hDC = GetDC( zm_hWnd );	
	zm_bIsBackBuffer = false;
	zm_releaseHDC = true;
}

uiGraphics::uiGraphics(HDC hDC)
{
	zm_hDC = hDC;
	zm_hWnd = NULL;
	zm_releaseHDC = true;
}

uiGraphics::~uiGraphics(void)
{
	if ( zm_bIsBackBuffer )	
	{
		if ( zm_releaseHDC )
			DeleteDC( zm_hDC );			

		DeleteObject( zm_hBitmap );
	}
	else
	{
		if ( zm_releaseHDC )
			ReleaseDC( zm_hWnd, zm_hDC );
	}
	
}	

// getObject
// Tra ve ID doi tuong
DWORD uiGraphics::getObject()
{
	return UIOBJECT_GRAPHICS;
}

// getHandle
// Lay gia tri HDC
HDC uiGraphics::getHandle()
{
	return zm_hDC;
}

// getHwnd
// Lay handle HWND
HWND uiGraphics::getHwnd()
{
	return zm_hWnd;
}

// getBitmap
// Lay handle Bitmap
HBITMAP uiGraphics::getBitmap()
{
	
	if (zm_bIsBackBuffer)
		return zm_hBitmap;
	else
	{
		// Lay bitmap hien tai
		HBITMAP b = (HBITMAP) GetCurrentObject( zm_hDC, OBJ_BITMAP ); 
		return zm_hBitmap;
	}
}

// swapImage
// Ve Images len cua so
void uiGraphics::swapImage(int xDest, int yDest, int wDest, int hDest,
				uiBitmap *pImg, int xSrc,int ySrc, int wSrc, int hSrc, bool bRepeatX, bool bRepeatY, DWORD option)
{	
	// Tao 1 graphics tu bitmap
	uiGraphics *bitmap = this->createBackBuffer(pImg);
	
	// Tao 1 memGraphics;
	uiGraphics *mem = this->createBackBuffer( wSrc, hSrc );

	// Copy hinh nen
	mem->swapBuffer( xDest, yDest, wDest, hDest, this, xDest, yDest, wDest, hDest, SRCCOPY );

	// Dua hinh can draw len memGraphics
	mem->swapBuffer(0,0, wSrc,hSrc, bitmap, xSrc,ySrc,wSrc,hSrc, option);
		
	int nW = wDest;
	int nH = hDest;

	if (bRepeatX == false)
		nW = wSrc;
	if (bRepeatY == false)
		nH = hSrc;

	float fX = (float)nW/(float)wSrc;
	float fY = (float)nH/(float)hSrc;
	int loopX = ( fX>(int)fX? (int)fX + 1: (int)fX );
	int loopY = ( fY>(int)fY? (int)fY + 1: (int)fY );
	int w,h;

	// Paint loop Bitmap
	for (int i = 0; i < loopX; i++)
	{
		for (int j = 0; j < loopY; j++)
		{
			w = wSrc;
			h = hSrc;
			
			// Neu la o cuoi cung bi thua thi cat bot Width hay Height
			if ( i == loopX - 1)
				w =	(int) ( wDest - i*wSrc );
			if ( j == loopY - 1)
				h = (int) ( hDest - j*hSrc );
			
			// Paint
			this->swapBuffer( xDest + i*wSrc, yDest + j*hSrc, w, h, 
				mem,0,0,w, h, option );
		}
	}

	// Giai phong bo nho
	bitmap->releaseGraphics();
	mem->releaseGraphics();	
}

// selectObject
// Thiet lap cac doi tuong graphics
HGDIOBJ uiGraphics::selectObject(uiGraphicsObject *pObj)
{
	return SelectObject( this->getHandle(), pObj->getSafeObject() );
}
		
// selectObject
// Thiet lap doi tuong graphics
HGDIOBJ uiGraphics::selectObject(HGDIOBJ obj)
{
	return SelectObject( this->getHandle(), obj );
}

// createBackBuffer
// Khoi tao vung dem
uiGraphics* uiGraphics::createBackBuffer(int w, int h)
{	
	HBITMAP hBitmap = CreateCompatibleBitmap( this->getHandle(), w,h );
	HDC		hDC		= CreateCompatibleDC	( this->getHandle() );	
	
	SelectObject( hDC, hBitmap );
	
	uiGraphics *pBuffer = new uiGraphics(hDC);

	pBuffer->zm_bIsBackBuffer	= true;
	pBuffer->zm_hWnd			= this->zm_hWnd;	
	pBuffer->zm_hBitmap			= hBitmap;

	return pBuffer;
}

// createBackBuffer
// Khoi tao vung dem theo 1 bitmap
uiGraphics* uiGraphics::createBackBuffer(uiBitmap *pBitmap)
{		
	HBITMAP hBitmap = (HBITMAP)pBitmap->getSafeObject();
	int w = pBitmap->getWidth();
	int h = pBitmap->getHeight();
		
	HDC		hMemDC		= CreateCompatibleDC	( this->getHandle() );		
	SelectObject( hMemDC, hBitmap );	
	return	new uiGraphics(hMemDC);

	/*
	HBITMAP hBitmap = (HBITMAP)pBitmap->getSafeObject();
	int w = pBitmap->getWidth();
	int h = pBitmap->getHeight();

	// Lay thong tin DIBSection
	BITMAP bm;
	GetObject(hBitmap, sizeof(BITMAP), &bm);	

	// Khoi tao image va dc ao
	HBITMAP hMemBitmap = CreateCompatibleBitmap( this->getHandle(), w,h );
	HDC		hMemDC		= CreateCompatibleDC	( this->getHandle() );		
	SelectObject( hMemDC, hMemBitmap );	
	uiGraphics *pBuffer = new uiGraphics(hMemDC);
	
	// Thiet lap cho buffer ao
	pBuffer->zm_bIsBackBuffer	= true;
	pBuffer->zm_hWnd			= this->zm_hWnd;	
	pBuffer->zm_hBitmap			= hMemBitmap;

	// Copy Bitmap vao memDC //////////////////////////////////////////////
	HDC tHDC = CreateCompatibleDC(zm_hDC);
	SelectObject( tHDC, hBitmap );	
	StretchBlt( pBuffer->zm_hDC, 0,0,w,h, tHDC, 0,0,w,h, SRCCOPY );		

	DeleteDC( tHDC );	

	///////////////////////////////////////////////////////////////////////

	// Neu bitmap la 256 color
	if( ( bm.bmBitsPixel * bm.bmPlanes ) <= 8 )
	{
		RGBQUAD colorTable[256];
		
		// Lay bang mau hien tai cua bitmap (nam trong memDC)
		GetDIBColorTable( pBuffer->zm_hDC, 0,256, colorTable);
		
		// Khoi tao logPal
		LOGPALETTE *pLogPal = (LOGPALETTE *)malloc( sizeof(LOGPALETTE) + (256*sizeof(PALETTEENTRY)) );
		
		// Thiet lap LogPAL
		pLogPal->palVersion = 0x300;
		pLogPal->palNumEntries = 256;

		for(int i=0;i<256;i++)
		{
			pLogPal->palPalEntry[i].peRed	= colorTable[i].rgbRed;
			pLogPal->palPalEntry[i].peGreen = colorTable[i].rgbGreen;
			pLogPal->palPalEntry[i].peBlue	= colorTable[i].rgbBlue;
			pLogPal->palPalEntry[i].peFlags = 0;
		}

		// Tao PAL
		HPALETTE h = CreatePalette( pLogPal );
		SelectObject( pBuffer->zm_hDC, h );

		free(pLogPal);
	}


	return pBuffer;
	*/
}

// swapBuffer
// Hoan doi vung dem
void uiGraphics::swapBuffer(int xDest, int yDest, int wDest, int hDest, 
				uiGraphics *pBuffer, int xSrc, int ySrc, int wSrc, int hSrc, 
				DWORD dwOption)
{
	StretchBlt(
		this->getHandle(),		xDest, yDest, wDest, hDest, 
		pBuffer->getHandle(),	xSrc, ySrc, wSrc, hSrc, dwOption );
}
			
// releaseGraphics
// Huy bo vung dem
void uiGraphics::releaseGraphics()
{
	if (zm_bIsBackBuffer == true)
		delete this;
}

// drawPixel
// Ve 1 pixel mau tai diem x,y
void uiGraphics::drawPixel(int x, int y, uiColor color)
{
	SetPixel(this->getHandle(), x, y, color.getColor() );
}

// drawLine
// Ve 1 line
void uiGraphics::drawLine(int x1, int y1, int x2, int y2)
{
	MoveToEx( this->getHandle(), x1,y1 , NULL);
	LineTo( this->getHandle(), x2, y2 );
}
			
// moveTo
// Ve point dau tien cua line
void uiGraphics::moveTo(int x, int y)
{
	MoveToEx( this->getHandle(), x,y , NULL);
}

// lineTo
// Ve point tiep theo cua line
void uiGraphics::lineTo(int x, int y)
{
	LineTo( this->getHandle(), x, y );
}
			
// drawPolyline
// Ve nhieu duong thang
void uiGraphics::drawPolyline(uiPoint *a, int nPoint)
{
	int i;

	for (i = 0; i< nPoint;i+=2){
		if (i == 0)
			this->moveTo( (int) a[i].x, (int) a[i].y );
		else
			this->lineTo( (int) a[i].x, (int) a[i].y );

		this->lineTo( (int) a[i+1].x, (int) a[i+1].y );
	}
}

// drawPolylineLoop
// Ve polygon line (khong fill)
void uiGraphics::drawPolylineLoop(uiPoint *a, int nPoint)
{
	int i;

	for (i = 0; i< nPoint;i+=2){
		if (i == 0)
			this->moveTo( (int) a[i].x, (int) a[i].y );
		else
			this->lineTo( (int) a[i].x, (int) a[i].y );

		this->lineTo( (int) a[i+1].x, (int) a[i+1].y );
	}

	this->lineTo( (int) a[0].x, (int) a[0].y );
}
			
// drawBezier
// Ve duong con Bezier
void uiGraphics::drawBezier(uiPoint *a, int nPoint)
{
	int i;
	POINT *p = new POINT[nPoint];

	for (i = 0;i<nPoint;i++){
		p[i].x = (int) a[i].x;
		p[i].y = (int) a[i].y;
	}

	PolyBezier( this->getHandle(),(POINT*) a, nPoint );

	delete p;
}			
			
// drawArc
// Ve duong cung
void uiGraphics::drawArc(int nLeft, int nTop, int nRight, int nBottom, 
						 int nXStart, int nYStart, int nXEnd, int nYEnd )
{
	Arc( this->getHandle(), 
		nLeft, nTop, nRight, nBottom,
		nXStart, nYStart,
		nXEnd, nYEnd);
}

// drawArcAngle
// Ve 1 duong cung theo goc
void uiGraphics::drawArcAngle(int x, int y, DWORD dwRadius,float fStartAngle, float fSweepAngle)
{
	AngleArc( this->getHandle(), x, y, dwRadius, fStartAngle, fSweepAngle);
}

// drawPolygon
// Ve 1 polygon fill
void uiGraphics::drawPolygon( uiPoint *a, int nPoint)
{			
	Polygon( this->getHandle(), (POINT*) a, nPoint);
}
			
// drawEllipse
// Ve 1 hinh ellipse
void uiGraphics::drawEllipse(int x1, int y1, int x2, int y2)
{
	Ellipse( this->getHandle(), x1,y1, x2,y2 );
}			

// drawChord
// Ve 1 cung ellip
void uiGraphics::drawChord(int x1, int y1, int x2, int y2, int xR1, int yR1, int xR2, int yR2 )
{
	Chord( this->getHandle(), x1,y1, x2, y2, xR1, yR1, xR2, yR2 );
}

// drawChord
// Ve 1 cung pie
void uiGraphics::drawPie(int x1, int y1, int x2, int y2, int xR1, int yR1, int xR2, int yR2 )
{
	Pie( this->getHandle(), x1,y1, x2, y2, xR1, yR1, xR2, yR2 );
}
			
// drawPatternInvert
// Ve 1 rectangle co mau sac invert voi mau hien tai
void uiGraphics::drawPatternInvert(int x, int y, int w, int h)
{
	PatBlt( this->getHandle(), x,y, w,h , PATINVERT);
}
			
// drawRectangle
// Ve 1 hinh chu nhat (ko fill)
void uiGraphics::drawRectangle(int x1, int y1, int x2, int y2)
{
	Rectangle( this->getHandle(), x1,y1,x2,y2 );
}	

// drawFillRectangle
// Ve 1 hinh chu nhat
void uiGraphics::drawFillRectangle(int x1, int y1, int x2, int y2, uiBrush* pBrush)
{
	RECT r;
	r.left = x1;
	r.top = y1;
	r.right = x2;
	r.bottom = y2;

	FillRect( this->getHandle(),&r, (HBRUSH) pBrush->getSafeObject() );	
}
			
// drawRoundRectangle
// Ve 1 hinh chu nhat (bo goc)
void uiGraphics::drawRoundRectangle(int x1, int y1, int x2, int y2, int R1, int R2)
{
	RoundRect( this->getHandle(), x1,y1,x2,y2, R1, R2 );
}
			
// drawText
// Ve 1 doan text ra man hinh
void uiGraphics::drawText(int x, int y, LPWSTR lpText)
{
	TextOutW( this->getHandle(), x, y, lpText, uiString::length<WCHAR>(lpText)  );
}
			
// drawTextInRect
// Ve 1 doan text trong 1 khung hinh chu nhat
// Tham so:		- nFormat (const DT_  cua GDI)
void uiGraphics::drawTextInRect(int x, int y, int w, int h, LPWSTR lpText, UINT nFormat)
{
	RECT r;	
	SetRect(&r, x, y, x+w, y + h);
	DrawTextW( this->getHandle(), lpText, uiString::length<WCHAR>(lpText), &r, nFormat );
}

// setTextColor
// Thiet la mau sac cho text
void uiGraphics::setTextColor(uiColor c)
{
	SetTextColor( this->getHandle(), c.getColor() );
}
			
// setTextBkColor
// Thiet lap mau nen
void uiGraphics::setTextBkColor(uiColor c)
{	
	SetBkColor( this->getHandle(), c.getColor() );
}

// setTextBkTransparent
// Thiet lap mau nen trong suot
void uiGraphics::setTextBkTransparent(bool bSet)
{
	if (bSet)
		SetBkMode( this->getHandle(), TRANSPARENT );
	else
		SetBkMode( this->getHandle(), OPAQUE );
}

// setTextAlign 
// Canh text (su dung cho drawTextInRect) nTAlign la const TA_ cua GDI
void uiGraphics::setTextAlign( UINT nTAlign )
{
	SetTextAlign( this->getHandle(), nTAlign );
}			
			
// getCharWidth 
// Lay chieu rong cua 1 ky tu
DWORD uiGraphics::getCharWidth(WCHAR wChar)
{
	int nWidth;
	GetCharWidth32W(this->getHandle(), (UINT) wChar, (UINT) wChar, &nWidth );
	return nWidth;
}

// getStringWidth
// Lay chieu dai cua 1 chuoi ky tu
DWORD uiGraphics::getStringWidth(WCHAR *lpString)
{
	DWORD nWidth = 0;
	int nCharWidth = 0;

	DWORD nLen = uiString::length<WCHAR>( lpString );
	
	for ( int i = 0; i < (int)nLen; i++ )
	{
		GetCharWidth32W(this->getHandle(), (UINT) lpString[i], (UINT) lpString[i], &nCharWidth );
		nWidth += nCharWidth;
	}
	
	return nWidth;
}

// getTextColor
// Lay mau sac cua text
uiColor uiGraphics::getTextColor()
{
	COLORREF m;
	m = GetTextColor( this->getHandle() );
	return uiColor( (DWORD) m );
}

// getTextBkColor
// Lay mau nen cua text
uiColor uiGraphics::getTextBkColor()
{
	COLORREF m;
	m = GetBkColor( this->getHandle() );
	return uiColor( (DWORD) m );
}

// releaseHDCWhenDestructor
// Cho phep ko huy HDC
void uiGraphics::releaseHDCWhenDestructor( bool b )
{
	zm_releaseHDC = b;
}