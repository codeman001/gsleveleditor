#include "uiListPropertyComboBox.h"
#include "uiInclude.h"

using namespace uiClass;

uiListPropertyComboBox::uiListPropertyComboBox( uiWindow *pParent )
	:uiComboBox(L"", 0,0, 10,100, false, pParent)
{
	// Lay cua so edit cua comboBox
	HWND hEdit = FindWindowExW( this->getHandle(), NULL,L"Edit",NULL);

#pragma warning (disable:4311)
#pragma warning (disable:4312)
	// Luu proc mac dinh cua edit nay lai
	SetPropW(hEdit,L"Wprc",(HANDLE)GetWindowLong(hEdit,GWL_WNDPROC));

	// Thuc thi proc moi
	SetWindowLong(hEdit,GWL_WNDPROC,(DWORD)uiListPropertyComboBox::editComboBoxProc );
#pragma warning (default:4311)
#pragma warning (default:4312)

	// Luu handle Edit
	zm_hEdit = hEdit;
}

// windowProc		
// Muc dich de subclass lai control edit ben trong ComboBox
LRESULT CALLBACK uiListPropertyComboBox::editComboBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
#pragma warning (disable:4311)

	// Lay proc mac dinh cua edit
	WNDPROC oldProc = (WNDPROC)GetPropW(hWnd,L"Wprc");

#pragma warning (default:4311)

	// Disable WM_CHAR cua comboBox
	if ( uMsg == WM_CHAR )
	{
		int code = ((lParam & 0x00FF0000) >> 16);	
		if ( 
				code != 1 &&	// ESC
				code != 28 &&	// RETURN
				code != 15		// TAB
			)
		{			
			return 0;
		}		
	}

	// Call lai proc mac dinh do
	if ( sm_globalEventWindow )
	{
		list<uiWindow*>::iterator iWin = sm_globalEventWindow->begin(), end = sm_globalEventWindow->end();
		while ( iWin != end )
		{
			(*iWin)->globalMessageMap( hWnd, uMsg, wParam, lParam );
			iWin++;
		}
	}

	return CallWindowProc((WNDPROC)oldProc ,hWnd,uMsg,wParam,lParam);

}