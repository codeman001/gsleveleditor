#ifndef _DAE_UTILS_H_
#define _DAE_UTILS_H_

#include "stdafx.h"
#include "CGameObject.h"
#include "CGameColladaSceneNode.h"
#include "CGameContainerSceneNode.h"

class CColladaMeshComponent;
class CColladaAnimation;

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

	core::matrix4									DefaultMatrix;
	
	core::quaternion								DefaultRot;
	core::vector3df									DefaultPos;
	bool											HasDefaultRot;
	bool											HasDefaultPos;
};

// SColladaAnim
// store animation info with array scenenode
struct SColladaAnimClip
{
	std::string	animName;
	float		time;
	float		duration;
	bool		loop;

	std::vector<SColladaNodeAnim*>					animInfo;
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
		return (int)animInfo.size();
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
	float getRealTimeLength( float baseFps = 30.0f )
	{
		return duration*1000.0f/baseFps;
	}
};
#pragma endregion

struct SColladaMeshVertexMap
{
	SMeshParam	*meshId;
	IMeshBuffer	*meshBuffer;
	s32			vertexId;

	bool operator<( const SColladaMeshVertexMap& another) const
	{
		if ( meshId == another.meshId )
		{
			if ( meshBuffer == another.meshBuffer)
			{
				if ( vertexId == another.vertexId )
					return false;
				else
					return vertexId < another.vertexId;
			}
			else
				return meshBuffer < another.meshBuffer;
		}
		else
		{
			return meshId < another.meshId;
		}
	}
};

struct SColladaVertexIndex
{
	s32	vertexId;
	s32 normalId;
	s32 texcoordId;

	bool operator<( const SColladaVertexIndex& another) const
	{
		if ( vertexId == another.vertexId )
		{
			if (normalId == another.normalId)
			{
				if ( texcoordId == another.texcoordId )
					return false;
				else
					return texcoordId < another.texcoordId;
			}
			else
				return normalId < another.normalId;
		}
		else
		{
			return vertexId < another.vertexId;
		}
	}
};

class CDaeUtils: public uiSingleton<CDaeUtils>
{
protected:
	CColladaMeshComponent*		m_component;
	
	CGameObject					*m_gameObject;
	CColladaAnimation			*m_animation;

	std::string					m_animeshFile;
	std::string					m_animFileName;

	SNodeParam					*m_boneRoot;

#pragma region DAE_PARSE_OBJECTS
	//	list of effect in collada scene
	ArrayImages					m_listImages;
	ArrayEffects				m_listEffects;
	ArrayEffects				m_listMaterial;
	ArrayEffectParams			m_listEffectsParam;
	ArrayMeshParams				m_listMesh;
	ArrayNodeParams				m_listNode;

	bool						m_needFlip;

    typedef std::vector<s32>    ArrayShort;
    
	// add for map vertex from collada to mesh buffer
	std::map<SColladaVertexIndex, s32>					m_vertexMap;
	std::map<SColladaMeshVertexMap, ArrayShort>	m_meshVertexIndex;
#pragma endregion
	

#pragma region DAE_ANIMATION_PARSE_OBJECTS
	SColladaAnimClip		m_globalClip;

	std::vector<SColladaAnimClip*>				*m_colladaAnim;
	std::map<std::string, SColladaAnimClip*>	*m_animWithName;
#pragma endregion

public:
	CDaeUtils();
	virtual ~CDaeUtils();
	
	void setCurrentComponent( CColladaMeshComponent *comp );	

	void loadFile( const char *lpFilename, CGameObject* obj );
	void loadDaeAnim( const char *lpFileName, CColladaAnimation	*collada);

protected:

#pragma region DAE_PARSE_FUNCTION
	// constructScene
	// create scene node
	void constructScene();

	// constructMeshBuffer
	// create mesh buffer
	void constructMeshBuffer( SMeshParam *mesh,	STrianglesParam* tri, IMeshBuffer *buffer, bool flip );
	void constructSkinMeshBuffer( SMeshParam *mesh,	STrianglesParam* tri, IMeshBuffer *buffer, bool flip );	

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

#pragma region DAE_ANIMATION_PARSE_FUNCTION
	// getRotationFrameID
	// get a rotation frame id at time
	bool getRotationFrameID( SColladaNodeAnim* frames, float frame, int *frameRotID, core::quaternion *rotateData );
	bool getPositionFrameID( SColladaNodeAnim* frames, float frame, int *framePosID, core::vector3df  *positionData );
	bool getScaleFrameID( SColladaNodeAnim* frames, float frame, int *frameScaleID, core::vector3df  *scaleData );


	// parseAnimationNode
	// parse anim node
	void parseAnimationNode( io::IXMLReader *xmlRead );

	// parseDefaultValue
	// parse default
	void parseDefaultValueRotate( io::IXMLReader *xmlRead, float *x, float *y, float *z, float *angle );
	void parseDefaultValuePosition( io::IXMLReader *xmlRead, float *x, float *y, float *z );

	// parseClipNode
	// parse clip time node
	void parseClipNode( io::IXMLReader *xmlRead );

	// clippingDaeAnim
	// clip a long clip to many clip
	void clipDaeAnim();

	core::matrix4 getDefaultAnimMatrix( const std::string& nodeName );
#pragma endregion

};

#endif