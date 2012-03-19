// uiMouseEvent.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 15/12/2008
// Thong tin:
//		- Khai bao cac function cho uiMouseEvent

#include "uiEvent.h"
#include "uiInclude.h"

using namespace uiClass;

uiMouseEvent::uiMouseEvent(UINT uParam)
{
	zm_uParam = uParam;
}

uiMouseEvent::~uiMouseEvent(void)
{

}

// getObject
// Tra ve id doi tuong
DWORD uiMouseEvent::getObject()
{
	return UIOBJECT_EVENTMOUSE;
}


// isCtrlKeyDown
// Tra ve true neu phim control giu
bool uiMouseEvent::isCtrlKeyDown(){
	return (bool)( ( zm_uParam & MK_CONTROL) == MK_CONTROL );
}

// isShiftKeyDown
// Tra ve true neu phim shift giu
bool uiMouseEvent::isShiftKeyDown(){
	return (bool)( ( zm_uParam & MK_SHIFT ) == MK_SHIFT );
}

// isAltKeyDown
// Tra ve true neu phim alt giu
bool uiMouseEvent::isAltKeyDown()
{
	if ( GetAsyncKeyState( VK_MENU ) < 0 )
		return true;
	return false;
	//return (bool)( ( zm_uParam & MK_ALT ) == MK_ALT );
}

// isLeftButtonDown
// Tra ve true neu chuot trai giu
bool uiMouseEvent::isLeftButtonDown(){
	return (bool)( ( zm_uParam & MK_LBUTTON ) == MK_LBUTTON);
}

// isMiddleButtonDown
// Tra ve true neu chuot phai giu
bool uiMouseEvent::isMiddleButtonDown(){
	return (bool)( ( zm_uParam & MK_MBUTTON ) == MK_MBUTTON);
}

// isRightButtonDown
// Tra ve true neu chuot giua giu
bool uiMouseEvent::isRightButtonDown(){
	return (bool)( ( zm_uParam & MK_RBUTTON ) == MK_RBUTTON);
}

// isWheelUp
// Tra ve true neu chuot giua day len (false day xuong)
bool uiMouseEvent::isWheelUp()
{
	int nDelta = (int) zm_uParam;
	if (nDelta > 0)
		return true;
	else
		return false;
}