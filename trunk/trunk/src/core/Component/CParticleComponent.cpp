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
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );
}

// loadData
// load data to serializable
void CParticleComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
	
	initParticle();
}


// initParticle
// create empty particle
void CParticleComponent::initParticle()
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	// create an empty node
	m_gameObject->m_node = smgr->addEmptySceneNode( smgr->getRootSceneNode() );
	m_gameObject->m_node->grab();
}

// createParticle
// create an empty particle
IParticleSystemSceneNode* CParticleComponent::createParticle()
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	// create an empty particle
	IParticleSystemSceneNode *ps = smgr->addParticleSystemSceneNode( false, m_gameObject->m_node );
	
	SParticleInfo psInfo;
	psInfo.ps = ps;

	m_arrayParticle.push_back( psInfo );
	return ps;
}

// removeParticle
// remove particle at i
void CParticleComponent::removeParticle( int i )
{
	IParticleSystemSceneNode* ps = getParticle(i);
	if ( ps )	
	{
		ps->remove();
		m_arrayParticle.erase( m_arrayParticle.begin() + i );
	}
}

// getParticleInfo
// get particle info
SParticleInfo* CParticleComponent::getParticleInfo( IParticleSystemSceneNode *ps )
{
	vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
	while (i != end )
	{
		SParticleInfo& p = (*i);
		
		if ( p.ps == ps )		
			return &p;		

		i++;
	}
	return NULL;
}

// getParticleInfo
// get the id of particle
int CParticleComponent::getParticleID( IParticleSystemSceneNode *ps )
{
	int id = 0;
	vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
	while (i != end )
	{
		SParticleInfo& p = (*i);
		
		if ( p.ps == ps )		
			return id;

		id++;
		i++;
	}
	return -1;
}