#ifndef CCOLLADA_MESH_COMPONENT
#define CCOLLADA_MESH_COMPONENT 

#include "CGameObject.h"
#include "IObjectComponent.h"

#include "CGameColladaSceneNode.h"
#include "CGameContainerSceneNode.h"

#pragma region DAE_ENTITY_PARSE_DECLARE

// const for buffer
const int k_positionBuffer = 0;
const int k_normalBuffer = 1;
const int k_texCoordBuffer = 2;

// const for mesh
const int k_mesh = 0;
const int k_skinMesh = 1;

struct SImage
{
	std::wstring	id;
	std::wstring	name;
	std::wstring	fileName;
};
typedef std::vector<SImage>	ArrayImages;

struct SEffect
{
	video::SMaterial	Mat;
	std::wstring		Id;
	f32					Transparency;
	bool				HasAlpha;
	bool				TransparentAddColor;

	inline bool operator< (const SEffect & other) const
	{
		return Id < other.Id;
	}
};
typedef std::vector<SEffect> ArrayEffects;


struct SEffectParam
{
	std::wstring		Name;
	std::wstring		Source;
	std::wstring		InitFromTexture;
};
typedef std::vector<SEffectParam> ArrayEffectParams;


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
	std::vector<SWeightParam>	Weights;	
};


struct SMeshParam
{
	std::wstring			Name;
	std::wstring			ControllerName;

	std::vector<SBufferParam>	Buffers;
	std::vector<SVerticesParam>	Vertices;
	std::vector<STrianglesParam>	Triangles;
	std::vector<SJointParam>		Joints;
	
	std::vector<s32>				JointVertexIndex;
	std::vector<s32>				JointIndex;

	int						Type;
	core::matrix4			BindShapeMatrix;
};
typedef std::vector<SMeshParam>	ArrayMeshParams;


struct SNodeParam
{
	std::wstring			Name;
	std::wstring			Type;
	std::wstring			SID;
	std::wstring			Instance;
	core::matrix4			Transform;
	std::vector<SNodeParam*>		Childs;
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
typedef std::vector<SNodeParam*>	ArrayNodeParams;

#pragma endregion


#pragma region DAE_ANIMATION_ENTITY_DECLARE

// SColladaNodeAnim
// store keyframes on a scenenode
struct SColladaNodeAnim
{
	std::string	sceneNodeName;

	core::array<CGameAnimationTrack::SPositionKey>	PositionKeys;
	core::array<CGameAnimationTrack::SScaleKey>		ScaleKeys;
	core::array<CGameAnimationTrack::SRotationKey>	RotationKeys;
	core::array<CGameAnimationTrack::SEventKey>		EventKeys;
};

// SColladaAnim
// store animation info with array scenenode
struct SColladaAnimClip
{
	std::string	animName;
	float		time;
	float		duration;
	bool		loop;

	std::vector<SColladaNodeAnim*>				animInfo;
	std::map<std::string, SColladaNodeAnim*>		animNameToInfo;

	SColladaAnimClip()
	{
		animName = "";
		time = 0.0f;
		duration = 0.0f;
		loop = true;
	}

	~SColladaAnimClip()
	{
		freeAllNodeAnim();
	}

	void freeAllNodeAnim()
	{
		std::vector<SColladaNodeAnim*>::iterator i = animInfo.begin(), end = animInfo.end();
		while ( i != end )
		{
			delete (*i);
			i++;
		}
		animInfo.clear();
		animNameToInfo.clear();
	}

	void addNodeAnim( SColladaNodeAnim* anim )
	{
		std::vector<SColladaNodeAnim*>::iterator i = animInfo.begin(), end = animInfo.end();
		while ( i != end )
		{
			if ( (*i)->sceneNodeName == anim->sceneNodeName )
			{
				delete (*i);
				*i = anim;
				animNameToInfo[ (*i)->sceneNodeName ] = anim;
				return;
			}
			i++;
		}
		
		animInfo.push_back( anim );
		animNameToInfo[ anim->sceneNodeName ] = anim;

	}

	// getNodeAnimCount
	// return number node with animation
	int getNodeAnimCount()
	{
		return animInfo.size();
	}

