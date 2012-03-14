#ifndef _OBJECTTEMPLATE_
#define _OBJECTTEMPLATE_

#include "IObjectComponent.h"
#include "CComponentFactory.h"

class CObjectTemplate
{
protected:
	uiStringW				m_templateName;
	int						m_objectTemplateID;
	vector<CSerializable>	m_objectComponents;
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

	// containComponent
	// check the obj template has the componentID
	bool containComponent( int comID );

	// addComponent
	// add component to object
	bool addComponent( int comID );
	
	// removeComponent
	// remove component from object
	bool removeComponent( int comID );

	// getAllComponentProperty
	// return list property
	inline ArraySerializable* getAllComponentProperty()
	{
		return &m_objectComponents;
	}
protected:
	
	// sortComponentByName
	// sort component on the list
	void sortComponentByName();

};

#endif