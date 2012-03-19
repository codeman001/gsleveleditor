#include "uiInclude.h"

#ifdef UICLASS_SUPPORT_RICHEDIT

#include "uiRichEdit.h"
#include "uiApplication.h"
#include <Richedit.h>
#include <algorithm>
#include <windowsx.h>

using namespace uiClass;

#define TWIPS_PER_POINT 20

uiRichEdit::~uiRichEdit(void)
{
}

// uiEdit
// Khoi tao uiEdit		
uiRichEdit::uiRichEdit(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	this->create( lpTitle, x,y,w,h, pParent );
}

// create
// Khoi tao uiEdit				
bool uiRichEdit::create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
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

#pragma warning(disable:4312)

	// create font format
	CHARFORMAT2 charDefaultFormat;

	// create font format
	memset(&charDefaultFormat, 0, sizeof(charDefaultFormat));
    
	charDefaultFormat.cbSize = sizeof(CHARFORMAT2);

    // don't change these properties
    charDefaultFormat.dwMask = CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_SUPERSCRIPT | CFM_SUBSCRIPT;

    // change these.
    charDefaultFormat.dwMask |= CFM_FACE | CFM_SIZE | CFM_COLOR | CFM_BACKCOLOR;
    charDefaultFormat.dwEffects |= CFE_AUTOCOLOR | CFE_AUTOBACKCOLOR;
    
	// set font
	uiString::copy<WCHAR>(charDefaultFormat.szFaceName, L"Tahoma");

	// set size
    charDefaultFormat.yHeight = (LONG)(24 * TWIPS_PER_POINT);
	// end


	HRESULT hr = E_FAIL;
    HMODULE hModule = LoadLibrary(L"Riched20.dll");
    if (NULL != hModule)
    {        
        zm_hWnd = CreateWindowEx
			(
				ES_SUNKEN, 
				RICHEDIT_CLASS, lpTitle,
				ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 
				x, y, w, h,
				hParent, 
				(HMENU) uiApplication::controlRegister(this), 
				hInst, 
				NULL
			);

        if (zm_hWnd)
        {
            SendMessage(zm_hWnd, (UINT) EM_SETEVENTMASK, 0, (LPARAM) ENM_SELCHANGE);
            SendMessage(zm_hWnd, (UINT) EM_SETCHARFORMAT, (WPARAM) SCF_SELECTION, (LPARAM)&charDefaultFormat);

            hr = S_OK;
        }
        else
        {
            FreeLibrary(hModule);
        }
    }	
#pragma warning(default:4312)

	// Thay doi WND
	this->subClass();

	// Thiet lap quan he
	pParent->addChild(this);
	this->setParent(pParent);
	
	// Thay doi font chu
	SendMessage(zm_hWnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(false, 0));

	return true;
}

// messageControlMap
// Phan loai Message
void uiRichEdit::messageControlMap(UINT uMsg)
{
}
	
// getObject
// Lay gia tri UIOBJECT_
DWORD uiRichEdit::getObject()
{
	return UIOBJECT_RICHEDIT;
}

// getTextLong
// Lay so luong text cua edit
DWORD uiRichEdit::getTextLong()
{
	GETTEXTLENGTHEX input;
	input.codepage = GT_DEFAULT;
	input.flags = 1200;

	DWORD size = (DWORD)SendMessage( this->getHandle(), EM_GETTEXTLENGTHEX, (WPARAM) &input, NULL );
	return size;
}

// getText
// Lay text ra buffer
DWORD uiRichEdit::getText( void *lpBuffer, int size )
{	
	GETTEXTEX input;

	input.cb = size;
	input.codepage = GT_DEFAULT;
	input.flags = 1200;
	input.lpDefaultChar = NULL;
	input.lpUsedDefChar = NULL;

	DWORD ret = (DWORD)SendMessage( this->getHandle(), EM_GETTEXTEX, (WPARAM)&input, (LPARAM)lpBuffer );

	return ret;
}

#endif //UICLASS_SUPPORT_RICHEDIT