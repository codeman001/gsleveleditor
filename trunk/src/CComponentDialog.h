#ifndef _ADDCOMPONENTWINDOW_
#define _ADDCOMPONENTWINDOW_

#include "CBaseDialog.h"
#include "IObjectComponent.h"

class CComponentDialog: public CBaseDialog
{
protected:
	uiListProperty	*m_listControl;	
	CSerializable	*m_componentTemplate;

protected:

	// setEditMode
	// update edit control for variable type (float, string, number...)
	void setEditMode( NSSerializable::dataType type, uiListPropertyEdit *p );

public:
	CComponentDialog(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, CSerializable *componentTemplate = NULL);

	virtual  ~CComponentDialog();

	// updateEditType
	// set edit mode on all rows
	void updateEditType();
};

#endif