#ifndef _BINARY_UTILS_H_
#define _BINARY_UTILS_H_

#include "stdafx.h"
#include "CGameObject.h"
#include "CGameColladaSceneNode.h"
#include "CGameContainerSceneNode.h"

class CMemoryReadWrite
{
protected:
	unsigned char *m_memory;
	unsigned long m_size;
	unsigned long m_pos;
	bool m_fromMemory;
public:
	CMemoryReadWrite(unsigned long initMem);
	
	CMemoryReadWrite(unsigned char *fromMem, unsigned long size);

	virtual ~CMemoryReadWrite();
	
	void writeData( const void* data, unsigned long size );

	unsigned long readData( void* data, unsigned long size );

	const unsigned char *getData()
	{
		return m_memory;
	}

	unsigned long getSize()
	{
		return m_size;
	}

	unsigned long getPos()
	{
		return m_pos;
	}

	void setPos( unsigned long pos )
	{
		m_pos = pos;
	}
};

class CBinaryUtils: public uiSingleton<CBinaryUtils>
{
protected:
	map<unsigned long, CGameColladaSceneNode*>		m_listSceneNode;
	map<unsigned long, CGameColladaMesh*>			m_listMesh;
	map<unsigned long, SMaterial>					m_listMaterial;

public:
	CBinaryUtils();
	virtual ~CBinaryUtils();
	
	void saveColladaScene( io::IWriteFile *file, CGameColladaSceneNode* node );
	void saveColladaMesh( io::IWriteFile *file, CGameColladaMesh* mesh );	
	void saveMaterial( io::IWriteFile *file, SMaterial* mat );

	void loadFile( io::IReadFile *file, CGameObject* obj );

protected:
	void readColladaScene( unsigned char *data, unsigned long size, CGameObject* obj );
	void readColladaMesh( unsigned char *data, unsigned long size );
	void readMaterial( unsigned char *data, unsigned long size );
};

#endif