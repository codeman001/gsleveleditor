#ifndef LIGHTING_COMPONENT
#define LIGHTING_COMPONENT

#include "IObjectComponent.h"
#include "CGameObject.h"

class CLightingComponent: public IObjectComponent
{
protected:
	ILightSceneNode	*m_lightSceneNode;

	SColor	m_diffuseColor;
	SColor	m_specularColor;
	float	m_strength;
	float	m_radius;

public:
	CLightingComponent(CGameObject *pObj);

	virtual ~CLightingComponent();	

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

protected:
	
	// getColorString
	// get string hexa color name from SColor
	std::string getColorString( const SColor& color );

	// setColorString
	// get SColor from string hexa name
	SColor setColorString( const std::string& stringColor );

};

#endif