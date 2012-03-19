// uiListProperty.h

// Lap trinh vien: Pham Hong Duc
// Email: duc.phamhong@gameloft.com
// Ngay viet: 12/10/2011
// Thong tin:
//

#ifndef _UILISTPROPERTY_
#define _UILISTPROPERTY_

#include "uiListPropertyItem.h"
#include "uiListPropertyGroup.h"
#include "uiListPropertyRow.h"

#include "uiInclude.h"
#include "uiWindow.h"

using std::vector;

// Ko can bao warning vector<dll class>
#pragma warning(disable: 4251)

namespace uiClass
{		
	class DLLEXPORT uiListProperty: public uiWindow
	{
	public:
		uiListProperty(LPWSTR lpTitle,int x, int y, int w, int h, uiWindow* pParent, int nCol);
		virtual ~uiListProperty(void);
	protected:
		int zm_scrollY;
		int zm_col;
	
		bool zm_showVScroll;
		bool zm_needUpdateScroll;

		bool zm_mouseDown;

		list<uiListPropertyItem*>	zm_listItem;
		uiListPropertyItem*			zm_hitItem;
		uiListPropertyItem*			zm_activeItem;

		uiCursor					zm_cursor;

		int zm_oldX;
		int zm_oldY;
		int zm_hItemDraw;
		bool zm_eraseDrag;

		int zm_lastHeight;

	protected:
		// _OnPaint
		// Wzm_PAINT
		virtual void _OnPaint( uiGraphics* pG );		

		// Wzm_MOUSEMOVE
		virtual void _OnMouseMove( uiMouseEvent e, int x, int y);

		// Wzm_LBUTTONDOWN
		virtual void _OnLButtonDown ( uiMouseEvent e, int x, int y );

		// Wzm_LBUTTONUP
		virtual void _OnLButtonUp( uiMouseEvent e, int x, int y );

		// Wzm_VSCROLL
		virtual void _OnVScroll( uiScrollBarEvent scrollEvent );

		// Wzm_SIZE
		virtual void _OnSize( uiSizeEvent e, int w, int h );

		// globalMessageMap
		// listen all event from this application
		virtual bool globalMessageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	protected:		
		uiEventCallback		eventOnUpdateProperty;
		uiEventCallback		eventOnActive;
		uiEventCallback		eventOnDeActive;

		uiEventCallback		eventOnCmbChange;
		uiEventCallback		eventOnEditChange;
	public:
		// Su kien OnClick
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnUpdateProperty(T* pObject)	{	eventOnUpdateProperty.setEvent<T,pTMethod>( pObject );	}
		
		// Su kien OnActive
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnActive(T* pObject)	{	eventOnActive.setEvent<T,pTMethod>( pObject );	}
		
		// Su kien OnActive
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnDeActive(T* pObject)	{	eventOnDeActive.setEvent<T,pTMethod>( pObject );	}

		// Su kien OnCmbChange
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnCmbChange(T* pObject)	{	eventOnCmbChange.setEvent<T,pTMethod>( pObject );	}

		// Su kien OnEditChange
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnEditChange(T* pObject)	{	eventOnEditChange.setEvent<T,pTMethod>( pObject );	}
	public:
		
		// getItemCount
		// lay so luong item
		int getItemCount();

		// getGroup
		// lay group thu i
		uiListPropertyItem* getItem(int i);

		// getHitTest
		// lay doi tuong dang click
		uiListPropertyItem* getHitTest(int x, int y);
		
		// addGroup
		// them group
		uiListPropertyGroup* addGroup(LPWSTR lpLabel);

		// addRowItem
		// them row 
		uiListPropertyRow*	addRowItem(LPWSTR lpLabel);

		// deleteAllGroup
		// xoa toan bo group
		void deleteAllItem();

		// deleteGroup
		// xoa group thu i
		void deleteItem( int i );


		// getActiveItem
		// lay item dang focus
		uiListPropertyItem* getActiveItem();

		// lostFocus
		// cancel edit cac property
		void lostFocus();

		// onCmbChange
		// su kien khi cmb tren property change
		virtual void _onCmbChange( uiObject *pSender );

		// _onEditChange
		// su kien khi edit text thay doi
		virtual void _onEditChange( uiObject *pSender );
	public:
		
		// reDrawInterface
		// ve lai doi tuong
		void reDrawInterface();

		// getItemColSize
		// lay chieu rong cua cot
		int getItemColSize( int col );

	protected:
		void setItemColSize( int col, int size );
	};

}
#endif
