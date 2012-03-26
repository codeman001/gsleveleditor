#ifndef _WAYPOINT_
#define _WAYPOINT_

#include "CGameObject.h"

class CWayPoint: public CGameObject
{
protected:
	CWayPoint	*m_next;
	CWayPoint	*m_back;
	
	long		m_timeWait;
public:
	CWayPoint();

	virtual ~CWayPoint();

	// setNext
	// set the next way point
	inline void setNext( CWayPoint *p )
	{
		m_next = p;
	}

	// setBack
	// set the next way point
	inline void setBack( CWayPoint *p )
	{
		m_back = p;
	}

	// setTimeWay
	// way milisecond time
	inline void setTimeWay(long timeWait)
	{
		m_timeWait = timeWait;
	}

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
	virtual void drawObject();
#endif

};

#endif