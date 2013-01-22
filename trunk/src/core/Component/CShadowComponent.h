#ifndef _SHADOWCOMPONENT_
#define _SHADOWCOMPONENT_

#include "CGameObject.h"
#include "IObjectComponent.h"

class CShadowComponent: public IObjectComponent
{
protected:
	std::vector<video::S3DVertex>	m_listShadowPos;

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

	// addShadowNode
	// add shadow
	void setShadowNode( const core::vector3df& pos, const core::vector3df& normal )
	{
		video::S3DVertex v;
		v.Pos = pos;
		v.Normal = normal;
		m_listShadowPos.push_back(v);
	}

	// getShadowCount
	// get number of shadow
	inline int getShadowCount()
	{
		return m_listShadowPos.size();
	}

	// getListShadow
	// get list shadow
	inline std::vector<video::S3DVertex>& getListShadow()
	{
		return m_listShadowPos;
	}

};

#endif