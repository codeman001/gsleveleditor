// uiSizeEvent.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hongduc.pr@gmail.com
// Ngay viet: 10/3/2012
// Thong tin:
//		- Khai bao cac function cho uiScrollBarEvent

#include "uiEvent.h"
#include "uiInclude.h"

using namespace uiClass;

uiScrollBarEvent::uiScrollBarEvent( uiObject *pControl, int pos, int min, int max )
{
	zm_scroll = pControl;
	zm_pos = pos;
	zm_min = min;
	zm_max = max;
}

// getObject
// Tra ve UIOBJECT_SCROLLBAREVENT
DWORD uiScrollBarEvent::getObject()
{
	return UIOBJECT_SCROLLBAREVENT;
}

// getPos
// Lay gia tri hien tai
int uiScrollBarEvent::getPos()
{
	return zm_pos;
}

// getMin
// Lay gioi han nho nhat
int uiScrollBarEvent::getMin()
{
	return zm_min;
}

// getMax
// Lay gioi han cao nhat
int uiScrollBarEvent::getMax()
{
	return zm_max;
}

// getControl
// Lay con tro control bar (co the la windows)
uiObject* uiScrollBarEvent::getControl()
{
	return zm_scroll;
}