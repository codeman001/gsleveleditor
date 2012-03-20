#include "stdafx.h"
#include "CSkyboxComponent.h"

CSkyboxComponent::CSkyboxComponent( CGameObject *pObj )
	:IObjectComponent( pObj, IObjectComponent::Skybox )
{
}

CSkyboxComponent::~CSkyboxComponent()
{
}

// init
// run when init object
void CSkyboxComponent::initComponent()
{
}

// updateComponent
// update object by frame
void CSkyboxComponent::updateComponent()
{
}
	
// saveData
// save data to serializable
void CSkyboxComponent::saveData( CSerializable* pObj )
{
}

// loadData
// load data to serializable
void CSkyboxComponent::loadData( CSerializable* pObj )
{
}