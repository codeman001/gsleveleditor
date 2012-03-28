#ifndef _CTRIGGER_H
#define _CTRIGGER_H

#include "CGameObject.h"

class CTrigger: public CGameObject
{
protected:
	float m_boxSize;

public:
	CTrigger();
	virtual ~CTrigger();

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

};

#endif