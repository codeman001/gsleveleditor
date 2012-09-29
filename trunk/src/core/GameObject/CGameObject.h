#ifndef _CGAMEOBJECT_H_
#define _CGAMEOBJECT_H_

#include "stdafx.h"
#include "CSerializable.h"
#include "IObjectComponent.h"
#include "CObjectTemplate.h"
#include "CDataPacket.h"

// prototype class declare
class CWayPoint;

// CGameObject
// Base class for object3d
//  y
//  *    z
//  |   *
//  |  /
//  | /
//  |/
//  *---------* x


class CGameObject
#ifdef GSEDITOR
	: public uiObject
#endif
{	
public:
	static long		s_objectID;

public:
	static core::vector3df s_ox;
	static core::vector3df s_oy;
	static core::vector3df s_oz;

	enum ObjectType
	{
		GameObject,
		ZoneObject,
		WaypointObject,
		CameraObject,
		TriggerObject,
		LightObject,
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
#ifdef GSGAMEPLAY
	std::wstring		m_defaultName;
#endif
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
	bool				m_lighting;
	bool				m_lockObject;

	bool				m_needSortComponent;
    bool                m_needSyncNetwork;
    bool                m_isNetworkController;
    
	CGameObject			*m_parent;

#ifdef GSEDITOR	
	uiTreeViewItem		*m_treeItem;
	bool				m_uiVisible;
	bool				m_dummyObject;
#endif

public:
	ISceneNode					*m_node;
	std::vector<IObjectComponent*>	m_components;

	void addComponenet( IObjectComponent* comp )
	{
		m_components.push_back( comp );
	}

public:	

	CGameObject();

	CGameObject(CGameObject *parent);

	virtual ~CGameObject();

protected:
	void initNull();

public:
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

#ifdef GSGAMEPLAY
	inline const wchar_t* getDefaultName()
	{
		return m_defaultName.c_str();
	}
#endif

	// setName
	// set name for zone
	inline void setName( const wchar_t *lpName )
	{
		m_name = lpName;

#ifdef GSGAMEPLAY
		if ( m_defaultName == L"" )
			m_defaultName = lpName;
#endif

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

	inline bool isUIVisible()
	{
		return m_uiVisible;
	}

	inline bool isDummyObject()
	{
		return m_dummyObject;
	}

	inline void setDummyObject(bool b)
	{
		m_dummyObject = b;
	}

	virtual void setUIVisible(bool b)
	{
		m_uiVisible = b;

		if ( m_node )
			m_node->setVisible( m_visible && b );
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
	virtual void setPosition(core::vector3df& pos)
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
	virtual void setRotation(core::vector3df& rot);	

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

	// getParentSceneNode
	// return irrlicht node of parent
	ISceneNode* getParentSceneNode();	

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
	// set visible of object ( false -> hidden )
	virtual void setVisible( bool b )
	{
		m_visible = b;
		if ( m_node )
		{
#ifdef GSEDITOR
			m_node->setVisible( b && m_uiVisible );
#else
			m_node->setVisible( b );
#endif
		}
	}

	// setLockObject
	// set lock object
	inline void setLockObject( bool b )
	{
		m_lockObject = b;
	}

	// isLock
	// check object is lock
	inline bool isLock()
	{
		return m_lockObject;
	}

    // setSyncNetwork
    // mark to sync network
    inline void setSyncNetwork( bool b )
    {
        m_needSyncNetwork = b;
    }
    
    // isSyncNetwork
    // check to sync network
    bool isSyncNetwork()
    {
        return m_needSyncNetwork;
    }
    
    // setNetworkController
    // set network controller object
    inline void setNetworkController( bool b )
    {
        m_isNetworkController = b;
    }
    
    // isNetworkController
    // check the network controller
    inline bool isNetworkController()
    {
        return m_isNetworkController;
    }    
    
	virtual void setLighting( bool b );

	virtual bool isLighting()
	{
		return m_lighting;
	}

	// updateObject
	// update object by frame...
	virtual void updateObject();	

	// destroyNode
	// remove node on scene
	virtual void destroyNode();
	
	// remove
	// destroy object
	virtual void remove();

	// saveData
	// save data to serializable ( use for save level .lv )
	virtual void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable ( use for load level .lv )
	virtual void loadData( CSerializable* pObj );

	// getData
	// get basic data to serializable
	virtual void getData( CSerializable* pObj );

	// updateData
	// update data
	virtual void updateData( CSerializable* pObj );

    // packDataMultiplayer
    // pack data multiplayer
    virtual void packDataMultiplayer(CDataPacket *packet);
    
    // unPackDataMultiplayer
    // unpack data on multiplayer
    virtual void unpackDataMultiplayer(CDataPacket *packet, int hostKeyId);
    
	// releaseAllComponent
	// delete all component on object
	virtual void releaseAllComponent();

	// initComponent
	// create a component on object
	virtual void createComponent( CSerializable* componentData );
	
	// initComponent
	// init all component on object
	virtual void initComponent( );

	// sortComponentById
	// sort array component
	virtual void sortComponentById();

	// getComponent
	// get component
	IObjectComponent *getComponent( int componentID );

	// saveTransform
	// save all transform
	void saveTransform();

	// loadTransform
	// load all transform
	void loadTransform();

	// setTransform
	// set transform by a matrix
	void setTransform( const core::matrix4& mat );

#ifdef GSGAMEPLAY
	// setPositionMoveToWayPoint
	// set position
	void setAnimatorMoveToWayPoint( CWayPoint *wayPoint, float speed, bool loop );
#endif

public:

#ifdef GSEDITOR
	// isLineHit
	// mouse x, y is hit on line x1y1 - x2y2
	bool isLineHit( int X1, int Y1, int X2, int Y2, int X, int Y );

	// isHittestObjectVector
	// typeVector:	1 ox, 2 oy, 3, oz
	bool isHittestObjectVector( int x, int y, int typeVector );
#endif

	// drawFrontUpLeftVector
	// draw front, left, up vector
	void drawFrontUpLeftVector();

	// drawCircleAroundObj
	// draw circle around object
	void drawCircleAroundObject();

#ifdef GSEDITOR
	// drawObject
	// draw another on node
	virtual void drawObject()
	{
	}
#endif

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
typedef std::vector<CGameObject*>			ArrayGameObject;
typedef std::vector<CGameObject*>::iterator	ArrayGameObjectIter;

// typedef for array component
typedef std::vector<IObjectComponent*>              ArrayComponent;
typedef std::vector<IObjectComponent*>::iterator	ArrayComponentIter;

// struct network id
struct SNetworkObjID
{
    short   hostID;
    long    objectID;
    
    bool operator < (const SNetworkObjID& other) const
    {
        if ( hostID != other.hostID )
            return hostID < other.hostID;
        else 
            return objectID < other.objectID;    
    }
};

#endif