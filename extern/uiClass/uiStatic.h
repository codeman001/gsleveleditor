// uiStatic.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 25/1/2009 (30 tet, ngay cuoi nam)
// Thong tin: Giao thua di an tat nien roi ve ngu... chan...
//


#ifndef _UISTATIC_
#define _UISTATIC_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

namespace uiClass
{
	class DLLEXPORT uiStatic: public uiControl
	{			
	protected:
	public:
		uiStatic(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);
		virtual ~uiStatic(void);
		
		// getObject
		// Tra ve UIOBJECT
		virtual DWORD getObject();

		// create
		// Khoi tao button
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);
	};
}

#endif
