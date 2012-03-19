// uiIcon.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 20/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"

using namespace uiClass;

uiIcon::uiIcon(LPWSTR lpFilename, int sx, int sy)
{
	this->loadIcon( lpFilename, sx, sy );
}

uiIcon::uiIcon(LPWSTR lpResource)
{
	this->loadIcon( lpResource );
}

uiIcon::uiIcon()
{
	
}

uiIcon::~uiIcon(void)
{
	DestroyIcon( zm_hIcon );
}

// getHandle
// Tra ve handle HICON
HICON uiIcon::getHandle()
{
	return zm_hIcon;
}
		
// loadIcon
// Load icon tu 1 file
bool uiIcon::loadIcon(LPWSTR lpFilename, int sx, int sy)
{
	DestroyIcon( zm_hIcon );

	zm_hIcon = (HICON)LoadImageW( GetModuleHandle(0), lpFilename, IMAGE_ICON, sx, sy, LR_LOADFROMFILE );
	if (zm_hIcon == NULL)
		return false;
	
	return true;
}
		
// LoadIcon
// Load icon tu resource
bool uiIcon::loadIcon(LPWSTR lpResource )
{
	DestroyIcon( zm_hIcon );

	// Load from resource
	zm_hIcon = (HICON)LoadIconW( GetModuleHandle(0), lpResource );
	if (zm_hIcon == NULL)
		return false;
	
	return true;
}

// getObject
// Tra ve doi tuong UIOBJECT
DWORD uiIcon::getObject()
{
	return UIOBJECT_ICON;
}

// draw
// Ve icon
void uiIcon::draw( uiGraphics* pG, int x, int y)
{
	DrawIcon( pG->getHandle(), x,y, zm_hIcon );
}

void uiIcon::draw( uiGraphics* pG, int x, int y, int w, int h, bool bDisable)
{
	UINT nFlags = DST_ICON;
	
	if (bDisable == true)
		nFlags |= DSS_DISABLED;

	DrawStateW( pG->getHandle() , NULL,NULL, (LPARAM) zm_hIcon, 0, x,y,w,h, nFlags );
}