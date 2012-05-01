#ifndef _GAMECONTAINER_SCENENODE_
#define _GAMECONTAINER_SCENENODE_

#include "ISceneNode.h"

class CGameObject;

class CGameContainerSceneNode: public ISceneNode
{
protected:
	CGameObject *m_owner;

public:
	CGameContainerSceneNode(
			CGameObject *owner,
			ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)
			);
	virtual ~CGameContainerSceneNode();

	//! returns the axis aligned bounding box of this node
	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	//! This method is called just before the rendering process of the whole scene.
	virtual void OnRegisterSceneNode();

	//! does nothing.
	virtual void render();

	//! Returns type of the scene node
	virtual ESCENE_NODE_TYPE getType() const { return ESNT_EMPTY; }

	//! Creates a clone of this scene node and its children.
	virtual ISceneNode* clone(ISceneNode* newParent=0, ISceneManager* newManager=0)
	{
		return NULL;
	}

protected:

	core::aabbox3d<f32> Box;
};


class CGameChildContainerSceneNode: public CGameContainerSceneNode
{
protected:
	vector<ISceneNode*>		m_boundingBoxOfChild;

public:
	CGameChildContainerSceneNode(
			CGameObject *owner,
			ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)
			);

	virtual ~CGameChildContainerSceneNode();

	//! This method is called just before the rendering process of the whole scene.
	virtual void OnRegisterSceneNode();

	void addBoundingBoxOfChild( ISceneNode *p )
	{
		if ( std::find( m_boundingBoxOfChild.begin(), m_boundingBoxOfChild.end(), p ) == m_boundingBoxOfChild.end() )
		{
			p->grab();
			m_boundingBoxOfChild.push_back( p );
		}
	}

	virtual void render();
};

#endif