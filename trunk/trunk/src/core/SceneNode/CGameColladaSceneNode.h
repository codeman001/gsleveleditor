#ifndef _CGAME_COLLADA_SCENE_NODE_
#define _CGAME_COLLADA_SCENE_NODE_

#include "stdafx.h"

class CGameColladaSceneNode: public ISceneNode
{
protected:
	core::aabbox3d<f32> Box;	
	
	SMaterial Material;

	core::matrix4	AnimationMatrix;

	core::matrix4	LocalMatrix;
	core::matrix4	GlobalInversedMatrix;
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

public:
	
	// getMaterialCount
	// get num material of mesh
	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	// getMaterial
	// get material of mesh
	virtual video::SMaterial& getMaterial(u32 i)
	{
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

	void setGlobalInversedMatrix( const core::matrix4& mat )
	{
		GlobalInversedMatrix = mat;
	}
	
	const core::matrix4& getGlobalInversedMatrix()
	{
		return GlobalInversedMatrix;
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

	bool	m_animationLoop;
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
public:
	
	// getCurrentFrame
	// get time of current animation
	inline float getCurrentFrame()			{ return m_currentFrame; }

	// setAnimationLoop
	// isloop property
	inline void setAnimationLoop( bool b )	{ m_animationLoop = b; }
	inline bool isAnimationLoop()			{ return m_animationLoop; }

	// getTotalFrame
	// return animation total time
	inline float getTotalFrame()			{ return m_totalFrame; }

	// setFPS
	// set animation speed
	inline void setFPS( float f )			{ m_framePerSecond = f; }
	inline float getFPS()					{ return m_framePerSecond; }
};

#endif