	// getAnimOfSceneNode
	// return array frame of scene at pos i
	SColladaNodeAnim* getAnimOfSceneNode( int i )
	{
		return animInfo[i];
	}

	// getAnimOfSceneNode
	// get array frame of scenenode
	SColladaNodeAnim* getAnimOfSceneNode(const char *sceneNodeName)
	{		
		return animNameToInfo[sceneNodeName];
	}

	// getRealTimeLength
	// get time by milisecond
	float getRealTimeLength( float baseFps = 40.0f )
	{
		return duration * baseFps;
	}
};

#pragma endregion


#pragma region DAE_ANIMATION_CLASS_DECLARE

// CColladaAnimation
// collada anim
class CColladaAnimation
{
protected:
	std::string							m_animFileName;
	std::vector<SColladaAnimClip*>			m_colladaAnim;
	std::map<std::string, SColladaAnimClip*>	m_animWithName;

	// input need for dae parse
	bool					m_needFlip;
	SColladaAnimClip		m_globalClip;
	// ----------------------------------------

protected:	

	// getRotationFrameID
	// get a rotation frame id at time
	bool getRotationFrameID( SColladaNodeAnim* frames, float frame, int *frameRotID, core::quaternion *rotateData );
	bool getPositionFrameID( SColladaNodeAnim* frames, float frame, int *framePosID, core::vector3df  *positionData );

	// parseAnimationNode
	// parse anim node
	void parseAnimationNode( io::IXMLReader *xmlRead );

	// parseClipNode
	// parse clip time node
	void parseClipNode( io::IXMLReader *xmlRead );

	// clippingDaeAnim
	// clip a long clip to many clip
	void clipDaeAnim();

	void loadDae( char *lpFileName );
	void loadDotAnim( char *lpFileName );

public:
	CColladaAnimation();

	virtual ~CColladaAnimation();

	void loadFile( char *lpFileName );

	SColladaAnimClip* getAnim( const char *lpAnimName )
	{
		return m_animWithName[ lpAnimName ];
	}

	SColladaAnimClip* getAnim( int animID )
	{
		return m_colladaAnim[ animID ];
	}

	int getAnimCount()
	{
		return m_colladaAnim.size();
	}

	void addClip( SColladaAnimClip* clip )
	{
		m_colladaAnim.push_back( clip );
		m_animWithName[ clip->animName ] = clip;
	}

	void removeClip( const std::string& clipName );

	void cloneAnim( const char *lpAnimName, const char *lpNewAnimName );

	void createSynchronizedAnim( const char *lpNewName, const char *lpAnim1, const char *lpAnim2, float w );
	
};

// CColladaAnimationFactory
// animation manager
class CColladaAnimationFactory: public uiSingleton<CColladaAnimationFactory>
{
protected:
	std::map<std::string, CColladaAnimation*>	m_animPackage;
public:
	CColladaAnimationFactory();
	virtual ~CColladaAnimationFactory();

	// loadAnimation
	// load package animation
	CColladaAnimation* loadAnimation( char *name, char *lpFileName );

	// getAnimation
	// get package animation
	inline CColladaAnimation* getAnimation( char *name )
	{
		return m_animPackage[ name ];
	}

	// freeAllAnimationPackage
	// release all package animation
	void freeAllAnimationPackage();
};

#pragma endregion

// CColladaMeshComponent
class CColladaMeshComponent: public IObjectComponent
{
protected:
	std::string					m_animeshFile;	
	std::string					m_defaultNodeString;
	
	std::vector<CGameColladaSceneNode*>	m_defaultNode;
	
	// current anim is playing
	SColladaAnimClip			*m_currentAnim;

#pragma region DAE_PARSE_OBJECTS
	//	list of effect in collada scene
	ArrayImages					m_listImages;
	ArrayEffects				m_listEffects;
	ArrayEffects				m_listMaterial;
	ArrayEffectParams			m_listEffectsParam;
	ArrayMeshParams				m_listMesh;
	ArrayNodeParams				m_listNode;
#pragma endregion

	// current node
	CGameColladaContainerSceneNode				*m_colladaNode;

	// current animation clip info
	CColladaAnimation							*m_colladaAnimation;

	std::map<std::string, CGameColladaSceneNode*>	m_mapNode;
	std::map<std::string, CGameColladaSceneNode*>	m_sidNode;

