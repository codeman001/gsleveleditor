#ifndef _CANIMOBJECT_H
#define _CANIMOBJECT_H

#include "CGameObject.h"
#include "IObjectComponent.h"
#include "CGameAnimatedMeshSceneNode.h"

class CAnimMeshComponent: public IObjectComponent
{
public:
	
protected:
	CGameAnimatedMeshSceneNode*	m_animNode;

	std::string					m_animeshFile;
	float						m_animSpeed;
public:
	CAnimMeshComponent( CGameObject *pObj );

	virtual ~CAnimMeshComponent();

	// init
	// run when init object
	virtual void initComponent();

	// updateComponent
	// update object by frame
	virtual void updateComponent();

	// loadFromFile
	// load anim object from file
	void loadFromFile( char *lpFilename );

	// getMeshFile
	// get current mesh file name
	const char *getMeshFile()
	{
		return m_animeshFile.c_str();
	}

	// setSpeed
	// set speed for anim (frame/sercond)
	inline void setSpeed( float speed )
	{
		if ( speed < 1 )
			speed = 1;

		if ( m_animNode )
			m_animNode->setAnimationSpeed( speed );

		m_animSpeed = speed;
	}

public:
	// saveData
	// save data to serializable
	void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	void loadData( CSerializable* pObj );
};


#endif