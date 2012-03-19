// uiToolTip.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 24/2/2009
// Thong tin:

#ifndef _UITOOLTIP_
#define _UITOOLTIP_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"
#include <vector>
#include <string>

using std::vector;
using std::wstring;

namespace uiClass
{
	class DLLEXPORT uiToolTip
	{
	protected:
		static HWND			zm_hWnd;
		static TOOLINFOW	zm_toolInfo;
	public:
		// showTip 
		// Hien thi tooltips
		static void showTip ( uiWindow *pWin, LPWSTR lpCaption, LPWSTR lpString, DWORD dwIcon,
			bool bIsBalloon, bool bIsClose,
			int x, int y);
		
		static void showTip ( uiWindow *pWin, LPWSTR lpString, int x, int y);

		// hideTip
		// An tip di
		static void hideTip();

		// setTipPos
		// Thiet lap tipPos
		static void setTipPos( int x, int y);

		// setTipString
		// Thay doi string
		static void setTipString( LPWSTR lpString );

		static void setTipString( LPWSTR lpString, LPWSTR lpCaption, DWORD dwIcon );

		// destroyTip
		// Huy tooltip
		static void destroyTip();
	};
}

#endif
