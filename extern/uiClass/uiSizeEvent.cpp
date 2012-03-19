// uiSizeEvent.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 15/12/2008
// Thong tin:
//		- Khai bao cac function cho uiSizeEvent

#include "uiEvent.h"
#include "uiInclude.h"

using namespace uiClass;

uiSizeEvent::uiSizeEvent(int nParam)
{
	zm_nParam = nParam;
}

uiSizeEvent::~uiSizeEvent()
{

}
		
// getObject
// Tra ve id doi tuong
DWORD uiSizeEvent::getObject()
{
	return UIOBJECT_EVENTSIZE;
}


// isMaximum
// Tra ve true neu cua so max
bool uiSizeEvent::isMaximum()
{
	return zm_nParam == SIZE_MAXIMIZED;
}

// isMinimum
// Tra ve true neu cua so min
bool uiSizeEvent::isMinimum()
{
	return zm_nParam == SIZE_MINIMIZED;
}

// isRestored
// Tra ve true neu cua so retor
bool uiSizeEvent::isRestored()
{
	return zm_nParam == SIZE_RESTORED;
}