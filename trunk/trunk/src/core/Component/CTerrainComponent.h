#ifndef _TERRAIN_COMPONENT_
#define _TERRAIN_COMPONENT_

#include "CGameObject.h"
#include "IObjectComponent.h"

struct SMeshCollisionNode
{
	IMesh		*mesh;
	int			type;
	ISceneNode	*node;		
};

typedef std::vector<SMeshCollisionNode>	ArrayMeshCollision;

class CTerrainComponent: public IObjectComponent
{
protected:

	bool				m_hideTerrain;
	ArrayMeshCollision	m_listCollisionNode;

public:
	CTerrainComponent( CGameObject *pObj );

	virtual ~CTerrainComponent();

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

	// getCollisionFromRay
	// get collision from the ray
	bool getCollisionFromRay( core::line3df &ray, f32 &outBestDistanceSquared, core::vector3df &outBestCollisionPoint, core::triangle3df &outBestTriangle);

	// getNumTerrainNode
	// get number node has collision data
	inline int getNumTerrainNode()
	{
		return m_listCollisionNode.size();
	}

	// getMeshCollisionNode	
	inline SMeshCollisionNode& getMeshCollisionNode( int i )		
	{
		return m_listCollisionNode[i];
	}

protected:
	// checkCollisionFromNode
	// check collision from ray in node
	bool checkCollisionFromNode( ISceneNode* node, core::line3df &ray, f32 &outBestDistanceSquared, core::vector3df &outBestCollisionPoint, core::triangle3df &outBestTriangle);
};

#endif