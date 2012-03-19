// uiRebarWindow.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 3/3/2009 
// Thong tin:
//

#ifndef _REBARWINDOW_
#define _REBARWINDOW_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

namespace uiClass
{
	class DLLEXPORT uiRebarWindow: public uiWindow
	{
	protected:	
	public:
		uiRebarWindow(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);
		virtual ~uiRebarWindow();

		virtual LRESULT messageMap( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	};
}

#endif
