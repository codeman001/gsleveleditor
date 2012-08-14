#include "stdafx.h"
#include "CLightingComponent.h"

CLightingComponent::CLightingComponent(CGameObject *pObj)
	:IObjectComponent( pObj, IObjectComponent::Lighting )
{
}

CLightingComponent::~CLightingComponent()
{
}

// init
// run when init object
void CLightingComponent::initComponent()
{
}

// update
// run when update per frame
void CLightingComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CLightingComponent::saveData( CSerializable* pObj )
{
}

// loadData
// load data to serializable
void CLightingComponent::loadData( CSerializable* pObj )
{
}