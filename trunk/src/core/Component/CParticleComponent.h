#ifndef _CPARTICLECOMPONENT_
#define _CPARTICLECOMPONENT_

#include "IObjectComponent.h"
#include "CGameObject.h"

class CParticleComponent: public IObjectComponent
{
protected:
	vector<IParticleSystemSceneNode*>	m_arrayParticle;

public:
	CParticleComponent(CGameObject *pObj);

	virtual ~CParticleComponent();	
	
	// init
	// run when init object
	virtual void initComponent();

	// update
	// run when update per frame
	virtual void updateComponent();

	// saveData
	// save data to serializable
	virtual void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	virtual void loadData( CSerializable* pObj );

	// initParticle
	// create empty particle
	void initParticle();

	// createParticle
	// create an empty particle
	IParticleSystemSceneNode* createParticle();

	// getParticle
	// get a particle
	inline IParticleSystemSceneNode* getParticle( int i )
	{
		if ( i >= (int)m_arrayParticle.size() || i < 0 )
			return NULL;

		return m_arrayParticle[i];
	}

	// removeParticle
	// remove particle at i
	void removeParticle( int i );
};

#endif