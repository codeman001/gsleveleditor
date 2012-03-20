#ifndef _SKYBOXCOMP_H_
#define _SKYBOXCOMP_H_

#include "CGameObject.h"
#include "IObjectComponent.h"
#include "CGameAnimatedMeshSceneNode.h"

class CSkyboxComponent: public IObjectComponent
{
protected:
	std::string					m_skyboxTexture;

public:
	CSkyboxComponent( CGameObject *pObj );

	virtual ~CSkyboxComponent();

	// init
	// run when init object
	virtual void initComponent();

	// updateComponent
	// update object by frame
	virtual void updateComponent();
	
	// loadSkyTextureFile
	// load file
	void loadSkyTextureFile( char *lpFileName );
public:
	// saveData
	// save data to serializable
	void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	void loadData( CSerializable* pObj );
};

#endif