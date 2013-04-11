#ifndef _SPAWNPOINT_COMP_
#define _SPAWNPOINT_COMP_

#include "IObjectComponent.h"
#include "CGameComponent.h"
#include "CGameObject.h"

class CSpawnPointComponent;

class CSpawnPointMgrComponent: public IObjectComponent
{
protected:
	std::vector<CSpawnPointComponent*>	m_listSpawnPoint;

public:
	CSpawnPointMgrComponent(CGameObject* obj);
	virtual ~CSpawnPointMgrComponent();

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

	// addSpawnPoint
	// add spawn point to list
	inline void addSpawnPoint(CSpawnPointComponent *spawnPoint)
	{
		m_listSpawnPoint.push_back(spawnPoint);
	}

	// getRandomPosition
	// random a pos from list spawnPoint
	core::vector3df getRandomPosition(int teamID = -1);
};




class CSpawnPointComponent: public IObjectComponent
{
protected:
	int		m_teamID;
public:
	CSpawnPointComponent(CGameObject* obj);
	virtual ~CSpawnPointComponent();
    
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

	// getTeamID
	// get team id
	inline int getTeamID()
	{
		return m_teamID;
	}
};

#endif