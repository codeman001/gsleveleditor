#include "stdafx.h"
#include "IObjectComponent.h"
#include "CGameObject.h"

const char *IObjectComponent::s_compType[] = 
{
	"Anim mesh component",
	"Static mesh component",
	"Skybox component",
	"Object transform component",
	"Object collision component",
	"Object shadow component",
	"Box object component",
	"Billboard component",
	"Particle component",
	"Collada mesh component",
	"Terrain component",
	"Grass component",
	"Water component",
	"Lighting component",
	"Ellipsoid collision component"
};

IObjectComponent::IObjectComponent(CGameObject *pObj, int compID)
{
	m_componentID	= compID;
	m_gameObject	= pObj;
}

IObjectComponent::~IObjectComponent()
{
}
