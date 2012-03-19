// uiListPropertyGroup.h

// Lap trinh vien: Pham Hong Duc
// Email: duc.phamhong@gameloft.com
// Ngay viet: 13/10/2011
// Thong tin:
//

#ifndef _UILISTPROPERTYCOMBOBOX_
#define _UILISTPROPERTYCOMBOBOX_

#include "uiInclude.h"
#include "uiComboBox.h"

namespace uiClass
{
	class DLLEXPORT uiListPropertyComboBox: public uiComboBox
	{
	protected:
		HWND	zm_hEdit;
	public:
		uiListPropertyComboBox( uiWindow *pParent );

		// windowProc		
		// Muc dich de subclass lai control edit ben trong ComboBox
		static LRESULT CALLBACK editComboBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	};
}

#endif