// uiEvent.h
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 15/12/2008
// Thong tin:
//		- Khai bao cac doi tuong event


#ifndef _UIEVENT_
#define _UIEVENT_

#include "uiInclude.h"
#include "uiObject.h"
#include "uiStringTemplate.h"

namespace uiClass
{
	// uiMouseEvent
	// Doi tuong ho tro su kien chuot WM_MOUSE
	class DLLEXPORT uiMouseEvent:public uiObject
	{
		protected:
			UINT zm_uParam;
		public:
			uiMouseEvent(UINT uParam);
			virtual ~uiMouseEvent(void);

			// getObject
			// Tra ve UIOBJECT_EVENTMOUSE
			virtual DWORD getObject();

			// isCtrlKeyDown
			// Tra ve true neu phim control giu
			bool isCtrlKeyDown();

			// isShiftKeyDown
			// Tra ve true neu phim shift giu
			bool isShiftKeyDown();
			
			// isAltKeyDown
			// Tra ve true neu phim alt giu
			bool isAltKeyDown();

			// isLeftButtonDown
			// Tra ve true neu chuot trai giu
			bool isLeftButtonDown();

			// isMiddleButtonDown
			// Tra ve true neu chuot phai giu
			bool isMiddleButtonDown();
			
			// isWheelUp
			// Tra ve true neu chuot giua day len (false day xuong)
			bool isWheelUp();

			// isRightButtonDown
			// Tra ve true neu chuot giua giu
			bool isRightButtonDown();			
	};

	// uiKeyEvent
	// Doi tuong ho tro su kien tu ban phim WM_KEY
	class DLLEXPORT uiKeyEvent:public uiObject
	{
	protected:
		UINT zm_wParam;
		UINT zm_lParam;	
	public:
		uiKeyEvent(UINT wParam, UINT lParam);
		virtual ~uiKeyEvent(void);
		
		// getObject
		// Tra ve UIOBJECT_EVENTKEY
		virtual DWORD getObject();

		// getVKeyCode
		// Tra lai ma VK_ cua ban phim vua bam
		UINT getVKeyCode();

		// getAnsiKey
		// Tra lai ky tu ansi
		char getAnsiKey();

		// getUnicodeKey
		// Tra lai ky tu unicode
		UINT getUnicodeKey();

		// getEventLoop		
		UINT getEventLoop();		
	};

	// uiCharEvent
	// Doi tuong ho tro su kien WM_CHAR
	class DLLEXPORT uiCharEvent:public uiObject
	{
	protected:
		UINT zm_wParam;
		UINT zm_lParam;	
	public:
		uiCharEvent(UINT wParam, UINT lParam);
		virtual ~uiCharEvent();

		// getObject
		// Tra ve UIOBJECT_EVENTCHAR
		virtual DWORD getObject();

		// getRepeatCount
		// Lay so lan lap
		UINT getRepeatCount();

		// getScanCode
		// Lay ma code
		UINT getScanCode();		

		// isAltDown
		// true neu ALT giu
		bool isAltDown();

		// isExKeyDown
		// true neu phim mo rong giu
		bool isExKeyDown();
	};
	
	// uiSizeEvent
	// Doi tuong ho tro WM_SIZE
	class DLLEXPORT uiSizeEvent:public uiObject
	{
	protected:
		int zm_nParam;
	public:
		uiSizeEvent(int nParam);
		virtual ~uiSizeEvent();
		
		// getObject
		// Tra ve UIOBJECT_EVENTSIZE
		virtual DWORD getObject();

		// isMaximum
		// Tra ve true neu cua so max
		bool isMaximum();

		// isMinimum
		// Tra ve true neu cua so min
		bool isMinimum();

		// isRestored
		// Tra ve true neu cua so retor
		bool isRestored();	
	};
	
	// uiFiledrop
	// Doi tuong ho tro WM_DROPSFILE
	class DLLEXPORT uiFileDrop:public uiObject
	{
	protected:
		std::vector<uiStringW>	m_listFile;
	public:
		// getObject
		// Tra ve UIOBJECT_FILEDROP
		virtual DWORD getObject();

		// add file to list
		void addFile( WCHAR* lpFile );
		
		// getFileCount
		// Lay so luong file
		int getFileCount();

		// getFilePath
		// Lay duong dan file
		WCHAR* getFilePath( int index );
	};

	// uiScrollBarEvent
	// Doi tuong ho tro event WM_HSCROLL & WM_VSCROLL
	class DLLEXPORT uiScrollBarEvent: public uiObject
	{
	protected:
		uiObject	*zm_scroll;
		int			zm_pos;
		int			zm_min;
		int			zm_max;
	public:
		uiScrollBarEvent( uiObject *pControl, int pos, int min, int max );

		// getObject
		// Tra ve UIOBJECT_SCROLLBAREVENT
		virtual DWORD getObject();

		// getPos
		// Lay gia tri hien tai
		int getPos();

		// getMin
		// Lay gioi han nho nhat
		int getMin();

		// getMax
		// Lay gioi han cao nhat
		int getMax();

		// getControl
		// Lay con tro control bar (co the la windows)
		uiObject* getControl();
	};

}


#endif
