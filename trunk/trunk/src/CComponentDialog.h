#ifndef _ADDCOMPONENTWINDOW_
#define _ADDCOMPONENTWINDOW_

#include "CBaseDialog.h"

class CComponentDialog: public CBaseDialog
{
public:
	CComponentDialog(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);

	virtual  ~CComponentDialog();
};

#endif