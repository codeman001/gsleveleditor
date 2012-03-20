#ifndef _STATICMESH_COMPONENT_
#define _STATICMESH_COMPONENT_


#include "CGameObject.h"
#include "IObjectComponent.h"
#include "CGameMeshSceneNode.h"

class CStaticMeshComponent: public IObjectComponent
{
public:
	
protected:
	CGameMeshSceneNode*			m_meshNode;

	std::string					m_meshFile;
public:
	CStaticMeshComponent( CGameObject *pObj );

	virtual ~CStaticMeshComponent();

	// init
	// run when init object
	virtual void initComponent();

	// updateComponent
	// update object by frame
	virtual void updateComponent();

	// loadFromFile
	// load anim object from file
	void loadFromFile( char *lpFilename );

	// getMeshFile
	// get current mesh file name
	const char *getMeshFile()
	{
		return m_meshFile.c_str();
	}

public:
	// saveData
	// save data to serializable
	void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	void loadData( CSerializable* pObj );
};


#endif