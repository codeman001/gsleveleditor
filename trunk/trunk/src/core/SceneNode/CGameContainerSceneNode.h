#ifndef _GAMECONTAINER_SCENENODE_
#define _GAMECONTAINER_SCENENODE_

#include "ISceneNode.h"
#include "CGameColladaSceneNode.h"

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

	// setOwner
	// change owner object
	void setOwner( CGameObject *obj )
	{
		m_owner = obj;
	}

protected:

	core::aabbox3d<f32> Box;
};


class CGameColladaContainerSceneNode: public CGameContainerSceneNode
{
protected:
	std::vector<ISceneNode*>		m_boudingMeshNode;

    struct SNodeReference
    {
        ISceneNode *node1;
        ISceneNode *node2;
    };
    
    std::vector<SNodeReference>     m_nodeReference;
	bool							m_containSkinMesh;

public:
	CGameColladaContainerSceneNode(
			CGameObject *owner,
			ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)
			);

	virtual ~CGameColladaContainerSceneNode();

	//! This method is called just before the rendering process of the whole scene.
	virtual void OnRegisterSceneNode();
	
	//! Overide OnAnimate
	virtual void OnAnimate(irr::u32 timeMs);

	void addBoundingMeshNode( CGameColladaSceneNode *p )
	{
		if ( std::find( m_boudingMeshNode.begin(), m_boudingMeshNode.end(), p ) == m_boudingMeshNode.end() )
		{
			p->grab();

			// check skin mesh
			if ( p->getMesh() && p->getMesh()->IsStaticMesh == false )
				m_containSkinMesh = true;

			m_boudingMeshNode.push_back( p );
		}
	}

    // addNodeReferenceByAnimLayer
    void addNodeReferenceByAnimLayer(ISceneNode *node1, ISceneNode* node2)
    {
        if ( node1 == NULL || node2 == NULL )
            return;
        
        for ( int i = 0, n = (int)m_nodeReference.size(); i < n; i++)
        {
            if (m_nodeReference[i].node1 == node1 && m_nodeReference[i].node2 == node2 )
                return;
        }
        
        m_nodeReference.push_back(SNodeReference());
        SNodeReference& last = m_nodeReference.back();
        last.node1 = node1;
        last.node2 = node2;
    }
    
    // removeNodeReferenceByAnimLayer
    void removeNodeReferenceByAnimLayer(ISceneNode *node1, ISceneNode *node2)
    {
        for ( int i = 0, n = (int)m_nodeReference.size(); i < n; i++)
        {
            if (m_nodeReference[i].node1 == node1 && m_nodeReference[i].node2 == node2 )
            {
                m_nodeReference.erase(m_nodeReference.begin() + i);
                return;
            }
        }
    }
    
	// isContainSkinMesh
	bool isContainSkinMesh()
	{
		return m_containSkinMesh;
	}

	std::vector<ISceneNode*>* getBoundingMeshNode()
	{
		return &m_boudingMeshNode;
	}

	virtual void render();
protected:
	// compute bb
	core::aabbox3df	computeChildBoudingBox( ISceneNode *node );

};

#endif