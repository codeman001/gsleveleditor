// uiRebar.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 18/2/2009 
// Thong tin:
//

#ifndef _UIREBAR_
#define _UIREBAR_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

namespace uiClass
{
	class uiRebarBand;
	class uiRebar;


	// uiRebarBand
	// Doi tuong nam trong rebar
	class DLLEXPORT uiRebarBand: public uiObject
	{
	protected:
		WCHAR			zm_Text[200];
		uiWindow		*zm_windows;
		REBARBANDINFOW	zm_info;

		bool			zm_bGripper;
		bool			zm_bBreak;
	public:
		uiRebarBand( uiWindow* p, LPWSTR lpText );
		
		virtual ~uiRebarBand();

		// getObject
		// Tra ve UIOBJECT
		virtual DWORD getObject();

		// setMinWidthHeight
		// Thiet lap chieu rong + cao nho nhat cho Band
		// Khi w hay h = 0 -> su dung chieu cao mac dinh
		void setMinWidthHeight(DWORD w, DWORD h);

		// setWidth
		// Thiet lap vi tri Band
		void setWidth( DWORD w );

		// getInfoStruct
		// Lay thong tin Band Info
		REBARBANDINFOW* getInfoStruct();

		// setBandWindow
		// Thiet lap Window
		void setBandWindow( uiWindow *pWin, LPWSTR lpText );

		// enableGripper
		// Cho phep hien thi Gripper
		void enableGripper( bool b );

		// setBreakBand
		// Cho phep xuong dong hay ko
		void setBreakBand( bool b );
	};

	// Rebar
	// Doi tuong rebar chua dung cac control nhu toolbar, comboBox,... giong IE
	class DLLEXPORT uiRebar: public uiControl
	{		
	protected:
		bool zm_bResizeFather;
	public:
		uiRebar(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);
		virtual ~uiRebar(void);
		
		// getObject
		// Tra ve UIOBJECT
		virtual DWORD getObject();

		// create
		// Khoi tao button
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);

		// addBand
		// Them 1 band vao Rebar
		void addBand( uiRebarBand *pBand );

		// messageMap
		// Phan loai message
		virtual LRESULT messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

		// notifyControl
		// Xu ly notify
		virtual void notifyControl(LPARAM lParam);

		// showGrip
		// Hien thi grip hay khong
		void showBandBorder( bool b );
	};
}

#endif
