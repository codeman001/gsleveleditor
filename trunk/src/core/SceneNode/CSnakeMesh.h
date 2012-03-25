#ifndef _SNAKEMESH_H_
#define _SNAKEMESH_H_

#include "stdafx.h"

class CSnakeMesh: public SMesh
{
protected:
	float	m_segRadius;
	
	list<core::vector3df>*	m_arrayBone;

	int		m_cylinderSeg;
	int		m_cylinderLenSeg;

	bool	m_dirty;

	IDynamicMeshBuffer *m_meshBuffer;
public:
	CSnakeMesh();

	virtual ~CSnakeMesh();

	// skinSnakeMesh
	// update snake vertex buffer
	void skinSnakeMesh();

	// setSnakeBone
	// set snake none
	inline void setSnakeBone( list<core::vector3df>* arrayBone )
	{
		m_arrayBone = arrayBone;
		m_dirty = true;
	}

	// setDirty
	// need skin bone
	void setDirty( bool b )
	{		
		m_dirty = b;
	}
};

#endif