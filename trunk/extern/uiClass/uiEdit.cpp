// uiButton.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 2/1/2009
// Thong tin:

#include "uiEdit.h"
#include "uiInclude.h"
#include "uiApplication.h"

#include <algorithm>
#include <windowsx.h>

using namespace uiClass;

uiEdit::uiEdit()
{

}

uiEdit::~uiEdit(void)
{

}
		
// uiEdit
// Khoi tao uiEdit		
uiEdit::uiEdit(LPCWSTR lpTitle, int x, int y, int w, int h, bool bPasswordEdit , uiWindow* pParent)
{
	this->create( lpTitle, x,y,w,h ,bPasswordEdit, pParent );
}

// create
// Khoi tao uiEdit
// Tham so: 
bool uiEdit::create(LPCWSTR lpTitle, int x, int y, int w, int h, bool bPasswordEdit , uiWindow* pParent)
{
	assert( pParent );
		
	HINSTANCE hInst = pParent->getInstance();	
	HWND hParent;
	if (pParent == NULL)
		hParent = NULL;
	else
		hParent = pParent->getHandle();		
	
	zm_pParent = pParent;
	 
	this->zm_hInstance = hInst;	

	DWORD dwStyle = ES_AUTOHSCROLL|ES_AUTOVSCROLL;
	if ( bPasswordEdit )
		dwStyle |= ES_PASSWORD;

#pragma warning(disable:4312)
	//Khoi tao cua so
	zm_hWnd = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		L"Edit",
		lpTitle,
		WS_CHILD | WS_TABSTOP | WS_VISIBLE | dwStyle,
		x,y,w,h,
		hParent,
		(HMENU) uiApplication::controlRegister(this),
		hInst, NULL);
#pragma warning(default:4312)

	// Thay doi WND
	this->subClass();

	// Thiet lap quan he
	pParent->addChild(this);
	this->setParent(pParent);
	
	// Thay doi font chu
	SendMessage(zm_hWnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
               MAKELPARAM(false, 0));

	return true;
}

// messageControlMap
// Phan loai Message
void uiEdit::messageControlMap(UINT uMsg)
{
	switch (uMsg)
	{
	case EN_CHANGE:
		eventOnChange(this);
		break;
	case EN_UPDATE:
		eventOnUpdate(this);
		break;
	case EN_SETFOCUS:
		eventOnFocus(this);
		break;
	case EN_KILLFOCUS:
		eventOnLostFocus(this);
		break;
	}	
}

// getObject
DWORD uiEdit::getObject()
{
	return UIOBJECT_EDIT;
}

// editEmptyUndoBuffer
// Xoa Undo
void uiEdit::emptyUndoBuffer()
{
	Edit_EmptyUndoBuffer( this->getHandle() );
}

// editGetLine
// Lay du lieu o dong thu nLine
int  uiEdit::getLine(int nLine, LPWSTR lpString, int charCount)
{
	int i = Edit_GetLine( this->getHandle(), nLine, lpString, charCount );	
	if (i != 0)
		lpString[i] = 0;

	return i;
}

// editGetLineCount
// Dem so dong
int	 uiEdit::getLineCount()
{
	return Edit_GetLineCount( this->getHandle() );
}

// editIsModify
// Kiem tra edit co thay doi hay ko
bool uiEdit::isModify()
{
#pragma warning(disable: 4800)
	return (bool)Edit_GetModify( this->getHandle() );
}

// editGetText
// Lay toan bo text
void uiEdit::getText(LPWSTR lpText, int nCount)
{
	GetWindowTextW( this->getHandle(), lpText, nCount );
}

// editGetTextCount
// Lay so ky tu hien tai
int  uiEdit::getTextCount()
{
	return Edit_GetTextLength( this->getHandle() );
}

void uiEdit::hideBalloonTip()
{
#ifndef EM_HIDEBALLOONTIP
	#define EM_HIDEBALLOONTIP 0x1504
#endif
	SendMessage( this->getHandle(), EM_HIDEBALLOONTIP, 0,0 );
}

// setNumberValueOnly
// Chi cho phep nhap number
void uiEdit::setNumberValueOnly( bool b )
{
	if (b == true)
		this->changeStyle(ES_NUMBER, 0);
	else
		this->changeStyle(0, ES_NUMBER);
}

// editSetLimitText
// Gioi han text
void uiEdit::setLimitText(int nLimit)
{
	Edit_LimitText( this->getHandle(), nLimit);
}		

// editReplaceSel
// Thay the chuoi duoc lua chon bang lpString
void uiEdit::replaceSel(LPWSTR lpString)
{
	Edit_ReplaceSel( this->getHandle(), lpString );
}

// editScroll
// Dich chuyen con tro
void uiEdit::scroll(int nLine, int nPos)
{
	Edit_Scroll( this->getHandle(), nLine, nPos );
}

// editScrollCaretToBegin
// Dich chuyen con tro toi vi tri dau tien
void uiEdit::scrollCaretToBegin()
{
	Edit_ScrollCaret( this->getHandle() );
}

// editSetPasswordChar
// Thiet lap ky tu cho password
void uiEdit::setPasswordChar( WCHAR wChar )
{
	Edit_SetPasswordChar( this->getHandle(), wChar );
}

// editSetReadOnly
// ReadOnly Editbox
void uiEdit::setReadOnly(bool b)
{
	Edit_SetReadOnly( this->getHandle(), b);
}

// editSetSelect
// Lua chon text
void uiEdit::setSelect( int nStart,int nEnd)
{
	Edit_SetSel( this->getHandle(), nStart, nEnd );
}

// editSetText
// Thiet lap text cho Edit
void uiEdit::setText( LPWSTR lpText )
{
	SetWindowTextW( this->getHandle(), lpText );
}		

// editUndo
// Undo du lieu
void uiEdit::undo()
{
	Edit_Undo( this->getHandle() );
}