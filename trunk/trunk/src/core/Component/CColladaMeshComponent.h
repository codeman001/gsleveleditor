#ifndef CCOLLADA_MESH_COMPONENT
#define CCOLLADA_MESH_COMPONENT 

#include "CGameObject.h"
#include "IObjectComponent.h"

#include "CGameColladaSceneNode.h"

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
	
	float		m_translateX;
	float		m_translateY;
	float		m_translateZ;

	int			m_interpolation;
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

	STrianglesParam()
	{		
		VerticesIndex = -1;
		EffectIndex = -1;
		NumPolygon = 0;
		IndexBuffer = NULL;
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
	ISceneNode				*SceneNode;
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
	ISceneNode									*m_colladaNode;
	map<std::string, CGameColladaSceneNode*>	m_mapNode;

public:
	CColladaMeshComponent( CGameObject *pObj );

	virtual ~CColladaMeshComponent();

	// loadFromFile
	// load anim object from dae file
	void loadFromFile( char *lpFilename );

	// loadAnimFile
	// load animation bone from dae file
	void loadAnimFile( char *lpFileName );

protected:
	// constructScene
	// create scene node
	void constructScene();

	// constructMeshBuffer
	// create mesh buffer
	void constructMeshBuffer( SMeshParam *mesh,	STrianglesParam* tri, IMeshBuffer *buffer, bool flip );

	// constructSkinMesh
	// apply bone to vertex
	void constructSkinMesh( SMeshParam *meshParam, ISkinnedMesh *mesh );

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
	bool getFrameAtTime( AnimationFrames* frames, float time, int *frameID, core::quaternion *rotateData, core::vector3df *translateData );

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

};

#endif
