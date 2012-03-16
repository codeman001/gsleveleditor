#ifndef _OBJECTTEMPLATE_
#define _OBJECTTEMPLATE_

#include "IObjectComponent.h"
#include "CComponentFactory.h"

class CObjectTemplate
{
protected:
	uiStringW				m_templateName;

#ifdef GSEDITOR	
	int						m_numObjRef;
	bool					m_isDirty;
#endif

	vector<CSerializable>	m_objectComponents;
public:

	CObjectTemplate();
	virtual ~CObjectTemplate();

#ifdef GSEDITOR

	// incRef
	// add a ref when gameobject use this template
	inline void incRef()
	{
		m_numObjRef++;
	}

	// decRef
	// remove a ref when gameobject donot use this template
	inline void decRef()
	{
		m_numObjRef--;
	}

	// getRef
	// return num object use this template
	inline int getRef()
	{
		return m_numObjRef;
	}

	// isDirty
	// return true if the template has change component
	// so we need reload object if it is dirty
	inline bool isDirty()
	{
		return m_isDirty;
	}

	// setDirty
	// set dirty to template	
	inline void setDirty( bool b = true )
	{
		m_isDirty = true;
	}

#endif
	// setObjectTemplateName
	// set template name
	inline void setObjectTemplateName( wchar_t *name )
	{
		m_templateName = name;
	}

	// getObjectTemplateName
	// get name
	inline wchar_t* getObjectTemplateName()
	{
		return m_templateName.c();
	}

	// containComponent
	// check the obj template has the componentID
	bool containComponent( char* comName );

	// getComponent
	// get component
	CSerializable *getComponent( char* comName );

	// addComponent
	// add component to object
	bool addComponent( char* comName );
	
	// removeComponent
	// remove component from object
	bool removeComponent( char* comName );

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