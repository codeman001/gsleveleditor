#ifndef _CGAME_COLLADA_SCENE_NODE_
#define _CGAME_COLLADA_SCENE_NODE_

#include "stdafx.h"

#define MAX_BONEMATRIX	58
#define MAX_LIGHTS		2
#define MAX_POINTLIGHTS	4

#define MAX_ANIMLAYER	4
#define MAX_ANIMTRACK   10

class CGameColladaSceneNode;
class CColladaMeshComponent;
class CGameObject;

// SColladaMeshBuffer
// Collada mesh buffer
struct SColladaMeshBuffer: public SMeshBuffer
{
	SColladaMeshBuffer()
	{
	}
};

struct SColladaSkinMeshBuffer: public SMeshBufferSkin
{
	SColladaSkinMeshBuffer()
	{
	}
};


// IGameAnimationCallback
// interface for modify animation data
class IGameAnimationCallback
{
public:
	virtual void _onUpdateFrameData( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int animLayer ) = 0;
	virtual void _onUpdateFrameDataChannel( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int channel, int animLayer ) = 0;
};

// CGameColladaMesh
// Collada mesh
class CGameColladaMesh: public IMesh
{
public:
	//! constructor
	CGameColladaMesh()
	{
		#ifdef _DEBUG
		setDebugName("CGameColladaMesh");
		#endif

		Component = NULL;
	}

	//! destructor
	virtual ~CGameColladaMesh()
	{
		// drop buffers
		for (u32 i=0; i<MeshBuffers.size(); ++i)
			MeshBuffers[i]->drop();
	}

	//! returns amount of mesh buffers.
	virtual u32 getMeshBufferCount() const
	{
		return MeshBuffers.size();
	}

	//! returns pointer to a mesh buffer
	virtual IMeshBuffer* getMeshBuffer(u32 nr) const
	{
		return MeshBuffers[nr];
	}

	//! returns a meshbuffer which fits a material
	/** reverse search */
	virtual IMeshBuffer* getMeshBuffer( const video::SMaterial & material) const
	{
		for (s32 i = (s32)MeshBuffers.size()-1; i >= 0; --i)
		{
			if ( material == MeshBuffers[i]->getMaterial())
				return MeshBuffers[i];
		}

		return 0;
	}

	//! returns an axis aligned bounding box
	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return BoundingBox;
	}

	//! set user axis aligned bounding box
	virtual void setBoundingBox( const core::aabbox3df& box)
	{
		BoundingBox = box;
	}

	//! recalculates the bounding box
	void recalculateBoundingBox()
	{
		if (MeshBuffers.size())
		{
			BoundingBox = MeshBuffers[0]->getBoundingBox();
			for (u32 i=1; i<MeshBuffers.size(); ++i)
				BoundingBox.addInternalBox(MeshBuffers[i]->getBoundingBox());
		}
		else
			BoundingBox.reset(0.0f, 0.0f, 0.0f);
	}

	//! adds a MeshBuffer
	void addMeshBuffer(IMeshBuffer* buf)
	{
		if (buf)
		{
			buf->grab();
			MeshBuffers.push_back(buf);
		}
	}

	// clone
	// clone this mesh
	virtual CGameColladaMesh* clone();	

	// updateJoint
	// remap joint to node
	void updateJoint();

	//! sets a flag of all contained materials to a new value
	virtual void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
	{
		for (u32 i=0; i<MeshBuffers.size(); ++i)
			MeshBuffers[i]->getMaterial().setFlag(flag, newvalue);
	}

	//! set the hardware mapping hint, for driver
	virtual void setHardwareMappingHint( E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer=EBT_VERTEX_AND_INDEX )
	{
		for (u32 i=0; i<MeshBuffers.size(); ++i)
			MeshBuffers[i]->setHardwareMappingHint(newMappingHint, buffer);
	}

	//! flags the meshbuffer as changed, reloads hardware buffers
	virtual void setDirty(E_BUFFER_TYPE buffer=EBT_VERTEX_AND_INDEX)
	{
		for (u32 i=0; i<MeshBuffers.size(); ++i)
			MeshBuffers[i]->setDirty(buffer);
	}

	// The component contain all node
	CColladaMeshComponent		*Component;

	//! The meshbuffers of this mesh
	core::array<IMeshBuffer*>	MeshBuffers;

	//! The bounding box of this mesh
	core::aabbox3d<f32>			BoundingBox;	

	//! Shape matrix from dae
	core::matrix4	BindShapeMatrix;
	core::matrix4	InvBindShapeMatrix;

	struct SJoint
	{
		std::wstring			name;
		core::matrix4			globalInversedMatrix;
		core::matrix4			skinningMatrix;

		CGameColladaSceneNode*	node;	
		SJoint()
		{
			name = L"noName";
			node = NULL;
		}
	};
	
	core::array<SJoint>			Joints;	
	bool						IsStaticMesh;
};

