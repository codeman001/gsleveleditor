#ifndef _CGAMEOBJECT_H_
#define _CGAMEOBJECT_H_

#include "stdafx.h"
#include "CSerializable.h"
#include "IObjectComponent.h"
#include "CObjectTemplate.h"

// CGameObject
// Base class for object3d
//  y
//  *    z
//  |   *
//  |  /
//  | /
//  |/
//  *---------* x
class CGameObject: public uiObject
{	
public:
	static core::vector3df s_ox;
	static core::vector3df s_oy;
	static core::vector3df s_oz;

	enum ObjectType
	{
		GameObject,
		TerrainObject,
		ZoneObject,
		Trigger,
		Cinematic,
		NumObject
	};
	
	enum ObjectState
	{
		Normal = 0,
		Move,
		Rotation,
		Scale,
		Review
	};

	static const char* s_stringObjType[];

protected:	
	long				m_objectID;
	std::wstring		m_name;
	std::wstring		m_objectTemplate;

	ObjectType			m_objectType;
	ObjectState			m_objectState;

	core::vector3df		m_position;
	core::vector3df		m_rotation;
	core::vector3df		m_scale;

	core::vector3df		m_oldPosition;
	core::vector3df		m_oldRotation;
	core::vector3df		m_oldScale;

	core::vector3df		m_front;
	core::vector3df		m_up;
	core::vector3df		m_right;

	bool				m_enable;
	bool				m_visible;

	CGameObject			*m_parent;

#ifdef GSEDITOR	
	uiTreeViewItem		*m_treeItem;
#endif

public:
	ISceneNode					*m_node;
	vector<IObjectComponent*>	m_components;
public:	

	CGameObject();

	virtual ~CGameObject();

	// setID
	// set id for object
	virtual void setID( long id )
	{
		m_objectID = id;
		if ( m_node )
			m_node->setID( id );
	}

	// getID
	// get object id
	inline long getID()
	{
		return m_objectID;
	}

		// getName
	// get zone name
	inline const wchar_t* getName()
	{
		return m_name.c_str();
	}

	// setName
	// set name for zone
	inline void setName( const wchar_t *lpName )
	{
		m_name = lpName;
#ifdef GSEDITOR
		if ( m_treeItem )
		{
			m_treeItem->setString( (LPWSTR) lpName );
			m_treeItem->update();
		}
#endif
	}

	// getTemplate
	// return the name of template
	inline wchar_t *getTemplate()
	{
		return (wchar_t*)m_objectTemplate.c_str();
	}

	// setObjectTemplate
	// set template name
	inline void setObjectTemplate( wchar_t *lpName )
	{
		m_objectTemplate = lpName;
	}

#ifdef GSEDITOR	
	inline uiTreeViewItem* getTreeItem()
	{
		return m_treeItem;
	}

	inline void setTreeItem( uiTreeViewItem *p )
	{
		m_treeItem = p;
	}	
#endif

	// getParent
	// get parent object
	inline CGameObject* getParent()
	{
		return m_parent;
	}

	// setParent
	// set parent object
	inline void setParent( CGameObject *p )
	{
		m_parent = p;
	}

	// setObjectState
	// set state for object
	inline void setObjectState( ObjectState state )
	{
		m_objectState = state;
	}

	// getObjectState
	// get state of object
	inline ObjectState getObjectState()
	{
		return m_objectState;
	}

	// getObjectType
	// get object type
	inline ObjectType getObjectType()
	{
		return m_objectType;
	}

	// getPosition
	// get 3d position
	inline core::vector3df& getPosition()
	{
		return m_position;
	}

	// setPosition
	// set 3d position
	inline void setPosition(core::vector3df& pos)
	{
		m_position = pos;
		updateNodePosition();
	}
	
	// getRotation
	// get 3d rotation
	inline core::vector3df& getRotation()
	{
		return m_rotation;
	}

	// setRotation
	// set 3d rotation
	void setRotation(core::vector3df& rot);	

	// getPitch
	inline float getPitch()	{	return m_rotation.X;	}

	// getRoll
	inline float getRoll()	{	return m_rotation.Z;	}

	// getYaw
	inline float getYaw()	{	return m_rotation.Y;	}

	// setYaw
	// rotation oy
	virtual void setYaw(float angle);	
	
	// setYaw
	// rotation ox
	virtual void setPitch(float angle);

	// setYaw
	// rotation oz
	virtual void setRoll(float angle);

	// getQuaternion
	// get rotation
	void getQuaternion( core::quaternion* q );
	
	// setQuaternion
	// set rotation
	void setQuaternion( core::quaternion& q );

	// setOrientation
	// set rotation by front & up
	virtual void setOrientation(const core::vector3df& front, const core::vector3df& up = core::vector3df(0.0f, 1.0f, 0.0f));

	// getFront
	// get front vector of object
	inline core::vector3df&		getFront()	{	return m_front;			}

	// getUp
	// get up vector of object
	inline core::vector3df&		getUp()		{	return m_up;			}

	// getRight
	// get right vector of object
	inline core::vector3df&		getRight()	{	return m_right;			}

	// updateRotation
	void updateRotation();

	// getScale
	// get 3d xyz scale
	inline core::vector3df& getScale() 
	{
		return m_scale;
	}

	// setScale
	// set 3d xyz scale
	inline void setScale(core::vector3df& scale) 
	{
		m_scale = scale;
		updateNodeScale();
	}

	// lookAt
	// rotation object to the object
	void lookAt(CGameObject* pObject);

	// lookAt
	// rotation object to pos
	void lookAt(const core::vector3df& pos);

	// getSceneNode
	// return irrlicht node
	inline ISceneNode* getSceneNode()
	{
		return m_node;
	}

	// isEnable
	// check the object is enable
	inline bool isEnable()
	{
		return m_enable;
	}

	// isVisible
	// check the object is visible
	inline bool isVisible()
	{
		return m_visible;
	}

	// setEnable
	// set enable of object
	virtual void setEnable( bool b )
	{
		m_enable = b;		
	}

	// setVisible
	// set vidible of object ( false -> hidden )
	virtual void setVisible( bool b )
	{
		m_visible = b;
		if ( m_node )
			m_node->setVisible( b );
	}

	// updateObject
	// update object by frame...
	virtual void updateObject();	

	// destroyNode
	// remove node on scene
	virtual void destroyNode();
	
	// saveData
	// save data to serializable
	virtual void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	virtual void loadData( CSerializable* pObj );

	// releaseAllComponent
	// delete all component on object
	virtual void releaseAllComponent();

	// initComponent
	// create a component on object
	virtual void initComponent( CSerializable* componentData );
		
	// saveTransform
	// save all transform
	void saveTransform();

	// loadTransform
	// load all transform
	void loadTransform();

public:

	// drawFrontUpLeftVector
	// draw front, left, up vector
	void drawFrontUpLeftVector();

	// drawCircleAroundObj
	// draw circle around object
	void drawCircleAroundObject();

	// updateNodeRotation
	// update irr node rotation
	void updateNodeRotation();

	// updateNodePosition
	// update irr node position
	void updateNodePosition();

	// updateNodeScale
	// update irr node scale
	void updateNodeScale();

protected:

	// fixRotationVector
	// fix rotation angle from 0 -> 360
	void fixRotationVector();

};

// typedef for array object
typedef vector<CGameObject*>			ArrayGameObject;
typedef vector<CGameObject*>::iterator	ArrayGameObjectIter;

// typedef for array component
typedef vector<IObjectComponent*>			ArrayComponent;
typedef vector<IObjectComponent*>::iterator	ArrayComponentIter;

#endif