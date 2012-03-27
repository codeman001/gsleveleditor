#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include "CGameObject.h"

class CGameCamera: public CGameObject
{
protected:
	ICameraSceneNode	*m_camera;

#ifdef GSEDITOR
	ISceneNode			*m_cameraMesh;
#endif

	core::vector3df		m_targetPos;
	CGameObject			*m_targetObject;

public:
	CGameCamera();

	virtual ~CGameCamera();

#ifdef GSEDITOR
	// setID
	// set id for object
	virtual void setID( long id )
	{
		m_cameraMesh->setID( id );
		CGameObject::setID(id);		
	}

#endif

	// setTarget
	// set target look at camera
	virtual void setTarget( core::vector3df& v )
	{
		m_targetPos = v;
		m_camera->setTarget( m_targetPos );
	}

	// getTarget
	// get target look
	inline core::vector3df& getTarget()
	{
		return m_targetPos;
	}

	// getCameraNode
	// set camera node
	inline ICameraSceneNode* getCameraNode()
	{
		return m_camera;
	}

	// setTargetObject
	// set object need look
	inline void setTargetObject( CGameObject *p )
	{
		m_targetObject = p;
	}

	// getTargetObject
	// return the object is looking
	inline CGameObject *getTargetObject()
	{
		return m_targetObject;
	}
	
	// updateObject
	// update object by frame...
	virtual void updateObject();

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

#ifdef GSEDITOR
	// drawObject
	// draw lookat vector
	virtual void drawObject();
#endif

};

#endif