// CGameAnimationTrack
// An animation track
class CGameAnimationTrack
{
public:	
	// begin declare animation key frame
	struct SPositionKey
	{
		f32 frame;
		core::vector3df position;
	};

	struct SScaleKey
	{
		f32 frame;
		core::vector3df scale;
	};

	struct SRotationKey
	{
		f32 frame;
		core::quaternion rotation;
	};
	
	struct SEventKey
	{
		f32 frame;
		core::stringc	event;
	};

protected:
	
	s32			m_posHint;
	s32			m_scaleHint;
	s32			m_rotHint;
	float		m_currentFrame;
	float		m_totalFrame;
	
	float		m_framePerSecond;
	float		m_speedRatio;

	bool		m_isEnable;
	float		m_animWeight;

	bool		m_isPause;
	bool		m_isLoop;
	bool		m_endTrack;

	bool					m_isCrossAnim;
	bool					m_crossAnimationLoop;

public:
	core::array<CGameAnimationTrack::SPositionKey>	PositionKeys;
	core::array<CGameAnimationTrack::SScaleKey>		ScaleKeys;
	core::array<CGameAnimationTrack::SRotationKey>	RotationKeys;
	core::array<CGameAnimationTrack::SEventKey>		EventKeys;

	core::array<CGameAnimationTrack::SPositionKey>	CrossAnimPositionKeys;
	core::array<CGameAnimationTrack::SScaleKey>		CrossAnimScaleKeys;
	core::array<CGameAnimationTrack::SRotationKey>	CrossAnimRotationKeys;
	core::array<CGameAnimationTrack::SEventKey>		CrossAnimEventKeys;

public:
	CGameAnimationTrack();
	virtual ~CGameAnimationTrack();

	// getCurrentFrame
	// get time of current animation
	inline float getCurrentFrame()			{ return m_currentFrame; }	

	// setCurrentFrame
	// set current frame
	inline void setCurrentFrame(float f)	{ m_currentFrame = f; }

	// getTotalFrame
	// return animation total time
	inline float getTotalFrame()			{ return m_totalFrame; }

	// setFPS
	// set animation speed
	inline void setFPS( float f )			{ m_framePerSecond = f; }
	inline float getFPS()					{ return m_framePerSecond; }

	// setEnable
	// enable this animation track
	inline bool isEnable()					{ return m_isEnable; }
	inline void setEnable(bool b)			{ m_isEnable = b; }

	// setAnimWeight
	// set anim weight
	inline void setAnimWeight(float f)		{ m_animWeight = f; }
	inline float getAnimWeight()			{ return m_animWeight; }

	// setPause
	// set pause anim
	inline void setPause( bool b )			{ m_isPause = b; }
	inline bool isPause()					{ return m_isPause; }

	// setLoop
	// set loop anim
	inline void setLoop( bool b )			{ m_isLoop = b; }
	inline bool isLoop()					{ return m_isLoop; }

	// isEndTrack
	// return true currentFrame == totalFrame
	inline bool isEndTrack()				{ return m_endTrack; }

	// isCrossAnim
	// return true if is crossfade animation
	inline bool isCrossAnim()				{ return m_isCrossAnim; }

	// setSpeedRatio
	// set speed ratio of track
	inline void setSpeedRatio( float ratio ){ m_speedRatio = ratio; }
	inline float getSpeedRatio(){ return m_speedRatio; }

	// getFrameData
	// get anim at frame
	void getFrameData(f32 frame, core::vector3df &position, core::vector3df &scale, core::quaternion &rotation, const core::matrix4& localMatrix);

	// update
	// update per frame
	void update(float timeStep);

	// clearAllKeyFrame
	// clear all animation key frame
	void clearAllKeyFrame()
	{
		PositionKeys.clear();
		ScaleKeys.clear();		
		RotationKeys.clear();

		CrossAnimPositionKeys.clear();
		CrossAnimScaleKeys.clear();
		CrossAnimRotationKeys.clear();

		m_posHint = 0;
		m_scaleHint = 0;
		m_rotHint = 0;
		m_currentFrame = 0;

		m_isCrossAnim = false;
	}

