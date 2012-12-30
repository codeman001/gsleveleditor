#ifndef _CLIGHTOBJECT_H
#define _CLIGHTOBJECT_H

#include "CGameObject.h"

class CLightObject: public CGameObject
{
protected:
	ILightSceneNode	*m_lightSceneNode;

	int				m_lightType;
	SColor			m_diffuseColor;
	SColor			m_specularColor;
	float			m_strength;
	float			m_radius;
	core::vector3df	m_direction;

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

	// setLightOrientation
	// set light dir rotation
	void setLightOrientation( const core::vector3df& v );

#ifdef GSEDITOR
	// drawObject	
	virtual void drawObject();
#endif

    // getLightSceneNode
    // get light obj
    inline ILightSceneNode* getLightSceneNode()
    {
        return m_lightSceneNode;
    }
    
    // setVisible
    // turn on/off light
    inline void setVisible(bool b)
    {
        m_lightSceneNode->setVisible(b);
    }
    
    // setLightData
    // set light data
    void setLightData(float radius, float strength);
};

// A utility class to aid in sorting scene nodes into a distance order
class LightDistanceElement
{
public:
	LightDistanceElement() {};

	LightDistanceElement(scene::ILightSceneNode* n, f64 d)
		: node(n), distance(d) { }

	scene::ILightSceneNode* node;
	f64 distance;

	// Lower distance elements are sorted to the start of the array
	bool operator < (const LightDistanceElement& other) const
	{
		return (distance < other.distance);
	}
};

class CLightManager: public	ILightManager
{
protected:
	core::array<ILightSceneNode*>*	m_listLightOnMap;
	scene::E_SCENE_NODE_RENDER_PASS m_renderPass;
public: 
	CLightManager();
	virtual ~CLightManager();

	ILightManager* getLightMgr()
	{
		return (ILightManager*)this;
	}

	// OnPreRender
	// This is called before the first scene node is rendered.
	virtual void OnPreRender(core::array<ILightSceneNode*> & lightList);
	

	// OnPostRender
	// Called after the last scene node is rendered.
	virtual void OnPostRender(void);

	virtual void OnRenderPassPreRender(E_SCENE_NODE_RENDER_PASS renderPass);

	virtual void OnRenderPassPostRender(E_SCENE_NODE_RENDER_PASS renderPass);

	// OnNodePreRender
	// This is called before the specified scene node is rendered
	virtual void OnNodePreRender(ISceneNode* node);
		
	// OnNodePostRender
	// Called after the specified scene node is rendered
	virtual void OnNodePostRender(ISceneNode* node);
};

#endif