#include "stdafx.h"
#include "IObjectComponent.h"
#include "CGameObject.h"

/*
Null = 0,
AnimMesh,
Mesh,
Health,
Collision,
AINpcComponent,
NpcComponent,
UnknownType
*/

const char *IObjectComponent::s_compType[] = 
{
	"Null",
	"Anim mesh component",
	"Static mesh component",
	"Health component",
	"Collision component",	
	"AI NPC component",
	"NPC component",
	"Unknown component"
};

IObjectComponent::IObjectComponent(CGameObject *pObj, int compID)
{
	m_componentID	= compID;
	m_gameObject	= pObj;
}

IObjectComponent::~IObjectComponent()
{
}
