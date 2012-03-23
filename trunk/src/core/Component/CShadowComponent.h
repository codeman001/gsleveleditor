#ifndef _SHADOWCOMPONENT_
#define _SHADOWCOMPONENT_

#include "CGameObject.h"
#include "IObjectComponent.h"

class CShadowComponent: public IObjectComponent
{
protected:
	bool m_shadowCasting;
	bool m_shadowReceiving;

public:
	CShadowComponent(CGameObject *owner);
	
	virtual ~CShadowComponent();

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

	// isShadowCasting
	// return true if casting shadow
	inline bool isShadowCasting()
	{
		return m_shadowCasting;
	}

	// isShadowReceiving
	// return true if receiving shadow
	inline bool isShadowReceiving()
	{
		return m_shadowReceiving;
	}
};

#endif