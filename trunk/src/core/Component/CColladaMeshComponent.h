#ifndef CCOLLADA_MESH_COMPONENT
#define CCOLLADA_MESH_COMPONENT 

#include "CGameObject.h"
#include "IObjectComponent.h"

#include "CGameColladaSceneNode.h"
#include "CGameContainerSceneNode.h"

#include "CBinaryUtils.h"

// const for animation
const int k_linear = 0;

// const for buffer
const int k_positionBuffer = 0;
const int k_normalBuffer = 1;
const int k_texCoordBuffer = 2;

// const for mesh
const int k_mesh = 0;
const int k_skinMesh = 1;

struct SAnimFrame
{
	float		m_time;

	float		m_rotX;
	float		m_rotY;
	float		m_rotZ;
	float		m_rotAngle;
	
	bool		m_haveTranslate;

	float		m_translateX;
	float		m_translateY;
	float		m_translateZ;

	int			m_interpolation;

	SAnimFrame()
	{
		m_time = 0.0f;

		m_rotX = 0.0f;
		m_rotY = 0.0f;
		m_rotZ = 0.0f;
		m_rotAngle = 0.0f;
	
		m_haveTranslate = false;

		m_translateX = 0.0f;
		m_translateY = 0.0f;
		m_translateZ = 0.0f;

		m_interpolation =0;
	}
};

typedef vector<SAnimFrame>	AnimationFrames;



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

typedef map<string, SAnimClip>				ClipAnimation;
typedef map<string, AnimationFrames>		JointAnimation;


struct SEffect
{
	video::SMaterial	Mat;
	std::wstring		Id;
	f32					Transparency;

	inline bool operator< (const SEffect & other) const
	{
		return Id < other.Id;
	}
};
typedef vector<SEffect> ArrayEffects;


struct SEffectParam
{
	std::wstring		Name;
	std::wstring		Source;
	std::wstring		InitFromTexture;
};
typedef vector<SEffectParam> ArrayEffectParams;


struct SBufferParam
{
	std::wstring		Name;
	float				*FloatArray;
	int					ArrayCount;
	int					Type;
	int					Strike;

	SBufferParam()
	{
		FloatArray = NULL;
	}
};

struct SVerticesParam
{
	std::wstring		Name;
	int					PositionIndex;
	int					NormalIndex;
	int					TexCoord1Index;
	int					TexCoord2Index;

	SVerticesParam()
	{
		PositionIndex	= -1;
		NormalIndex		= -1;
		TexCoord1Index	= -1;
		TexCoord2Index	= -1;
	}
};

struct STrianglesParam
{
	int					VerticesIndex;
	int					EffectIndex;
	int					NumPolygon;
	s32					*IndexBuffer;
	int					NumElementPerVertex;	
	int					OffsetVertex;
	int					OffsetNormal;
	int					OffsetTexcoord1;
	int					OffsetTexcoord2;

	STrianglesParam()
	{		
		VerticesIndex = -1;
		EffectIndex = -1;
		NumPolygon = 0;
		IndexBuffer = NULL;

		NumElementPerVertex = 1;
		OffsetVertex = 0;
		OffsetNormal = 0;
		OffsetTexcoord1 = 0;
		OffsetTexcoord2 = 0;
	}
};


struct SWeightParam
{
	u32 VertexID;		
	f32 Strength;
};

struct SJointParam
{
	std::wstring			Name;
	core::matrix4			InvMatrix;
	vector<SWeightParam>	Weights;	
};


struct SMeshParam
{
	std::wstring			Name;
	std::wstring			ControllerName;
	vector<SBufferParam>	Buffers;
	vector<SVerticesParam>	Vertices;
	vector<STrianglesParam>	Triangles;
	vector<SJointParam>		Joints;
	
	vector<s32>				JointVertexIndex;
	vector<s32>				JointIndex;

	int						Type;
	core::matrix4			BindShapeMatrix;
};
typedef vector<SMeshParam>	ArrayMeshParams;


struct SNodeParam
{
	std::wstring			Name;
	std::wstring			Type;
	std::wstring			SID;
	std::wstring			Instance;
	core::matrix4			Transform;
	vector<SNodeParam*>		Childs;
	SNodeParam*				Parent;

