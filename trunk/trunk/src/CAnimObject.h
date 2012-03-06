#ifndef _CANIMOBJECT_H
#define _CANIMOBJECT_H

#include "CGameObject.h"
#include "CGameAnimatedMeshSceneNode.h"

class CAnimObject: public CGameObject
{
public:
	
protected:
	CGameAnimatedMeshSceneNode*	m_animNode;
	string	m_animeshFile;

public:
	CAnimObject();

	virtual ~CAnimObject();

	// updateObject
	// update object by frame
	virtual void updateObject();

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
		if ( m_animNode )
			m_animNode->setAnimationSpeed( speed );
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