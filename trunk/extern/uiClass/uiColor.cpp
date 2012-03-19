// uiWindowEvent.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 16/12/2008
// Thong tin: Mo ta function cho uiColor

#include "uiWindow.h"
#include "uiInclude.h"

using namespace uiClass;

uiColor::uiColor()
{
	zm_dwColor = 0;
}

uiColor::uiColor(DWORD color)
{
	zm_dwColor = color;
}

uiColor::uiColor(UINT R, UINT G, UINT B, UINT A)
{
	this->setRGBA(R,G,B,A);
}

// getObject
// Tra ve UIOBJECT_COLOR
DWORD uiColor::getObject()
{
	return UIOBJECT_COLOR;
}

// getColor
// Lay mau sac hien tai
DWORD uiColor::getColor()
{
	return zm_dwColor;
}

// setRGBA
// Thiet lap mau sac
void uiColor::setRGBA(BYTE nR, BYTE nG, BYTE nB, BYTE nA)
{
	DWORD myColor=0;

	// A
	myColor = (UCHAR) nA;
	myColor = myColor << 8;	
	// B
	myColor = myColor | (UCHAR) nB;
	myColor = myColor << 8;	
	// G
	myColor = myColor | (UCHAR) nG;
	myColor = myColor << 8;	
	// R
	myColor = myColor | (UCHAR) nR;

	zm_dwColor = myColor;
}

// Lay mau sac hien tai
void uiColor::getRGBA(BYTE *R, BYTE* G, BYTE* B, BYTE* A)
{
	*R = (BYTE)(  0x000000FF & zm_dwColor );
	*G = (BYTE)( (0x0000FF00 & zm_dwColor) >> 8 );
	*B = (BYTE)( (0x00FF0000 & zm_dwColor) >> 16 );
	*A = (BYTE)( (0xFF000000 & zm_dwColor) >> 24 );
}

// fromRGBA
// Thiet lap mau sac RGBA
uiColor uiColor::fromRGBA (UINT nR, UINT nG, UINT nB, UINT nA)
{
	uiColor c(nR,nG,nB,nA);
	return c;
}

// fromSystem
// Thiet lap mau sac he thong (index la const cua WINAPI COLOR_ )
uiColor uiColor::fromSystem	(UINT nIndex)
{	
	uiColor c;
	c.zm_dwColor = GetSysColor( nIndex );
	return c;
}
// fromDWORD
// Thiet lap mau sac tu 1 gia tri dword
uiColor uiColor::fromDWORD	(DWORD color )
{
	uiColor c(color);
	return c;
}
			
// getR
// Lay sac do
BYTE uiColor::getR()
{
	return (BYTE)(0x000000FF & zm_dwColor);
}

// getG
// Lay sac xanh la
BYTE uiColor::getG()
{
	return (BYTE)( (0x0000FF00 & zm_dwColor) >> 8 );
}
			
// getB
// Lay sac xanh
BYTE uiColor::getB()
{
	return (BYTE)( (0x00FF0000 & zm_dwColor) >> 16 );
}

// getA
// Lay do trong suot
BYTE uiColor::getA()
{
	return (BYTE)( (0xFF000000 & zm_dwColor) >> 24 );
}
					

uiColor& uiColor::operator = (const uiColor& c)
{
	zm_dwColor = c.zm_dwColor;
	return *this;
}