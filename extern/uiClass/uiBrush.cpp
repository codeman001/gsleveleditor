// uiWindowEvent.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 16/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"

using namespace uiClass;

uiBrush::uiBrush()
{
	this->createSolidBrush( uiColor(255,255,255) );
}

uiBrush::uiBrush(uiColor color)
{
	this->createSolidBrush( color );
}

uiBrush::~uiBrush()
{
	this->deleteObject();
}
			
// getObjectType
// Tra ve ID doi tuong (const UIOBJECT_ )
DWORD uiBrush::getObject()
{
	return UIOBJECT_BRUSH;
}
			
// getLogBrush
// Tra ve handle Brush
LOGBRUSH uiBrush::getLogBrush()
{
	return zm_lBrush;
}
			
// createPatternBrush
// Khoi tao brush dua tren 1 bitmap
// void uiBrush::createPatternBrush( ZBitmap *pBitmap );

// createSolidBrush
// Khoi tao brush theo mau sac
void uiBrush::createSolidBrush( uiColor color )
{
	zm_lBrush.lbColor = color.getColor() ;
	zm_lBrush.lbStyle = BS_SOLID;
	zm_lBrush.lbHatch = 0;

	this->deleteObject();
	zm_hObj = CreateSolidBrush( color.getColor() );
}

// createHatchBrush
// Khoi tao mau to theo mau nHatch la const (HS_ cua GDI)
void uiBrush::createHatchBrush( uiColor color, int nHatch )
{
	zm_lBrush.lbColor = color.getColor() ;
	zm_lBrush.lbStyle = BS_HATCHED;
	zm_lBrush.lbHatch = nHatch;

	this->deleteObject();
	zm_hObj = (HGDIOBJ) CreateHatchBrush( nHatch , color.getColor() );
}			

// createNullBrush
// Khoi tao 1 brush khong mau
void uiBrush::createNullBrush()
{
	this->deleteObject();
	zm_hObj = GetStockObject( NULL_BRUSH );
}

// createUserMonoPatternBrush 
// Khoi tao brush theo 1 bitmap trang den
void uiBrush::createUserMonoPatternBrush ( int w, int h, BYTE* pData )
{
	this->deleteObject();

	HBITMAP hBM = CreateBitmap( w, h, 1, 1, pData );
	zm_hObj = CreatePatternBrush( hBM );
}

// createPatternBrush
// Khoi tao brush theo 1 hinh BITMAP
void uiBrush::createPatternBrush( uiBitmap *pBitmap )
{
	this->deleteObject();
	zm_hObj = CreatePatternBrush( (HBITMAP)pBitmap->getSafeObject() );
}