#include "stdafx.h"
#include "CParticleComponent.h"
#include "IView.h"

CParticleComponent::CParticleComponent(CGameObject *pObj)
	:IObjectComponent(pObj, IObjectComponent::Particle)
{
}

CParticleComponent::~CParticleComponent()
{
}
	
// init
// run when init object
void CParticleComponent::initComponent()
{
}

// update
// run when update per frame
void CParticleComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CParticleComponent::saveData( CSerializable* pObj )
{
}

// loadData
// load data to serializable
void CParticleComponent::loadData( CSerializable* pObj )
{
	initParticle();
}

// initParticle
// create empty particle
void CParticleComponent::initParticle()
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	// create an empty node
	m_gameObject->m_node = smgr->addEmptySceneNode( smgr->getRootSceneNode() );
}

// createParticle
// create an empty particle
IParticleSystemSceneNode* CParticleComponent::createParticle()
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	// create an empty particle
	IParticleSystemSceneNode *ps = smgr->addParticleSystemSceneNode( false, m_gameObject->m_node );

	m_arrayParticle.push_back( ps );
	return ps;
}

// removeParticle
// remove particle at i
void CParticleComponent::removeParticle( int i )
{
	IParticleSystemSceneNode* ps = getParticle(i);
	if ( ps )	
		ps->remove();
}