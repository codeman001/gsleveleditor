#ifndef BOXOBJECT_COMPONENT
#define BOXOBJECT_COMPONENT

#include "IObjectComponent.h"
#include "CGameObject.h"

class CBoxObjectComponent: public IObjectComponent
{
protected:
	float		m_size;
	std::string	m_textureFile;

public:
	CBoxObjectComponent(CGameObject *pObj);

	virtual ~CBoxObjectComponent();

	// createBox
	// create box node
	void createBox();

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