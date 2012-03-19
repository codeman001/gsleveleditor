// uiWindowEvent.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 16/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"

using namespace uiClass;

uiGraphicsObject::uiGraphicsObject()
{
	zm_hObj = 0;
}

// getSafeObject
// Lay handle HGDIOBJ
HGDIOBJ uiGraphicsObject::getSafeObject()
{
	return zm_hObj;
}

// deleteObject
// Xoa bo object
void uiGraphicsObject::deleteObject()
{
	if ( zm_hObj != NULL )
		DeleteObject( zm_hObj );
}