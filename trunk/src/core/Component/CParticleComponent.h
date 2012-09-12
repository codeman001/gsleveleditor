#ifndef _CPARTICLECOMPONENT_
#define _CPARTICLECOMPONENT_

#include "IObjectComponent.h"
#include "CGameObject.h"

typedef struct tagParticleInfo
{
	IParticleSystemSceneNode*	ps;
	std::string					texture;
	bool						additiveTrans;
	long						startTime;
	long						lifeTime;	
	int							minParticle;
	int							maxParticle;
	bool						isStop;

	tagParticleInfo()
	{
		ps = NULL;
		texture = "";
		additiveTrans = true;
		startTime = 0;
		lifeTime = -1;
		isStop = false;
		minParticle = 1;
		maxParticle = 1;
	}
}SParticleInfo;

class CParticleComponent: public IObjectComponent
{
protected:
	std::vector<SParticleInfo>	m_arrayParticle;
	std::string				m_xmlPath;
	
	unsigned long			m_time;
	unsigned long			m_totalLifeTime;

	bool					m_stopEmitter;
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

		return m_arrayParticle[i].ps;
	}

	// getParticleTexture
	// get texture of particle
	inline std::string getParticleTexture( int i )
	{
		if ( i >= (int)m_arrayParticle.size() || i < 0 )
			return "";

		return m_arrayParticle[i].texture;
	}

	// getParticleInfo
	// get particle info
	SParticleInfo* getParticleInfo( IParticleSystemSceneNode *ps );	

	// getParticleInfo
	// get the id of particle
	int getParticleID( IParticleSystemSceneNode *ps );	

	// removeParticle
	// remove particle at i
	void removeParticle( int i );

	// removeAllParticle
	// remove all particle
	void removeAllParticle();

	// saveXML
	// save list particle to xml file
	void saveXML( const char *lpFileName );

	// loadXML
	// load particle from xml file
	void loadXML( const char *lpFileName );

	// stopParticle
	// stop particle emitter
	void stopParticle();

	// startParticle
	// begin particle emitter
	void startParticle();

	// fixParticlePosition
	// fix affector rotation & affector attraction
	void fixParticlePosition();
};

#endif