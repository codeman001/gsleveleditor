// uiForm.cpp.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 17/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"

using namespace uiClass;

// getObject
DWORD uiDock::getObject()
{
	return UIOBJECT_DOCK;
}

// getClientRect
// Lay vung cua so trong con lai de dock
void uiDock::getClientRect(int *x, int *y, int *r, int *b)
{
	*x = 0;
	*y = 0;
	*r = zm_pMyWin->getClientWidth();
	*b = zm_pMyWin->getClientHeight();

	// Duyet tung cua so da duoc dock de tinh w,h
	vector<uiWindow*>::iterator i;
	uiWindow* p;
	for (i = zm_aDocked.begin(); i != zm_aDocked.end(); i++)
	{
		p = (*i);		
		
		int ml, mr, mt, mb;
		p->getMargin(&ml, &mr, &mt, &mb);

		// Cua so nay hidden
		if ( p->isShow() == false 
			&& p->getObject() != UIOBJECT_REBAR
			&& p->getObject() != UIOBJECT_STATUSBAR)
			continue;

		// Xac dinh trang thai dock cua cac cua so
		switch ( p->getDock() )
		{
		case UIDOCK_TOP:
			*y += (mt + p->getHeight());
			break;
		case UIDOCK_LEFT:
			*x += (ml + p->getWidth());
			break;
		case UIDOCK_RIGHT:			
			*r -= (mr + p->getWidth());
			break;
		case UIDOCK_BOTTOM:
			*b -= (mb + p->getHeight());
			break;
		case UIDOCK_FILL:
		case UIDOCK_CENTER:
			*x = 0;
			*y = 0;
			*r = 0;
			*b = 0;
			break;
		}
	}
}		

uiDock::uiDock(uiWindow* pWin)
{
	zm_pMyWin = pWin;
}

uiDock::~uiDock()
{
	
}
			
// dock
// dock cua so pWin vao parent
void uiDock::dock(uiWindow* pWin)
{
	this->unDock( pWin );
	zm_aDock.push_back(pWin);
}		
			
// unDock
// unDock cua so pWin
void uiDock::unDock(uiWindow* pWin)
{
	vector<uiWindow*>::iterator i;
	
	for (i = zm_aDock.begin(); i != zm_aDock.end(); i++)
	{
		if ( (*i) == pWin )
		{
			zm_aDock.erase( i );
			return;
		}
	}
}

// update
// cap nhat vi tri cac cua so
void uiDock::update()
{	
	zm_aDocked.clear();
	
	vector<uiWindow*>::iterator i;
	uiWindow* p;

	int x,y,r,b,w,h;	
	int marginT,marginB,marginL,marginR;

	for (i = zm_aDock.begin(); i != zm_aDock.end(); i++)
	{
		p = (*i);

		this->getClientRect(&x,&y,&r,&b);
		w = r - x;
		h = b - y;
		p->setParent( zm_pMyWin );
		
		// Lay margin
		p->getMargin(&marginL, &marginR, &marginT, &marginB);
		
		x += marginL;
		y += marginT;
		w -= (marginL + marginR );
		h -= (marginT + marginB );

		switch ( p->getDock() )
		{
		case UIDOCK_TOP:
			p->moveWindow(x,y,w, p->getHeight());
			break;
		case UIDOCK_LEFT:
			p->moveWindow(x,y, p->getWidth(), h);			
			break;
		case UIDOCK_RIGHT:
			p->moveWindow(r - p->getWidth(), y, p->getWidth(), h);			
			break;
		case UIDOCK_BOTTOM:
			p->moveWindow(x, b - p->getHeight(),w, p->getHeight());			
			break;
		case UIDOCK_FILL:
			p->moveWindow(x,y,w,h);
			break;
		case UIDOCK_CENTER:
			x += (w/2 - p->getWidth()/2 );
			y += (h/2 - p->getHeight()/2 );

			p->moveWindow(x,y,p->getWidth(),p->getHeight());
			break;
		}

		zm_aDocked.push_back(p);
	}
}