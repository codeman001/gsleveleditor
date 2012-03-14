#ifndef _OBJECTTEMPLATE_
#define _OBJECTTEMPLATE_

#include "IObjectComponent.h"
#include "CComponentFactory.h"

class CObjectTemplate
{
protected:
	uiStringW	m_templateName;
	int			m_objectTemplateID;
	vector<int>	m_objectComponents;
public:
	CObjectTemplate();
	virtual ~CObjectTemplate();

	// setTemplateID
	// set template id
	inline void setTemplateID( int id )
	{
		m_objectTemplateID = id;
	}

	// getTemplateID
	// return template id
	inline int getTemplateID()
	{
		return m_objectTemplateID;
	}

	// addComponent
	// add component to object
	bool addComponent( int comID );
	
	// removeComponent
	// remove component from object
	bool removeComponent( int comID );
};

#endif