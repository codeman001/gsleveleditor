#ifndef _ADDTEMPLATEWINDOW_
#define _ADDTEMPLATEWINDOW_

#include "CBaseDialog.h"

#include "CObjTemplateFactory.h"
#include "CComponentFactory.h"

class CTemplateDialog: public CBaseDialog
{
protected:
	uiListProperty	*m_componentControl;
	uiListProperty	*m_propertyControl;

	CObjectTemplate	*m_objectTemplate;
public:
	CTemplateDialog(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, CObjectTemplate *objectTemplate = NULL);

	virtual ~CTemplateDialog();

};

#endif