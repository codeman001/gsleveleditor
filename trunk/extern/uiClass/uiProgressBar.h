// uiProgressBar.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 25/1/2009 (30 tet, ngay cuoi nam)

#ifndef _UIPROGRESSBAR_
#define _UIPROGRESSBAR_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

namespace uiClass
{
	class DLLEXPORT uiProgressBar: public uiControl
	{
	protected:

	public:		
		uiProgressBar(LPCWSTR lpTitle, int x, int y, int w, int h, bool isMarquee, bool isVertical,uiWindow* pParent);
		virtual ~uiProgressBar(void);
		
		// getObject
		// Tra ve UIOBJECT
		virtual DWORD getObject();

		// create
		// Khoi tao button
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, bool isMarquee, bool isVertical, uiWindow* pParent);

		// setMarque
		// Cho Progress Bar chay
		// Luu y: uiProgressBar phai duoc khoi tao isMarque = true
		void setMarquee(bool b);

		// setRange
		// Thiet lap pham vi cho Progress Bar
		void setRange(int min, int max);

		// setPos
		// Thiet lap vi tri
		void setPos(int pos);

		// getPos
		// Tra ve vi tri hien tai
		int getPos();
	};
}

#endif
