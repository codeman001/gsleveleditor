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
	bool				m_movePosition;

	int					m_frames;

	SAnimClip()
	{
		m_id			= "unAvailable";
		m_time			= 0;
		m_duration		= 0;
		m_loop			= true;
		m_movePosition	= true;
		m_frames		= 0;
	}
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

	// map animation name & animation frame data 
	AnimationData				m_animationData;

	// list of animation anme
	vector<string>				m_animationName;

	// map joint name & all frame data 
	JointAnimation				m_jointAnimation;


	SAnimClip					*m_currentAnim;

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

	// getFrameAtTime
	// get a frame at time
	bool getFrameAtTime( ArrayAnimationFrame* frames, float time, int *frameID, core::quaternion *rotateData, core::vector3df *translateData );
	
public:
	// getMeshFile
	// get current mesh file name
	const char *getMeshFile()
	{
		return m_animeshFile.c_str();
	}

	// setAnimation
	// apply Animation to skin joint
	void setAnimation(const char *lpAnimName);

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

	// getCurrentAnim
	// get current anim
	inline const SAnimClip* getCurrentAnim()
	{
		return m_currentAnim;
	}

	// CGameAnimatedMeshSceneNode
	// return anim node
	inline CGameAnimatedMeshSceneNode* getAnimNode()
	{
		return m_animNode;
	}

	// getAnimCount
	// return number of anim
	inline int getAnimCount()
	{
		return (int)m_animationName.size();
	}

	// getAnimName
	// return anim name id
	inline const char* getAnimName( int id )
	{
		if ( id < 0 || id >= (int)m_animationName.size() )
			return NULL;

		return m_animationName[id].c_str();
	}

	// getAnimClip	
	inline SAnimClip* getAnimClip( const char *lpAnimName )
	{
		AnimationData::iterator it = m_animationData.find( std::string(lpAnimName) );
		if ( it == m_animationData.end() )
			return NULL;

		return &it->second;
	}

	// getAnimClip	
	inline SAnimClip* getAnimClip( int animID )
	{
		const char *aninName = getAnimName(animID);
		if ( aninName == NULL )
			return NULL;

		AnimationData::iterator it = m_animationData.find( std::string(aninName) );
		if ( it == m_animationData.end() )
			return NULL;

		return &it->second;
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