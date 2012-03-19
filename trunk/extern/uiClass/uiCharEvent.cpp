// uiCharEvent.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 15/12/2008
// Thong tin:
//		- Khai bao cac function cho uiCharEvent

#include "uiEvent.h"
#include "uiInclude.h"

using namespace uiClass;

uiCharEvent::uiCharEvent(UINT wParam, UINT lParam)
{
	zm_wParam = wParam;
	zm_lParam = lParam;
}

uiCharEvent::~uiCharEvent()
{
	
}

// getObject
// Tra ve id doi tuong
DWORD uiCharEvent::getObject()
{
	return UIOBJECT_EVENTCHAR;
}

// getRepeatCount
// Lay so lan lap
UINT uiCharEvent::getRepeatCount()
{
	return (UINT) (zm_lParam & 0x0000FFFF);
}

// getScanCode
// Lay ma code
UINT uiCharEvent::getScanCode()
{
	return (UINT) ((zm_lParam & 0x00FF0000) >> 16);
}

// isAltDown
// true neu ALT giu
bool uiCharEvent::isAltDown()
{
	return (bool) ((zm_lParam >> 29) & 1);
}

// isExKeyDown
// true neu phim mo rong giu
bool uiCharEvent::isExKeyDown()
{
	return (bool) ((zm_lParam >> 24) & 1);
}