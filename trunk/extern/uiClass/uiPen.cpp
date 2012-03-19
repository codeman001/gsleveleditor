// uiWindowEvent.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 16/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"

using namespace uiClass;

uiPen::uiPen()
{
	this->create(1, PS_SOLID, uiColor(0,0,0) );
}

uiPen::~uiPen()
{
	this->deleteObject();
}

// uiPen
// Constructor khoi tao but ve
// Tham so:		- Chieu rong
//				- Kieu ve (const GDI PS_)
//				- Mau sac
uiPen::uiPen(int nWidth, int nStyle, uiColor color)
{
	this->create( nWidth, nStyle, color );
}
			
// getObjectType
// Tra ve ID doi tuong 
DWORD uiPen::getObject()
{
	return UIOBJECT_PEN;
}

// create
// Khoi tao but ve
// Tham so:		- Chieu rong
//				- Kieu ve (const GDI PS_)
//				- Mau sac
void uiPen::create(int nWidth, int nStyle, uiColor color)
{
	zm_lPen.lopnColor	= color.getColor();
	zm_lPen.lopnStyle	= nStyle;
	zm_lPen.lopnWidth.x = nWidth; 
	
	this->deleteObject();

	zm_hObj = (HGDIOBJ) CreatePen( nStyle, nWidth, color.getColor() );
}

// create
// Khoi tao but ve tu brush
// Tham so:		- Chieu rong
//				- Kieu ve (const GDI PS_)
//				- Mau sac
void uiPen::createBrushPen(int nWidth, int nStyle, uiBrush* pBrush)
{
	this->deleteObject();

	LOGBRUSH lb = pBrush->getLogBrush();	
	zm_lPen.lopnColor =		lb.lbColor;
	zm_lPen.lopnStyle =		nStyle;
	zm_lPen.lopnWidth.x = nWidth; 

	zm_hObj = (HGDIOBJ) ExtCreatePen( PS_GEOMETRIC |nStyle, nWidth, &lb , 0 , 0 );
}
			
// createUserPen
// Khoi tao but ve tu do
// Tham so:		- Chieu rong
//				- Brush the hien mau sac
//				- So phan tu cua mang
//				- Kieu mang
// Thong tin: xem them ham ExtCreatePen cua WINAPI
void uiPen::createUserPen(int nWidth, uiBrush* pBrush,int dwStyleCount, DWORD *aStyle)
{
	this->deleteObject();
	LOGBRUSH lb = pBrush->getLogBrush();	
		
	zm_hObj = (HGDIOBJ) ExtCreatePen( PS_COSMETIC | PS_USERSTYLE,
		nWidth, &lb ,dwStyleCount , aStyle );
}