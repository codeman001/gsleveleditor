#ifndef BILLBOARD_COMPONENT
#define BILLBOARD_COMPONENT

#include "IObjectComponent.h"
#include "CGameObject.h"

class CBillboardComponent: public IObjectComponent
{
protected:
	std::string			m_textureFile;
	core::dimension2df	m_billboardSize;
public:
	CBillboardComponent(CGameObject *pObj);

	virtual ~CBillboardComponent();	

	// createBillboard
	// create billboard scenenode
	void createBillboard();

	// init
	// run when init object
	virtual void initComponent();

	// update
	// run when update per frame
	virtual void updateComponent();

	// saveData
	// save data to serializable
	virtual void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	virtual void loadData( CSerializable* pObj );
};

#endif