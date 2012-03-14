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
	bool			m_isEditObjTemplate;
public:
	CTemplateDialog(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, CObjectTemplate *objectTemplate = NULL);

	virtual ~CTemplateDialog();

	// onComboboxEnableComponent
	// when user change combobox enable, disable component
	virtual void onComboboxEnableComponent( uiObject *pSender );

	// onUpdateListComponent
	// when user update list control component
	virtual void onUpdateListComponent(uiObject *pSender );

	// onUpdateListProperty
	// when user update list control property
	virtual void onUpdateListProperty(uiObject *pSender );

protected:

	// setListPropertyValue
	// add value from list component to property
	void setListPropertyValue();
	
	// addSerializableToProperty
	// add serializable
	void addSerializableToProperty( CSerializable *p );

};

#endif