	// setCrossAnimation
	// setCrossCallback
	void enableCrossAnimation(bool loop)
	{
		m_isCrossAnim			= true;
		m_crossAnimationLoop	= loop;
	}

	// crossAnimation
	// switch to main animation
	void crossAnimation();
};

// CGameAnimation
// game animation (it will management many animation track)
class CGameAnimation
{
protected:
	CGameAnimationTrack		m_animTrack[MAX_ANIMTRACK];
	bool					m_nullAnimation;
	
	bool					m_isEnable;
public:
	CGameAnimation();
	virtual ~CGameAnimation();

	// getTrack
	// get animation track
	CGameAnimationTrack* getTrack(int i)
	{
		return &m_animTrack[i];
	}

	// disableAllTrack
	// disable all track
	void disableAllTrack()
	{
		for ( int i = 0; i < MAX_ANIMTRACK; i++ )
			m_animTrack[i].setEnable( false );
	}

	// onlyEnableTrack
	// enable a track i
	void onlyEnableTrack( int track )
	{
		for ( int i = 0; i < MAX_ANIMTRACK; i++ )
		{
			if ( i == track )
				m_animTrack[i].setEnable( true );
			else
				m_animTrack[i].setEnable( false );
		}
	}

	// setAnimWeight
	// set anim weight
	void setAnimWeight( int track, float w )
	{
		m_animTrack[track].setAnimWeight( w );
	}

	// getFrameData
	// get anim at frame
	void getFrameData( core::vector3df &position, core::vector3df &scale, core::quaternion &rotation, const core::matrix4& localMatrix, IGameAnimationCallback* callback, ISceneNode *parent, int layer);

	// synchronizedTimeScale
	// sync speed of n track	
	void synchronizedByTimeScale();

	// update
	// update per frame
	void update(float timeStep);

	// setEnable
	// enable animation
	void setEnable( bool b )
	{
		m_isEnable = b;
	}

	// isNullAnimation
	// return true if not set animation
	inline bool isNullAnimation()
	{
		return m_nullAnimation;
	}

	// isEnable
	// return true if enable this animation
	inline bool isEnable()
	{
		return m_isEnable;
	}

protected:
	
	struct SAnimWeight
	{
		int		animID;
		float	animWeight;
	};

	void sortWeightTrack( SAnimWeight *arrayTrack, int &num );
};

// CGameColladaSceneNode
// Collada Node
class CGameColladaSceneNode: public ISceneNode
{
protected:
	core::aabbox3d<f32> Box;	
	
	SMaterial		Material;
	
public:
	core::matrix4	AnimationMatrix;
	core::matrix4	AbsoluteAnimationMatrix;

	core::matrix4	AnimationMatrixLayer[MAX_ANIMLAYER];
	core::matrix4	AbsoluteAnimationMatrixLayer[MAX_ANIMLAYER];
    
	core::matrix4	LocalMatrix;
	core::matrix4	GlobalInversedMatrix;
	
	CGameColladaMesh	*ColladaMesh;
	float				BoneMatrix[MAX_BONEMATRIX*16];

protected:
	bool			m_isRootColladaNode;
	bool			m_enableAnim;

	// for hardware skinning
	bool			m_isHardwareSkinning;

#ifdef GSANIMATION
	bool			m_isShowName;
#endif

	std::string		m_sid;

	CColladaMeshComponent		*m_component;

	IGameAnimationCallback		*m_animationCallback;

	int				m_animationLayer;
	bool			m_connectLayerAnim;
public:
	CGameColladaSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, bool hardwareSkinning = true);

	virtual ~CGameColladaSceneNode();

