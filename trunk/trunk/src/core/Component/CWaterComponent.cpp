#include "stdafx.h"
#include "CWaterComponent.h"

CWaterComponent::CWaterComponent( CGameObject *pObj )
	:IObjectComponent( pObj, (int)IObjectComponent::Water )
{
	m_waterNode = NULL;
}

CWaterComponent::~CWaterComponent()
{
}

// init
// run when init object
void CWaterComponent::initComponent()
{
}

// updateComponent
// update object by frame
void CWaterComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CWaterComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );
	
}

// loadData
// load data to serializable
void CWaterComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	initWaterNode();
}

// initGrassNode
// init grass
void CWaterComponent::initWaterNode()
{

}