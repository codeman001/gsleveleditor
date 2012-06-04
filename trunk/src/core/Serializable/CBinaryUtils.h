#ifndef _BINARY_UTILS_H_
#define _BINARY_UTILS_H_

#include "stdafx.h"
#include "CGameColladaSceneNode.h"

class CMemoryReadWrite
{
protected:
	unsigned char *m_memory;
	unsigned long m_size;
	unsigned long m_pos;
public:
	CMemoryReadWrite(unsigned long initMem);
	
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
public:
	CBinaryUtils();
	virtual ~CBinaryUtils();
	
	void saveColladaScene( io::IWriteFile *file, CGameColladaSceneNode* node );
	void saveColladaMesh( io::IWriteFile *file, CGameColladaMesh* mesh );	
	void saveMaterial( io::IWriteFile *file, SMaterial* mat );

};

#endif