public:
	// OnAnimate
	// ISceneNode override implement 
	virtual void OnAnimate(u32 timeMs);	
	
	// OnRegisterSceneNode
	// ISceneNode override implement 
	virtual void OnRegisterSceneNode();

	// updateAbsolutePosition
	// ISceneNode override implement
	virtual void updateAbsolutePosition();

	// render
	// ISceneNode override implement 
	virtual void render();

	// getBoundingBox
	// ISceneNode override implement 
	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return Box;
	}

	// getBoundingBox
	// ISceneNode override implement 
	core::aabbox3d<f32>& getBoundingBox()
	{
		return Box;
	}

	// clone
	// ISceneNode override implement 
	virtual ISceneNode* clone(ISceneNode* newParent=0, ISceneManager* newManager=0);

	// setColladaMesh
	// Set current collada mesh
	void setColladaMesh(CGameColladaMesh* mesh);

	CGameColladaMesh* getMesh()
	{
		return ColladaMesh;
	}

	// setAnimLayer
	// Set animation from layer
	inline void setAnimLayer(int layer)
	{
		if (m_animationLayer >= 0 && m_animationLayer < MAX_ANIMLAYER)
			m_animationLayer = layer;
	}

	// setConnectAnimLayer	
	inline void setConnectAnimLayer(bool b)
	{
		m_connectLayerAnim = b;
	}

	// setSIDName
	// set sid joint name
	inline void setSIDName( const std::string& name )
	{
		m_sid = name;
	}

	// getSIDName
	// get sid joint name
	inline const std::string& getSIDName()
	{
		return m_sid;
	}

	// isEnableAnim
	// enable anim on this node
	inline bool isEnableAnim()
	{
		return m_enableAnim;
	}

	// setEnableAnim
	// enable/disable anim on this node
	inline void setEnableAnim(bool b)
	{
		m_enableAnim = b;
	}


	// enableAnimOnAllChild
	// enable/disable anim on all child
	void enableAnimOnAllChild( bool b );

	// setComponent
	// set component
	void setComponent( CColladaMeshComponent *comp )
	{
		m_component = comp;
	}

	inline void setAnimationCallback( IGameAnimationCallback *callback )
	{
		m_animationCallback = callback;
	}

	// setSkydome
	// set scenenode is skydome
	void setSkydome( bool b );	

	// setTerrainNode
	// set node is terrain collision
	inline void setTerrainNode( bool b )
	{
		m_terrainNode = b;
	}

	bool isTerrainNode()
	{
		return m_terrainNode;
	}
	
	// setHideTerrain
	// hide terrain collision node
	inline void setHideTerrain( bool b )
	{
		m_hideTerrainNode = b;
	}

	bool isHideTerrain()
	{
		return m_hideTerrainNode;
	}

#ifdef GSANIMATION
protected:
	int m_renderRotate;
	
public:
	void showName( bool b )
	{
		m_isShowName = b;

		if (b == true)
			m_renderRotate = 1|2|4;
		else
			m_renderRotate = 0;
	}	
		
	// setRenderRotateFlag
	// set flag
	void setRenderRotateFlag( int flag );

	// renderListPoint
	// draw a point to multi point
	void renderListPoint(core::vector3df *point, int nPoint, SColor color);
	
	// renderOxyz
	// draw oxyz
	void renderOxyz();

	// buildRotPoint
	// build list 
	// type = 0: rotY, 1: rotX, 2: rotZ
	void buildRotPoint( core::vector3df *point, int nPoint, float radius, int type );

	// getHitState
	// get hit on rotate
	// return -1: none
	// return 0: y
	// return 1: x
	// return 2: z
	int getHitState( int x, int y );

	// isHitOnListPoint
	// check hit on list point
	bool isHitOnListPoint(core::vector3df *point, int nPoint, int x, int y);

	// isLineHit
	// check hit on a line
	bool isLineHit( int X1, int Y1, int X2, int Y2, int X, int Y );	
#endif

public:
	
	// getMaterialCount
	// get num material of mesh
	virtual u32 getMaterialCount() const
	{
		if ( ColladaMesh )
			return ColladaMesh->getMeshBufferCount();

		return 1;
	}

	// getMaterial
	// get material of mesh
	virtual video::SMaterial& getMaterial(u32 i)
	{
		if ( ColladaMesh )
			return ColladaMesh->getMeshBuffer(i)->getMaterial();

		return Material;
	}

public:
	
	void setLocalMatrix( const core::matrix4& mat )
	{
		LocalMatrix = mat;
		AnimationMatrix = mat;
	}
	
	const core::matrix4& getLocalMatrix()
	{
		return LocalMatrix;
	}

	// updateAnimation
	// calc relative matrix of animation
	void updateAnimation();

	// getAnimation
	// get animation data
	inline CGameAnimation* getAnimation(int animLayer)
	{
		return &m_gameAnimation[animLayer];
	}

protected:	
	u32				m_timer;	

	bool			m_isSkydome;
	bool			m_terrainNode;
	bool			m_hideTerrainNode;

	// Multi layer:
	// example:
	//	- foot is a animation
	//	- hand is a animation
	CGameAnimation	m_gameAnimation[MAX_ANIMLAYER];

public:		

	// skin
	// skin mesh
	void skin();
public:
		
	inline void setRootColladaNode( bool b )	{ m_isRootColladaNode = b; }
	inline bool isRootColladaNode()				{ return m_isRootColladaNode; }
};

#endif