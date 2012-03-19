#include "uiInclude.h"
#include "uiWindow.h"

#include <vector>
#include <algorithm>

using namespace uiClass;

// getObject
// Tra ve const UIOBJECT
DWORD uiTabController::getObject()
{
	return UIOBJECT_TABCONTROLLER;
}

// pushWindow
// Them mot cua so vao list
void uiTabController::pushWindow( uiWindow* pWin )
{
	zm_listWindow.push_back( pWin );
	zm_current = --zm_listWindow.end();

	pWin->setTabController(this);
}

// removeWindow
// Loai bo cua so ra khoi list
void uiTabController::removeWindow( uiWindow* pWin )
{
	zm_listWindow.remove( pWin );

	if ( zm_listWindow.size() == 0 )
		zm_current = zm_listWindow.end();
}

// CompareWindow
// So sanh 2 win theo thu tu tabIndex
bool uiTabController::CompareWindow(	uiWindow*& win1, uiWindow*& win2 )
{
	if ( win1->getTabIndex() < win2->getTabIndex() )
		return true;
	return false;
}

// setfocus
// Thiet lap cua so dang active
void uiTabController::setWinFocus( uiWindow* pWin )
{
	list<uiWindow*>::iterator i = std::find( zm_listWindow.begin(), zm_listWindow.end(), pWin );
	if ( i != zm_listWindow.end() )
		zm_current = i;
}

// updateIndex
// Sap xep lai danh sach theo thu tu index
void uiTabController::updateIndex()
{
	zm_listWindow.sort( uiTabController::CompareWindow );
}

// next
// Chuyen toi cua so phia truoc
void uiTabController::next()
{	
	list<uiWindow*>::iterator i = zm_current;
	zm_current++;

	// Kiem tra cac cua so ke tiep xem cua so nao co the active
	while ( zm_current != zm_listWindow.end() )
	{
		uiWindow* pWin = (*zm_current);

		// Neu cua so nay dang hien thi va ko bi disable
		if ( pWin->isShow() && pWin->isEnable() )
		{
			pWin->setFocus();
			return;
		}
		zm_current++;
	}
	
	// Neu khong tim thay cua so thi chay lai tu dau
	zm_current = zm_listWindow.begin();
	while ( zm_current != i )
	{
		uiWindow* pWin = (*zm_current);

		// Neu cua so nay dang hien thi va ko bi disable
		if ( pWin->isShow() && pWin->isEnable() )
		{
			pWin->setFocus();
			return;
		}
		zm_current++;
	}
	
}