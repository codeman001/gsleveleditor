#ifndef _OBJECTPROPERTYCOMPONENT_
#define _OBJECTPROPERTYCOMPONENT_

#include "CGameObject.h"
#include "IObjectComponent.h"

class CObjectPropertyComponent: public IObjectComponent
{
protected:
	bool	m_enableDirectionLight;
	bool	m_enablePointLight;

public:
	CObjectPropertyComponent(CGameObject *owner);
	
	virtual ~CObjectPropertyComponent();

	// init
	// run when init object
	virtual void initComponent();

	// update
	// run when update per frame
	virtual void updateComponent();

	// saveData
	// save data to serializable
	virtual void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	virtual void loadData( CSerializable* pObj );
public:

	// isEnableDirectionLight
	// enable direction light
	inline bool isEnableDirectionLight()
	{
		return m_enableDirectionLight;
	}

	// isEnablePointLight
	// enable point light
	inline bool isEnablePointLight()
	{
		return m_enablePointLight;
	}
};

#endif