#ifndef _CANIMOBJECT_H
#define _CANIMOBJECT_H

#include "CGameObject.h"
#include "IObjectComponent.h"
#include "CGameAnimatedMeshSceneNode.h"

const int k_linear = 0;

struct SAnimFrame
{
	float		m_time;

	float		m_rotX;
	float		m_rotY;
	float		m_rotZ;
	float		m_rotAngle;
	
	float		m_translateX;
	float		m_translateY;
	float		m_translateZ;

	int			m_interpolation;
};

typedef vector<SAnimFrame>		ArrayAnimationFrame;



struct SAnimClip
{
	std::string			m_id;
	float				m_time;
	float				m_duration;
	bool				m_loop;	
};

typedef map<string, SAnimClip>				AnimationData;
typedef map<string, ArrayAnimationFrame>	JointAnimation;

class CAnimMeshComponent: public IObjectComponent
{
public:
	
protected:
	CGameAnimatedMeshSceneNode*	m_animNode;

	std::string					m_animeshFile;
	std::string					m_animFile;

	float						m_animSpeed;

	AnimationData				m_animationData;
	vector<string>				m_animationName;
	JointAnimation				m_jointAnimation;

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

	// loadAnimFile
	// load animation bone from dae file
	void loadAnimFile( char *lpFileName );

protected:
	
	// parseAnimationNode
	// parse anim node
	void parseAnimationNode( io::IXMLReader *xmlRead );

public:
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