// uiKeyEvent.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 15/12/2008
// Thong tin:
//		- Khai bao cac function cho uiKeyEvent

#include "uiEvent.h"
#include "uiInclude.h"

using namespace uiClass;

uiKeyEvent::uiKeyEvent(UINT wParam, UINT lParam)
{
	zm_wParam = wParam;
	zm_lParam = lParam;
}

uiKeyEvent::~uiKeyEvent(void)
{
	
}

// getObject
// Tra ve id doi tuong
DWORD uiKeyEvent::getObject()
{
	return UIOBJECT_EVENTKEY;
}

// getVKeyCode
// Tra lai ma VK
UINT uiKeyEvent::getVKeyCode()
{
	return (UINT)zm_wParam;
}

// getAnsiKey
// Tra lai ky tu ansi
char uiKeyEvent::getAnsiKey()
{
	// Lay thong tin ban phim hien tai
	BYTE aKeyState[256];
	GetKeyboardState( aKeyState );
	
	char cChar;
	
	// Lay tu tu Ansi
	ToAscii(
		this->getVKeyCode(),
		MapVirtualKey(this->getVKeyCode(),0) ,
		aKeyState,
		(WORD*)&cChar, 
		0);
	
	if (cChar < 0 )
		cChar=0;

	return cChar;
}

// getUnicodeKey
// Tra lai ky tu unicode
UINT uiKeyEvent::getUnicodeKey()
{
	// Lay thong tin ban phim hien tai
	BYTE aKeyState[256];
	GetKeyboardState( aKeyState );
	
	WCHAR wChar;
	
	// Lay ky tu Unicode
	ToUnicode( this->getVKeyCode(), 
		MapVirtualKey(this->getVKeyCode(),0),
		aKeyState,
		&wChar,
		sizeof(WCHAR),
		0);
		
	return wChar;
}

// getEventLoop		
UINT uiKeyEvent::getEventLoop()
{
	return (UINT)zm_lParam;
}