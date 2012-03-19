// uiListPropertyEdit.h

// Lap trinh vien: Pham Hong Duc
// Email: duc.phamhong@gameloft.com
// Ngay viet: 10/3/2012
// Thong tin:
//

#ifndef _UILISTPROPERTYEDIT_
#define _UILISTPROPERTYEDIT_

#include "uiInclude.h"
#include "uiEdit.h"

namespace uiClass
{
	class DLLEXPORT uiListPropertyEdit: public uiEdit
	{
	protected:
		bool	m_inputFloat;
	public:
		uiListPropertyEdit( uiWindow *pParent );
		virtual ~uiListPropertyEdit();
		
		// messageMap
		// Phan loai message
		virtual LRESULT messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

		// setNumberFloatValueOnly
		// Chi duoc nhap so thuc
		void setNumberFloatValueOnly( bool b );
	};
}

#endif