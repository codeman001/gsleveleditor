// uiGroupBox.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 1/1/2009
// Thong tin:
//		Happy new year 2009!!!

#ifndef _UIGROUPBOX_
#define _UIGROUPBOX_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

namespace uiClass
{
	class DLLEXPORT uiGroupbox: public uiControl
	{
	protected:
		uiGroupbox(void);		

	public:		
		uiGroupbox(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);
		virtual ~uiGroupbox(void);
		
		// getObject
		// Lay gia tri UIOBJECT
		virtual DWORD getObject();

		// create
		// Khoi tao 1
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);		

		// Phan loai msg cho control
		virtual void messageControlMap(UINT uMsg);		
	};
}

#endif
