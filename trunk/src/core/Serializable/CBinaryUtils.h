#ifndef _BINARY_UTILS_H_
#define _BINARY_UTILS_H_

#include "stdafx.h"
#include "CGameObject.h"
#include "CGameColladaSceneNode.h"
#include "CGameContainerSceneNode.h"
#include "CColladaMeshComponent.h"

class CMemoryReadWrite
{
protected:
	unsigned char *m_memory;
	unsigned int m_size;
	unsigned int m_pos;
	bool m_fromMemory;
public:
	CMemoryReadWrite(unsigned int initMem);
	
	CMemoryReadWrite(unsigned char *fromMem, unsigned int size);

	virtual ~CMemoryReadWrite();
	
	void writeData( const void* data, unsigned int size );

	unsigned int readData( void* data, unsigned int size );

	const unsigned char *getData()
	{
		return m_memory;
	}

	unsigned int getSize()
	{
		return m_size;
	}

	unsigned int getPos()
	{
		return m_pos;
	}

	void setPos( unsigned int pos )
	{
		m_pos = pos;
	}
};

typedef std::pair<unsigned int, unsigned int>	SPairID;

class CBinaryUtils: public uiSingleton<CBinaryUtils>
{
protected:
	std::map<unsigned int, CGameColladaSceneNode*>		m_listSceneNode;
	std::map<unsigned int, CGameColladaMesh*>			m_listMesh;
	std::map<unsigned int, SMaterial*>					m_listMaterial;

	std::vector< SPairID >								m_constructSceneMesh;
	std::map<unsigned int, unsigned int>				m_constructMeshMaterial;
	std::map<IMeshBuffer*, unsigned int>				m_constructMeshBufferID;

	CColladaMeshComponent*							m_component;
public:
	CBinaryUtils();
	virtual ~CBinaryUtils();
	
	inline void setCurrentComponent( CColladaMeshComponent *comp )
	{
		m_component = comp;
	}

	void saveCollada( io::IWriteFile *file, CGameObject* gameObject );
	void saveAnimation( io::IWriteFile *file, CColladaAnimation* anim );
	void saveAnimClip( io::IWriteFile *file, SColladaAnimClip* animClip );

	void saveColladaScene( io::IWriteFile *file, CGameColladaSceneNode* node );
	void saveColladaMesh( io::IWriteFile *file, CGameColladaMesh* mesh );	
	void saveMaterial( io::IWriteFile *file, SMaterial* mat );

	void loadFile( io::IReadFile *file, CGameObject* obj );
	void loadAnim( io::IReadFile *file, CColladaAnimation* anim );

protected:
	void readAnimClip( unsigned char *data, unsigned int size, CColladaAnimation *anim );
	void readColladaScene( unsigned char *data, unsigned int size, CGameObject* obj );
	void readColladaMesh( unsigned char *data, unsigned int size );
	void readMaterial( unsigned char *data, unsigned int size, std::string currentPath );
};

#endif