	bool						m_needFlip;

	bool						m_isPauseAnim[2];
	float						m_pauseAtFrame[2];

public:
	CColladaMeshComponent( CGameObject *pObj );

	virtual ~CColladaMeshComponent();

	// loadFromFile
	// load anim object from dae file
	void loadFromFile( const char *lpFilename );
		
	void loadDae( const char *lpFileName );
	void loadScene( const char *lpFileName );

	// initFromNode
	// init cache from node
	void initFromNode( CGameColladaContainerSceneNode* node );

	// getSceneNode
	// find child node with name
	CGameColladaSceneNode* getSceneNode( const char *name )
	{
		return m_mapNode[ std::string(name) ];
	}

	// getChildsOfSceneNode
	// find all childs of scene node
	void getChildsOfSceneNode( const char *name, std::vector<CGameColladaSceneNode*>& listChilds );

	// getSceneNodeBySID
	// find child node with name
	CGameColladaSceneNode* getSceneNodeBySID( const char *name )
	{
		return m_sidNode[ std::string(name) ];
	}
		
	// getDefaultNode
	// get default node
	CGameColladaSceneNode* getDefaultNode(int i)
	{
		int numDefaultNode = m_defaultNode.size();

		if ( numDefaultNode == 0 || i >= numDefaultNode || i < 0 )
			return NULL;
		return m_defaultNode[i];
	}

	// getNumDefaultNode
	// return number of default node
	int getNumDefaultNode()
	{
		return (int)m_defaultNode.size();
	}
	
	// isDefaultNode
	// check the node is default node or not
	bool isDefaultNode( ISceneNode* node )
	{
		int numDefaultNode = m_defaultNode.size();
		for ( int i = 0; i < numDefaultNode; i++ )
		{
			if ( m_defaultNode[i] == node )
				return true;
		}
		return false;
	}

	// getColladaNode
	// get root of collada node
	CGameColladaContainerSceneNode* getColladaNode()
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

#pragma region DAE_PARSE_FUNCTION
protected:
	// constructScene
	// create scene node
	void constructScene();

	// constructMeshBuffer
	// create mesh buffer
	void constructMeshBuffer( SMeshParam *mesh,	STrianglesParam* tri, IMeshBuffer *buffer, int *beginVertex, int *endVertex ,bool flip );
	void constructSkinMeshBuffer( SMeshParam *mesh,	STrianglesParam* tri, IMeshBuffer *buffer, int *beginVertex, int *endVertex ,bool flip );

	// constructSkinMesh
	// apply bone to vertex
	void constructSkinMesh( SMeshParam *meshParam, CGameColladaMesh *mesh );

	// parseImageNode
	// parse image
	void parseImageNode( io::IXMLReader *xmlRead, SImage* image = NULL );

	// parseMaterialNode
	// parse material
	void parseMaterialNode( io::IXMLReader *xmlRead, SEffect* effect = NULL );

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
		
	// updateJointToMesh
	// update joint
	void updateJointToMesh( SMeshParam *mesh, std::vector<std::wstring>& arrayName, float *arrayWeight, float *arrayTransform, std::vector<s32>& vCountArray, std::vector<s32>& vArray, bool flipZ );	

	// cleanData
	// free all data from parse dae
	void cleanData();
#pragma endregion

public:
	
	// setAnimationPackage
	// set anim package for this mesh
	void setAnimationPackage( CColladaAnimation *colladaAnim )
	{
		m_colladaAnimation = colladaAnim;
	}

	// getCurrentAnimPackage
	// get anim package for this mesh
	CColladaAnimation* getCurrentAnimPackage()
	{
		return m_colladaAnimation;
	}

	// setCrossFadeAnimation
	// crossfade current animation to new animation
	void setCrossFadeAnimation(const char *lpAnimName, int trackChannel = 0, float nFrames = 10.0f, bool loop = true);
	void setCrossFadeAnimation(const std::string& animName, int trackChannel = 0, float nFrames = 10.0f, bool loop = true)
	{
		setCrossFadeAnimation( animName.c_str(), trackChannel, nFrames, loop );
	}

