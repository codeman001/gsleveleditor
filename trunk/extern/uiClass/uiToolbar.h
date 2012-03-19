// uiToolbar.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 18/2/2009 
// Thong tin:
//

#ifndef _UITOOLBAR_
#define _UITOOLBAR_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

using std::vector;

namespace uiClass
{
	class uiToolbarButton;
	class uiToolbar;

// Neu bien dich bang g++ /////////////////////
#ifdef __GNUG__
	typedef struct {
		UINT cbSize;
		DWORD dwMask;
		int idCommand;
		int iImage;
		BYTE fsState;
		BYTE fsStyle;
		WORD cx;
		DWORD_PTR lParam;
		LPWSTR pszText;
		int cchText;
	} TBBUTTONINFOW, *LPTBBUTTONINFOW;
#endif
///////////////////////////////////////////////
	// uiToolbarButton
	// Doi tuong ToolbarButton
	class DLLEXPORT uiToolbarButton: public uiObject
	{
	protected:
		WCHAR			zm_pLabel[200];
		WCHAR			zm_pTooltip[500];
		uiToolbar		*zm_pToolbar;
		TBBUTTONINFOW	zm_button;
		DWORD			zm_idButton;

		uiMenuPopup		*zm_pMenuPopup;

		uiEventCallback	eventOnClicked;
		uiEventCallback	eventOnPopupMenu;
	public:
		uiToolbarButton( uiToolbar *p, LPWSTR lpLabel, DWORD iImage );

		virtual ~uiToolbarButton();

		// Su kien OnClick
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnClicked(T* pObject)	{	eventOnClicked.setEvent<T,pTMethod>( pObject );	}

		// Su kien OnPopupMenu
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnPopupMenu(T* pObject){	eventOnPopupMenu.setEvent<T,pTMethod>( pObject );	}


		// getObject
		// Tra ve doi tuong UIOBJECT_
		virtual DWORD getObject();

		// setLabel
		// Thiet lap label cho button
		void setLabel( LPWSTR lpString );

		// setImage
		// Thiet lap image cho button
		void setImage( DWORD nIndex );

		// getStruct
		// Lay thong tin struct cua button
		TBBUTTONINFOW* getStruct();
		
		// setID
		// Thiet lap ID cho button
		void setID( DWORD id );

		// getID
		// Lay id hien hanh
		DWORD getID();

		// setCheck
		// Thiet lap trang thai button
		void setCheck( bool check );

		// getCheck
		// Lay trang thay button
		bool isCheck();

		// enableButton
		// Cho phep button hoat dong hay ko
		void enableButton( bool b );

		// isEnable
		// Kiem tra trang thai button
		bool isEnable();

		// update
		// Cap nhat trang thai button
		void update();

		// setMenuPopup
		// Thiet lap menu popup cho button
		void setMenuPopup( uiMenuPopup* pMenu );
		
		// getMenuPopup
		// Lay menu popup
		uiMenuPopup* getMenuPopup();

		// doEventClick
		// Thuc thi su kien click
		void doEventClick();

		// doEventPopupMenu
		// Thuc thi su kien menu push
		void doEventPopupMenu();

		// setButtonType
		// Thay doi kieu cua button
		// dwType la const UITOOLBARBUTTON_
		void setButtonType( DWORD dwType );	

		// setTip
		// Thiet lap string tooltip cho button
		void setTip( LPWSTR lpString );

		// getTip
		// Lay toolTip
		LPWSTR getTip();

		// getLabel
		// Lay label cua Button
		LPWSTR getLabel();
	};


	// uiToolbar
	// Doi tuong chua cac button 
	class DLLEXPORT uiToolbar: public uiControl
	{
	protected:
		HIMAGELIST zm_listImage;
		vector< uiToolbarButton*> zm_listButton;
	public:	
		uiToolbar(LPCWSTR lpTitle, int x, int y, int w, int h, DWORD iconW, DWORD iconH, uiWindow* pParent);
		virtual ~uiToolbar(void);
		
		// getObject
		// Tra ve UIOBJECT
		virtual DWORD getObject();		

		// create
		// Khoi tao button
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, DWORD iconW, DWORD iconH, uiWindow* pParent);
		
		// pushImage
		// Them 1 image button vao toolbar
		DWORD pushImage ( uiBitmap *p );

		// pushImage
		// Them 1 image button vao toolbar
		DWORD pushImage ( uiBitmap *p, uiColor mask );

		// pushImage
		// Them 1 image button vao toolbar
		DWORD pushImage ( uiIcon *p );

		// removeImage
		// Xoa image
		void removeImage( DWORD index );
		
		// addButton
		// Them 1 button vao toolbar
		uiToolbarButton* addButton( LPWSTR lpLabel, DWORD iImage );
		
		// addSeparator
		// Them 1 button 1 khoang cach
		uiToolbarButton* addSeparator();

		// getButton
		// Lay doi tuong button
		uiToolbarButton* getButton( DWORD index );
		
		// insertButton
		// Them 1 button vao toolbar
		uiToolbarButton* insertButton( DWORD index, LPWSTR lpLabel, DWORD iImage );

		// deleteButton
		// Xoa button tren toolbar
		void deleteButton( DWORD index );

		// createButtonID
		// Khoi tao ID cho button
		DWORD createButtonID();
		
		// setButtonWH
		// Thiet lap chieu rong + cao cho button
		void setButtonWH(DWORD w, DWORD h);

		// setImageWH
		// Thiet lap chieu rong + cao cho image
		void setImageWH(DWORD w, DWORD h);
		
		// setTextRight
		// Thiet lap text tren phai cho toolbar (true/false)
		void setTextRight( bool b );
	};
}

#endif
