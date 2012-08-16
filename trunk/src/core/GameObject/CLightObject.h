#ifndef _CLIGHTOBJECT_H
#define _CLIGHTOBJECT_H

#include "CGameObject.h"

class CLightObject: public CGameObject
{
protected:
	ILightSceneNode	*m_lightSceneNode;

	int		m_lightType;
	SColor	m_diffuseColor;
	SColor	m_specularColor;
	float	m_strength;
	float	m_radius;

#ifdef GSEDITOR
	CGameObject	*m_directionObj;
#endif

protected:

	void initLight();

public:
	CLightObject();

	CLightObject(CGameObject *parent);

	virtual ~CLightObject();

	// saveData
	// save data to serializable ( use for save in game .sav )
	virtual void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable ( use for load in game .sav )
	virtual void loadData( CSerializable* pObj );

	// getData
	// get basic data to serializable
	virtual void getData( CSerializable* pObj );

	// updateData
	// update data
	virtual void updateData( CSerializable* pObj );

	// updateObject
	// update object by frame...
	virtual void updateObject();

public:
#ifdef GSEDITOR
	// getDirectionObject
	// return obj to adjust direction on game editor
	CGameObject	*getDirectionObject()
	{
		return m_directionObj;
	}
#endif

	// getColorString
	// get string hexa color name from SColor
	std::string getColorString( const SColor& color );

	// setColorString
	// get SColor from string hexa name
	SColor setColorString( const std::string& stringColor );

#ifdef GSEDITOR
	// drawObject	
	virtual void drawObject();
#endif

};

#endif