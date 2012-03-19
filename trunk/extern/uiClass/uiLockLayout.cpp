// uiLockLayout.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 26/2/2009
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"
#include <algorithm>

using namespace uiClass;

// getObject
DWORD uiLockLayout::getObject()
{
	return UIOBJECT_LOCKLAYOUT;
}

uiLockLayout::uiLockLayout(uiWindow* pWin)
{
	zm_pMyWin = pWin;
}

uiLockLayout::~uiLockLayout()
{

}

// lockLayout
// Them Window vao lockLayout
void uiLockLayout::lockLayout( uiWindow *pWin )
{
	// Tim kiem vi tri pWin
	vector< uiWindow*>::iterator i = std::find( zm_aLockLayout.begin(), zm_aLockLayout.end(), pWin);

	// Neu chua ton tai -> Them vao danh sach
	if ( i == zm_aLockLayout.end() )
		zm_aLockLayout.push_back (pWin);
}

// unlockLayout
// Loai bo Window khoi lockLayout
void uiLockLayout::unlockLayout( uiWindow *pWin )
{
	// Tim vi tri pWin
	vector< uiWindow*>::iterator i = std::find( zm_aLockLayout.begin(), zm_aLockLayout.end(), pWin);

	// Neu ton tai -> Xoa danh sach
	if ( i != zm_aLockLayout.end() )
		zm_aLockLayout.erase( i );
}

// update
// Cap nhat vi tri cac cua so
void uiLockLayout::update()
{
	vector<uiWindow*>::iterator i;
	uiWindow* p;
	int x,y,w,h;
	int x2,y2;

	for (i = zm_aLockLayout.begin(); i != zm_aLockLayout.end(); i++)
	{
		p = (*i);	

		// Lay layout
		uiMargin g;
		DWORD layout = p->getLockLayout(&g);
		
		// Lay toa do nguyen
		p->getLeftTop(&x,&y);			
		p->getWidthHeight(&w,&h);		

		// Dieu chinh kich thuoc
		if ( (layout & UILOCKLAYOUT_TOP) == UILOCKLAYOUT_TOP )
			y = g.nTop;

		if ( (layout & UILOCKLAYOUT_LEFT) == UILOCKLAYOUT_LEFT )
			x = g.nLeft;
		
		if ( (layout & UILOCKLAYOUT_RIGHT) == UILOCKLAYOUT_RIGHT )
		{
			x2 = zm_pMyWin->getClientWidth() - g.nRight;	
			if ( (layout & UILOCKLAYOUT_LEFT) != UILOCKLAYOUT_LEFT )
				x = x2 - p->getClientWidth();

			w = x2 - x;
		}

		if ( (layout & UILOCKLAYOUT_BOTTOM) == UILOCKLAYOUT_BOTTOM )
		{
			y2 = zm_pMyWin->getClientHeight() - g.nBottom;
			if ( (layout & UILOCKLAYOUT_TOP) != UILOCKLAYOUT_TOP )
				y = y2 - p->getClientHeight();
			
			h = y2 - y;
		}

		// Thiet lap
		p->setParent( zm_pMyWin );
		p->moveWindow(x,y,w,h);

	}
}