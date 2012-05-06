#ifndef _GRASSCOMPONENT_
#define _GRASSCOMPONENT_


#include "CGameObject.h"
#include "IObjectComponent.h"
#include "CGameGrassSceneNode.h"

class CGrassComponent: public IObjectComponent
{	
protected:
	CGameGrassSceneNode*	m_grassNode;
	
	float		m_width;
	float		m_height;
	std::string	m_texture;

	// vertex buffer
	video::S3DVertex	m_vertices[4];

	// index buffer
	u16					m_indices[6];

public:
	CGrassComponent( CGameObject *pObj );

	virtual ~CGrassComponent();

	// init
	// run when init object
	virtual void initComponent();

	// updateComponent
	// update object by frame
	virtual void updateComponent();

	// initGrassNode
	// init grass
	void initGrassNode();
public:
	// saveData
	// save data to serializable
	void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	void loadData( CSerializable* pObj );
};


#endif