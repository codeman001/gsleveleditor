#include "stdafx.h"
#include "IView.h"
#include "CZone.h"
#include "CSpawnPointComponent.h"

/////////////////////////////////////////////////////////////////
// Spawn Point Manager Component
/////////////////////////////////////////////////////////////////

CSpawnPointMgrComponent::CSpawnPointMgrComponent(CGameObject* obj)
	:IObjectComponent(obj, CGameComponent::SpawnPointMgrComponent)
{
}

CSpawnPointMgrComponent::~CSpawnPointMgrComponent()
{
}

// init
// run when init object
void CSpawnPointMgrComponent::initComponent()
{
}

// update
// run when update per frame
void CSpawnPointMgrComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CSpawnPointMgrComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( CGameComponent::s_compType[ m_componentID ] );
}

// loadData
// load data to serializable
void CSpawnPointMgrComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
}

// getRandomPosition
// random a pos from list spawnPoint
void CSpawnPointMgrComponent::getRandomPosition(int teamID, core::vector3df& pos, core::vector3df &rot)
{
	core::vector3df ret;

	if ( m_listSpawnPoint.size() > 0 )
	{
		int r = getIView()->getDevice()->getRandomizer()->rand() % (int)m_listSpawnPoint.size();
		
		pos = m_listSpawnPoint[r]->getGameObject()->getPosition();
		rot = m_listSpawnPoint[r]->getGameObject()->getRotation();
	}	
}




/////////////////////////////////////////////////////////////////
// Spawn Point Component
/////////////////////////////////////////////////////////////////

CSpawnPointComponent::CSpawnPointComponent(CGameObject* obj)
	:IObjectComponent(obj, CGameComponent::SpawnPointComponent)
{
	m_teamID = 0;
}

CSpawnPointComponent::~CSpawnPointComponent()
{
}
    
// init
// run when init object
void CSpawnPointComponent::initComponent()
{
	CGameObject* pZone = (CGameObject*)m_gameObject->getParent();
	if ( pZone->getObjectType() == CGameObject::ZoneObject )
	{
		CGameObject *spawnPointMgr = ((CZone*)pZone)->getSpawnPointManager();

		CSpawnPointMgrComponent* spawnPointMgrComp = (CSpawnPointMgrComponent*)spawnPointMgr->getComponent(CGameComponent::SpawnPointMgrComponent);
		if ( spawnPointMgrComp )
		{
			spawnPointMgrComp->addSpawnPoint(this);
		}
	}
}
    
// update
// run when update per frame
void CSpawnPointComponent::updateComponent()
{
#ifdef GSGAMEPLAY
	m_gameObject->setVisible(false);
#endif
}    
// saveData
// save data to serializable
void CSpawnPointComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( CGameComponent::s_compType[ m_componentID ] );
	pObj->addInt("teamID", m_teamID);
}
    
// loadData
// load data to serializable
void CSpawnPointComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
	m_teamID = pObj->readInt();
}