	ISkinnedMesh::SJoint	*Joint;
	CGameColladaSceneNode	*SceneNode;
	int						ChildLevel;

	SNodeParam()
	{
		Joint = NULL;
		SceneNode = NULL;
		ChildLevel = 0;
	}
};
typedef vector<SNodeParam*>	ArrayNodeParams;

class CColladaMeshComponent: public IObjectComponent
{
protected:
	std::string					m_animeshFile;	
	std::string					m_animFile;
	std::string					m_defaultNode;
	float						m_animSpeed;

	// map animation name & animation frame data 
	ClipAnimation				m_clipAnimation;

	// list of animation anme
	vector<string>				m_animationName;

	// map joint name & all frame data 
	JointAnimation				m_jointAnimation;

	// current anim is playing
	SAnimClip					*m_currentAnim;

	//	list of effect in collada scene
	ArrayEffects				m_listEffects;
	ArrayEffectParams			m_listEffectsParam;
	ArrayMeshParams				m_listMesh;
	ArrayNodeParams				m_listNode;

	// current node
	CGameChildContainerSceneNode				*m_colladaNode;

	map<std::string, CGameColladaSceneNode*>	m_mapNode;
	map<std::string, CGameColladaSceneNode*>	m_sidNode;

	bool						m_needFlip;

	float						m_currentFrame;
	float						m_animFrames;	
	float						m_pauseAnimFrame;
	bool						m_pauseAnim;

public:
	CColladaMeshComponent( CGameObject *pObj );

	virtual ~CColladaMeshComponent();

	// loadFromFile
	// load anim object from dae file
	void loadFromFile( char *lpFilename );
		
	void loadDae( char *lpFileName );
	void loadScene( char *lpFileName );

	// loadAnimFile
	// load animation bone from dae file
	void loadAnimFile( char *lpFileName );
	
	// initFromNode
	// init cache from node
	void initFromNode( CGameChildContainerSceneNode* node );

	// getSceneNode
	// find child node with name
	CGameColladaSceneNode* getSceneNode( const char *name )
	{
		return m_mapNode[ std::string(name) ];
	}

	// getSceneNodeBySID
	// find child node with name
	CGameColladaSceneNode* getSceneNodeBySID( const char *name )
	{
		return m_sidNode[ std::string(name) ];
	}
	
	// getDefaultNode
	// get default node
	CGameColladaSceneNode* getDefaultNode()
	{
		return m_mapNode[ m_defaultNode ];
	}
	
	// getColladaNode
	// get root of collada node
	CGameChildContainerSceneNode* getColladaNode()
	{
		return m_colladaNode;
	}

	// registerName
	// register name
	void registerName( const std::string& name, CGameColladaSceneNode *node )
	{
		m_mapNode[name] = node;
	
	}

	// registerName
	// register sid name
	void registerSID( const std::string& name, CGameColladaSceneNode *node )
	{
		m_sidNode[name] = node;
	}

protected:
	// constructScene
	// create scene node
	void constructScene();

	// constructMeshBuffer
	// create mesh buffer
	void constructMeshBuffer( SMeshParam *mesh,	STrianglesParam* tri, IMeshBuffer *buffer, bool flip );

	// constructSkinMesh
	// apply bone to vertex
	void constructSkinMesh( SMeshParam *meshParam, CGameColladaMesh *mesh );

	// parseEffectNode
	// parse effect material node
	void parseEffectNode( io::IXMLReader *xmlRead, SEffect* effect = NULL );
	
	// parseGeometryNode
	// parse mesh data
	void parseGeometryNode( io::IXMLReader *xmlRead );

	// parseSkinNode
	// parse skin data
	SMeshParam* parseSkinNode( io::IXMLReader *xmlRead );

	// parseControllersNode
	// parse controllser
	void parseControllerNode( io::IXMLReader *xmlRead );

	// parseSceneNode
	// parse scene data
	void parseSceneNode( io::IXMLReader *xmlRead );

	// parseNode
	// parse <node> element
	SNodeParam* parseNode( io::IXMLReader *xmlRead, SNodeParam* parent );

