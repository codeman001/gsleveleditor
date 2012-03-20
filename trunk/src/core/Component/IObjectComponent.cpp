#include "stdafx.h"
#include "IObjectComponent.h"
#include "CGameObject.h"

const char *IObjectComponent::s_compType[] = 
{
	"Anim mesh component",
	"Static mesh component",
	"Health component",
	"Collision component",	
	"AI NPC component",
	"NPC component",
};

IObjectComponent::IObjectComponent(CGameObject *pObj, int compID)
{
	m_componentID	= compID;
	m_gameObject	= pObj;
}

IObjectComponent::~IObjectComponent()
{
}
