// uiInclude.h
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 15/12/2008
// Thong tin:
//		- Khai bao cac thu vien dung chung


#ifndef _UIINCLUDE_
#define _UIINCLUDE_

#include <windows.h>
#include <uxtheme.h>
#include <Vsstyle.h>
#pragma comment(lib,"uxtheme.lib")

#include <assert.h>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdarg.h>

#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")

#include "uiDebug.h"

#define DLLEXPORT __declspec(dllexport)

#define UICLASS_SUPPORT_RICHEDIT

// Khai bao doi tuong cho uiGraphics
#define UIOBJECT_PEN		0x01
#define UIOBJECT_BRUSH		0x02
#define UIOBJECT_FONT		0x03
#define UIOBJECT_GRAPHICS	0x04

// Khai bao doi tuong cho uiMenu
#define UIOBJECT_MENU		0x05
#define UIOBJECT_MENUITEM	0x06

// Khai bao doi tuong chi uiWindow
#define UIOBJECT_WINDOW		0x07
#define UIOBJECT_FORM		0x08
#define UIOBJECT_DIALOG		0x08

#define UIOBJECT_EVENTCALL	0x09
#define UIOBJECT_COLOR		0x0A

#define UIOBJECT_EVENTMOUSE	0x0B
#define UIOBJECT_EVENTKEY	0x0C
#define UIOBJECT_EVENTCHAR	0x0D
#define UIOBJECT_EVENTSIZE	0x0E

#define UIOBJECT_DOCK		0x0F
#define UIOBJECT_CTLSPLIT	0x10
#define UIOBJECT_ICON		0x11
#define UIOBJECT_CURSOR		0x12

#define UIOBJECT_BITMAP		0x13
#define UIOBJECT_MENUPOPUP	0x14

#define UIOBJECT_CONTROL	0x15
#define UIOBJECT_BUTTON		0x16
#define UIOBJECT_CHKBOX		0x17
#define UIOBJECT_RADIOBUTTON 0x18
#define UIOBJECT_RADIOGROUP 0x19
#define UIOBJECT_GROUPBOX	0x1A
#define UIOBJECT_COMBOBOX	0x1B
#define UIOBJECT_EDIT		0x1C
#define UIOBJECT_LISTVIEW	0x1D
#define UIOBJECT_LISTVIEWCOL 0x1E
#define UIOBJECT_LISTVIEWROW 0x1F
#define UIOBJECT_STATIC		0x20
#define UIOBJECT_LISTBOX	0x21
#define UIOBJECT_PROGRESSBAR 0x22
#define UIOBJECT_TREEVIEW	0x23
#define UIOBJECT_REBAR		0x24
#define UIOBJECT_TOOLBAR	0x25
#define UIOBJECT_REBARBAND	0x26
#define UIOBJECT_TOOLBARBUTTON	0x27
#define UIOBJECT_TABCONTROL	0x28
#define UIOBJECT_TOOLTIP	0x29
#define UIOBJECT_STATUSBAR	0x3A

#define UIOBJECT_LOCKLAYOUT	0x2A
#define UIOBJECT_KEYACCEL	0x2B
#define UIOBJECT_KEYACCELENTRY	0x2B
#define UIOBJECT_TABCONTROLLER	0x2C

#define UIOBJECT_FILEDROP	0x2D
#define UIOBJECT_RICHEDIT	0x2E
#define UIOBJECT_SCROLLBAREVENT 0x2F

// Khai bao stype cho uiWindow
#define UISTYLE_POPUP		0x00
#define UISTYLE_NORESIZE	0x01
#define UISTYLE_RESIZE		0x02
#define UISTYLE_TOOL		0x03
#define UISTYLE_CHILD		0x04

// Khai bao const Dock
#define UIDOCK_NONE			0x00
#define UIDOCK_TOP			0x01
#define UIDOCK_BOTTOM		0x02
#define UIDOCK_LEFT			0x03
#define UIDOCK_RIGHT		0x04
#define UIDOCK_FILL			0x05
#define UIDOCK_CENTER		0x06

// Khai bao const LockLayout
#define UILOCKLAYOUT_NONE	0x00
#define UILOCKLAYOUT_LEFT	0x01
#define UILOCKLAYOUT_RIGHT	0x02
#define UILOCKLAYOUT_TOP	0x04
#define UILOCKLAYOUT_BOTTOM	0x08

// Khai bao const LISTVIEW_ALIGN
#define UILISTVIEW_MFT_LEFT			LVCFMT_LEFT
#define UILISTVIEW_MFT_RIGHT		LVCFMT_RIGHT
#define UILISTVIEW_MFT_CENTER		LVCFMT_CENTER
#define UILISTVIEW_MFT_IMGRIGHT		LVCFMT_BITMAP_ON_RIGHT

#define UITOOLBARBUTTON_BUTTON			0x01
#define UITOOLBARBUTTON_DROPDOWN		0x02
#define UITOOLBARBUTTON_WHOLEDROPDOWN	0x03
#define UITOOLBARBUTTON_SEPARATOR		0x04

#define UILISTPROPERTY_EDIT			0
#define UILISTPROPERTY_COMBOBOX		1
#define UILISTPROPERTY_CUSTOM		2

// uiTagMargin
// Struct khai bao le cho cua so
typedef struct uiTagMargin
{
	int nLeft;
	int nRight;
	int nBottom;
	int nTop;
}uiMargin;

// uiTagPoint
// Struct khai bao toa do cho Graphics
typedef struct uiTagPoint
{
	long x;
	long y;
}uiPoint;

#define UICURSOR_ARROW           MAKEINTRESOURCEW(32512)
#define UICURSOR_IBEAM           MAKEINTRESOURCEW(32513)
#define UICURSOR_WAIT            MAKEINTRESOURCEW(32514)
#define UICURSOR_CROSS           MAKEINTRESOURCEW(32515)
#define UICURSOR_UPARROW         MAKEINTRESOURCEW(32516)
#define UICURSOR_SIZE            MAKEINTRESOURCEW(32640)  /* OBSOLETE: use UICURSOR_SIZEALL */
#define UICURSOR_ICON            MAKEINTRESOURCEW(32641)  /* OBSOLETE: use UICURSOR_ARROW */
#define UICURSOR_SIZENWSE        MAKEINTRESOURCEW(32642)
#define UICURSOR_SIZENESW        MAKEINTRESOURCEW(32643)
#define UICURSOR_SIZEWE          MAKEINTRESOURCEW(32644)
#define UICURSOR_SIZENS          MAKEINTRESOURCEW(32645)
#define UICURSOR_SIZEALL         MAKEINTRESOURCEW(32646)
#define UICURSOR_NO              MAKEINTRESOURCEW(32648) /*not in win3.1 */

#define UICONTROL_MENU			1000		
#define UICONTROL_COMMON		2000
#define UICONTROL_TOOLBAR		3000

#endif
