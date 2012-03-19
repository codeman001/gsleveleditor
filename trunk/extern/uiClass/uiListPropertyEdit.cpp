#include "uiListPropertyEdit.h"
#include "uiInclude.h"

using namespace uiClass;

uiListPropertyEdit::uiListPropertyEdit( uiWindow *pParent )
	:uiEdit(L"", 0,0, 10,100, false, pParent)
{
	m_inputFloat = false;
}

uiListPropertyEdit::~uiListPropertyEdit()
{
}
		
// messageMap
// Phan loai message
LRESULT uiListPropertyEdit::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ( uMsg == WM_CHAR && m_inputFloat == true )
	{
		wchar_t c = (wchar_t)wParam;
		bool number = (( c >= '0' && c <= '9' ) || c == '.' || c == 8 );	// 8 is backspace
		
		if ( number == false )
			return -1;
	}
	return uiEdit::messageMap( hWnd, uMsg, wParam, lParam );
}

// setNumberFloatValueOnly
// Chi duoc nhap so thuc
void uiListPropertyEdit::setNumberFloatValueOnly( bool b )
{
	m_inputFloat = b;
}