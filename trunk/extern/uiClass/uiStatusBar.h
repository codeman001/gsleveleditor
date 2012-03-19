// uiStatusBar.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 2/3/2009 
// Thong tin:
//
#ifndef _UISTATUSBAR_
#define _UISTATUSBAR_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

using std::vector;

namespace uiClass
{
	typedef vector<uiWindow*> uiListWindows;

	class DLLEXPORT uiStatusBar: public uiControl
	{
	protected:
		uiListWindows	zm_listWindow;
	public:
		uiStatusBar(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);
		virtual ~uiStatusBar(void);

		// create
		// Khoi tao status bar
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);

		// getObject
		// Lay const doi tuong
		virtual DWORD getObject();

		// createParts
		// Khoi tao cac thanh phan cua Part
		void createParts( int nPart, int *size );

		// setIcon
		// Thiet lap icon cho part
		void setIcon( int part, uiIcon *pIcon );

		// setText
		// Thiet lap text cho part
		void setText( int part, LPWSTR lpText );

		// setToolTip
		// Thiet lap tooltip cho part
		void setToolTip( int part, LPWSTR lpToolTip );

		// getRect
		// Lay kich thuoc rect
		void getRect( int part, LPRECT lpRect );

		// setWindow
		// Thiet lap cua so trong part
		void setWindow( DWORD part, uiWindow *pWin );

		// _OnSize
		// Su kien onSize
		virtual void _OnSize( uiSizeEvent e, int w, int h );
	};
}

#endif