	void setCrossFadeAnimation(const char *lpAnimName, std::vector<CGameColladaSceneNode*>& listNodes, int trackChannel = 0, float nFrames = 10.0f, bool loop = true);
	void setCrossFadeAnimation(const std::string& animName, std::vector<CGameColladaSceneNode*>& listNodes, int trackChannel = 0, float nFrames = 10.0f, bool loop = true)
	{
		setCrossFadeAnimation( animName.c_str(), listNodes, trackChannel, nFrames, loop );
	}


	// setAnimation
	// apply Animation to skin joint
	void setAnimation(const char *lpAnimName,		int track = 0, bool loop = true);
	void setAnimation(const std::string& animName,	int track = 0, bool loop = true)
	{
		setAnimation( animName.c_str(), track, loop );
	}

	// setAnimation
	// apply Animation to array of skin joint
	void setAnimation(const char *lpAnimName,		std::vector<CGameColladaSceneNode*>& listNodes, int track = 0, bool loop = true );
	void setAnimation(const std::string& animName,	std::vector<CGameColladaSceneNode*>& listNodes, int track = 0, bool loop = true )
	{
		setAnimation( animName.c_str(), listNodes, track, loop );
	}

	// synchronizedByTimeScale
	// sync n animation
	void synchronizedByTimeScale();

	// getCurrentAnim
	// get current anim
	inline SColladaAnimClip* getCurrentAnim()
	{
		return m_currentAnim;
	}

	// getCurrentAnimTimeLength
	// get current anim length
	float getCurrentAnimTimeLength()
	{
		float time = 0;
		if ( m_currentAnim )
			time = time + m_currentAnim->getRealTimeLength();

		return time;
	}

	// pauseAtFrame
	// pause anim at frame id
	void pauseAtFrame( float frame, int trackChannel = 0 );
	
	// getCurrentFrame
	// get current frame of anim
	float getCurrentFrame(int trackChannel = 0);	

	// setCurrentFrame	
	void setCurrentFrame(float f, int trackChannel = 0);

	// setAnimWeight
	void setAnimWeight(float w, int trackChannel = 0);

	// setAnimSpeed
	void setAnimSpeed(float s, int trackChannel = 0);

	// resumeAnim
	// resume animation
	void resumeAnim(int trackChannel = 0);

	// enableAnimTrackChanel
	void enableAnimTrackChanel( int trackChannel, bool b);
	void onlyEnableAnimTrackChannel( int trackChannel );

	// isPauseAnim
	// check anim is pause??
	inline bool isPauseAnim(int trackChanel = 0)
	{
		return m_isPauseAnim[trackChanel];
	}

	// getPauseAnim
	// get frame is pause
	inline float getPauseAnim(int trackChanel = 0)
	{
		return m_pauseAtFrame[trackChanel];
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

public:

	//////////////////////////////////////////////
	// gameplay func
	//////////////////////////////////////////////

	struct SColladaLodNode
	{
		float			distance;
		std::string		node;
	};

	std::vector<SColladaLodNode>			m_colladaLodNode;

	// sortLodData
	// sort lod data by distance
	void sortLodData();

	// clearLodData
	// clear all lod data
	void clearLodData();	

	// addLodData
	// set lod data
	void addLodData( float distanceCam, const char* node );

	// updateLod
	// update lod mesh
	void updateLod();
};

class CColladaCache
{
public:
	// for cache node
	static std::map<std::string, CGameColladaContainerSceneNode*>	s_nodeCache;

	// cacheNode
	// cache node with name
	static void cacheNode( const std::string &name, CGameColladaContainerSceneNode* node )
	{
		s_nodeCache[name] = node;
		node->grab();
	}

	// getNodeInCache
	// get node in cache
	static CGameColladaContainerSceneNode* getNodeInCache( const std::string &name )
	{
		return s_nodeCache[name];
	}

	// destroyCache
	// destroy all cache
	static void freeData()
	{
		std::map<std::string, CGameColladaContainerSceneNode*>::iterator i = s_nodeCache.begin(), end = s_nodeCache.end();
		while ( i != end )
		{
			CGameColladaContainerSceneNode *node = (*i).second;
			if ( node != NULL )
				node->drop();
			i++;
		}
		s_nodeCache.clear();
	}
};

#endif