	// parseAnimationNode
	// parse anim node
	void parseAnimationNode( io::IXMLReader *xmlRead );

	// parseClipNode
	// parse clip time node
	void parseClipNode( io::IXMLReader *xmlRead );

	// getFrameAtTime
	// get a frame at time
	bool getFrameAtTime( const core::matrix4& mat, AnimationFrames* frames, float time, int *frameID, core::quaternion *rotateData, core::vector3df *translateData );

	// updateJointToMesh
	// update joint
	void updateJointToMesh( SMeshParam *mesh, vector<wstring>& arrayName, float *arrayWeight, float *arrayTransform, vector<s32>& vCountArray, vector<s32>& vArray, bool flipZ );

	// setAnim
	void setAnim(const char *lpAnimName, IAnimatedMeshSceneNode *node);

	// cleanData
	// free all data from parse dae
	void cleanData();
public:
	// setAnimation
	// apply Animation to skin joint
	void setAnimation(const char *lpAnimName);

	// getCurrentAnim
	// get current anim
	inline const SAnimClip* getCurrentAnim()
	{
		return m_currentAnim;
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

	// getCurrentAnimFrames
	// get number of frames of anim
	inline float getCurrentAnimFrames()
	{
		return m_animFrames;
	}

	// pauseAtFrame
	// pause anim at frame id
	inline void pauseAtFrame( float frame )
	{
		m_pauseAnim = true;
		m_pauseAnimFrame = frame;
	}

	// getCurrentFrame
	// get current frame of anim
	float getCurrentFrame()
	{
		return m_currentFrame;
	}

	// setCurrentFrame	
	void setCurrentFrame(float f)
	{
		m_currentFrame = f;
	}

	// resumeAnim
	// resume animation
	inline void resumeAnim()
	{
		m_pauseAnim = false;		
	}

	// isPauseAnim
	// check anim is pause??
	inline bool isPauseAnim()
	{
		return m_pauseAnim;
	}

	// getPauseAnim
	// get frame is pause
	inline float getPauseAnim()
	{
		return m_pauseAnimFrame;
	}

	// getAnimClip	
	inline SAnimClip* getAnimClip( const char *lpAnimName )
	{
		ClipAnimation::iterator it = m_clipAnimation.find( std::string(lpAnimName) );
		if ( it == m_clipAnimation.end() )
			return NULL;

		return &it->second;
	}

	// getAnimClip	
	inline SAnimClip* getAnimClip( int animID )
	{
		const char *aninName = getAnimName(animID);
		if ( aninName == NULL )
			return NULL;

		ClipAnimation::iterator it = m_clipAnimation.find( std::string(aninName) );
		if ( it == m_clipAnimation.end() )
			return NULL;

		return &it->second;
	}

public:
	// init
	// run when init object
	virtual void initComponent();

	// updateComponent
	// update object by frame
	virtual void updateComponent();

	// saveData
	// save data to serializable
	void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	void loadData( CSerializable* pObj );

public:

	// saveSceneToBinary
	// save collada mesh info to binary file
	void saveSceneToBinary( const char *lpFileName );

	// saveAnimToBinary
	// save animation track to binary file
	void saveAnimToBinary( const char *lpFileName );

};

class CColladaCache
{
public:
	// for cache node
	static map<string, CGameChildContainerSceneNode*>	s_nodeCache;

	// cacheNode
	// cache node with name
	static void cacheNode( const std::string &name, CGameChildContainerSceneNode* node )
	{
		s_nodeCache[name] = node;
		node->grab();
	}

	// getNodeInCache
	// get node in cache
	static CGameChildContainerSceneNode* getNodeInCache( const std::string &name )
	{
		return s_nodeCache[name];
	}

	// destroyCache
	// destroy all cache
	static void freeData()
	{
		map<string, CGameChildContainerSceneNode*>::iterator i = s_nodeCache.begin(), end = s_nodeCache.end();
		while ( i != end )
		{
			CGameChildContainerSceneNode *node = (*i).second;
			if ( node != NULL )
				node->drop();
			i++;
		}
		s_nodeCache.clear();
	}
};

#endif
