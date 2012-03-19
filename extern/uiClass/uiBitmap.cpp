// uiBitmap.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 23/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"

using namespace uiClass;

uiBitmap::uiBitmap(uiGraphics *pG)
{
	
}

uiBitmap::uiBitmap(HBITMAP hBitmap)
{
	zm_hObj = hBitmap;
}

uiBitmap::uiBitmap( LPWSTR lpFrom, bool bResource )
{
	if ( bResource )
		this->loadFromResource( lpFrom );
	else
		this->loadFromFile( lpFrom ); 
}

uiBitmap::uiBitmap(DWORD w, DWORD h, BYTE nPlanes, BYTE nBitCount, void* lpBits)
{
	zm_hObj = CreateBitmap(w,h,nPlanes,nBitCount, lpBits);
}

uiBitmap::~uiBitmap()
{
	this->deleteObject();	
}

// getObjectType
// Tra ve doi tuong UIOBJECT
DWORD uiBitmap::getObject()
{
	return UIOBJECT_BITMAP;
}

// loadFromFile
// Load bitmap tu file .bmp
bool uiBitmap::loadFromFile( LPWSTR lpFile )
{
	this->deleteObject();

	zm_hObj = (HGDIOBJ) LoadImageW( NULL, lpFile,IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	if ( zm_hObj == NULL )
		return false;

	return true;
}

// loadFromResource
// Load bitmap from ID resource
bool uiBitmap::loadFromResource( LPWSTR res )
{
	this->deleteObject();

	zm_hObj = (HGDIOBJ) LoadBitmapW( GetModuleHandle(0) , res );
	if ( zm_hObj == NULL )
		return false;

	return true;
}

// getWidth
// Lay chieu rong cua Bitmap
int uiBitmap::getWidth()
{
	BITMAP bm;
	GetObject( zm_hObj, sizeof(BITMAP), &bm);

	return bm.bmWidth;
}
		
// getHeiht
// Lay chieu cao cua Bitmap
int uiBitmap::getHeight()
{
	BITMAP bm;
	GetObject( zm_hObj, sizeof(BITMAP), &bm);

	return bm.bmHeight;
}

// getBitPerPixel
// Lay so bit/Pixel
BYTE uiBitmap::getBitPerPixel()
{
	BITMAP bm;
	GetObject( zm_hObj, sizeof(BITMAP), &bm);

	return bm.bmBitsPixel * bm.bmPlanes;
}

// getRowBit
// Lay du lieu mau sac o line thu n
void uiBitmap::getRowBit(BYTE* pData, UINT n)
{
	// Chua code
}