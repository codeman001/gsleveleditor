#include "stdafx.h"
#include "CTerrainComponent.h"

CTerrainComponent::CTerrainComponent( CGameObject *pObj )
	:IObjectComponent( pObj, (int)IObjectComponent::Terrain )
{
}

CTerrainComponent::~CTerrainComponent()
{
}

// init
// run when init object
void CTerrainComponent::initComponent()
{
}

// updateComponent
// update object by frame
void CTerrainComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CTerrainComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );
}

// loadData
// load data to serializable
void CTerrainComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
}