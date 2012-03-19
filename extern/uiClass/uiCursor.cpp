// uiIcon.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 20/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"

using namespace uiClass;

// getHandle
// Tra ve handle HCURSOR
HCURSOR uiCursor::getHandle()
{
	return zm_hCursor;
}

// getObject
// Tra ve UIOBJECT
DWORD uiCursor::getObject()
{
	return UIOBJECT_CURSOR;
}

uiCursor::uiCursor(LPWSTR from, bool bResource)
{	
	if ( bResource == true )
		zm_hCursor = LoadCursorW( NULL, from );
	else
		zm_hCursor = LoadCursorFromFileW( from );
}

// loadFromFile
// Load icon .cur
bool uiCursor::loadFromFile( LPWSTR lpFile )
{
	DestroyCursor( zm_hCursor );
	zm_hCursor = LoadCursorFromFileW( lpFile );
	if ( zm_hCursor == NULL)
		return false;

	return true;
}

// loadFromResource
// Load icon tu resource
bool uiCursor::loadFromResource( LPWSTR lpRes )
{
	DestroyCursor( zm_hCursor );
	zm_hCursor = LoadCursorW( NULL, lpRes );

	if ( zm_hCursor == NULL)
		return false;

	return true;
}

uiCursor::~uiCursor()
{
	DestroyCursor( zm_hCursor );
}