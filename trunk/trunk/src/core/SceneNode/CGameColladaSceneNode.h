#ifndef _CGAME_COLLADA_SCENE_NODE_
#define _CGAME_COLLADA_SCENE_NODE_

#include "stdafx.h"

class CGameColladaSceneNode;
class CColladaMeshComponent;

class CGameColladaMesh: public IMesh
{
public:
	//! constructor
	CGameColladaMesh()
	{
		#ifdef _DEBUG
		setDebugName("CGameColladaMesh");
		#endif
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

	// ! For Skinning
	struct SWeight
	{	
		u16 buffer_id;
		u32 vertex_id;
		f32 strength;
		core::vector3df staticPos;
		core::vector3df staticNormal;
	};

	struct SJoint
	{
		std::wstring			name;
		core::array<SWeight>	weights;

		core::matrix4			globalInversedMatrix;
		core::matrix4			skinningMatrix;

		CGameColladaSceneNode*	node;		
	};
	
	core::array<SJoint>			Joints;
	core::array<s32>			JointIndex;
	core::array<s32>			JointVertexIndex;

	bool						IsStaticMesh;
};


class CGameColladaSceneNode: public ISceneNode
{
protected:
	core::aabbox3d<f32> Box;	
	
	SMaterial Material;

	core::matrix4	AnimationMatrix;
	core::matrix4	AbsoluteAnimationMatrix;

	core::matrix4	LocalMatrix;	

	CGameColladaMesh	*ColladaMesh;

	bool			m_isRootColladaNode;

	std::string		m_sid;

	CColladaMeshComponent		*m_component;

public:
	CGameColladaSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);

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

	// clone
	// ISceneNode override implement 
	virtual ISceneNode* clone(ISceneNode* newParent=0, ISceneManager* newManager=0);

	// setColladaMesh
	// Set current collada mesh
	void setColladaMesh(CGameColladaMesh* mesh)
	{
		if ( ColladaMesh )
			ColladaMesh->drop();

		ColladaMesh = mesh;

		if ( ColladaMesh )
			ColladaMesh->grab();
	}

	CGameColladaMesh* getMesh()
	{
		return ColladaMesh;
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

	// setComponent
	// set component
	void setComponent( CColladaMeshComponent *comp )
	{
		m_component = comp;
	}

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
		
	core::array<SPositionKey>	PositionKeys;
	core::array<SScaleKey>		ScaleKeys;
	core::array<SRotationKey>	RotationKeys;

	// end animation

	// clearAllKeyFrame
	// clear all animation key frame
	void clearAllKeyFrame()
	{
		PositionKeys.clear();
		ScaleKeys.clear();
		RotationKeys.clear();

		m_posHint = 0;
		m_scaleHint = 0;
		m_rotHint = 0;
	}

protected:
	float	m_currentFrame;
	float	m_totalFrame;

	float	m_framePerSecond;

	u32		m_timer;

	s32		m_posHint;
	s32		m_scaleHint;
	s32		m_rotHint;
	
protected:
	
	// updateAnimation
	// calc relative matrix of animation
	void	updateAnimation();
	
	// getFrameData
	// get position, scale, rotation data
	void getFrameData(f32 frame,
				core::vector3df &position,	s32 &positionHint,
				core::vector3df &scale,		s32 &scaleHint,
				core::quaternion &rotation, s32 &rotationHint);

	// skin
	// skin mesh
	void skin();
public:
	
	// getCurrentFrame
	// get time of current animation
	inline float getCurrentFrame()			{ return m_currentFrame; }	

	// getTotalFrame
	// return animation total time
	inline float getTotalFrame()			{ return m_totalFrame; }

	// setFPS
	// set animation speed
	inline void setFPS( float f )			{ m_framePerSecond = f; }
	inline float getFPS()					{ return m_framePerSecond; }

	inline void setRootColladaNode( bool b )	{ m_isRootColladaNode = b; }
	inline bool isRootColladaNode()				{ return m_isRootColladaNode; }
};

#endif