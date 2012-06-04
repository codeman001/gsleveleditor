#include "stdafx.h"
#include "CBinaryUtils.h"


//////////////////////////////////////////////
// function c implement
//////////////////////////////////////////////

void getArrayFromVector( const core::vector3df& v, float *floatArray )
{
	floatArray[0] = v.X;
	floatArray[1] = v.Y;
	floatArray[2] = v.Z;
}

//////////////////////////////////////////////
// struct declare
//////////////////////////////////////////////

#define CHUNK_SIGN		0x3214
#define CHUNK_DATASLOT	5

const char k_binaryTypeNode		= 1;
const char k_binaryTypeMesh		= 2;
const char k_binaryTypeMaterial = 3;

struct SBinaryChunk
{
	int				sign;	
	char			type;
	unsigned long	size;
	int				data[CHUNK_DATASLOT];


	SBinaryChunk()
	{
		sign = CHUNK_SIGN;
		size = 0;
		type = 0;
		memset(data, 0, sizeof(int)*CHUNK_DATASLOT);
	}

};

//////////////////////////////////////////////
// CBinaryUtils implement
//////////////////////////////////////////////

CBinaryUtils::CBinaryUtils()
{	
}

CBinaryUtils::~CBinaryUtils()
{
}

void CBinaryUtils::saveColladaScene( io::IWriteFile *file, CGameColladaSceneNode* node )
{	
	CMemoryReadWrite	memStream( 1024*1024*4 );
	vector<CGameColladaMesh*>	listMesh;


	char	stringc[256];
	float	floatArray[3];

	// identity id
	unsigned long pointerID = (unsigned long) node;
	memStream.writeData( &pointerID, sizeof(unsigned long) );

	// parent identity id
	pointerID = (unsigned long) node->getParent();
	memStream.writeData( &pointerID, sizeof(unsigned long) );

	// name
	strcpy( stringc, node->getName() );
	memStream.writeData( stringc, 256 );

	// id
	int id = node->getID();
	memStream.writeData(&id, sizeof(int));
	
	// transformation
	memStream.writeData(node->getAbsoluteTransformation().pointer(), sizeof(f32)*16);
	memStream.writeData(node->getRelativeTransformation().pointer(), sizeof(f32)*16);

	// position
	getArrayFromVector( node->getPosition(), floatArray );
	memStream.writeData( floatArray, sizeof(float)*3 );

	// rotation
	getArrayFromVector( node->getRotation(), floatArray );
	memStream.writeData( floatArray, sizeof(float)*3 );

	// scale
	getArrayFromVector( node->getScale(), floatArray );
	memStream.writeData( floatArray, sizeof(float)*3 );

	// cull
	int cull = (int)node->getAutomaticCulling();
	memStream.writeData( &cull, sizeof(int) );

	// visible
	int visible = node->isVisible()?1:0;
	memStream.writeData( &visible, sizeof(int) );

	// bouding box
	getArrayFromVector( node->getBoundingBox().MaxEdge, floatArray );
	memStream.writeData( floatArray, sizeof(float)*3 );

	getArrayFromVector( node->getBoundingBox().MinEdge, floatArray );
	memStream.writeData( floatArray, sizeof(float)*3 );

	// animation matrix
	memStream.writeData( node->AnimationMatrix.pointer(), sizeof(f32)*16 );
	memStream.writeData( node->AbsoluteAnimationMatrix.pointer(), sizeof(f32)*16 );
	memStream.writeData( node->LocalMatrix.pointer(), sizeof(f32)*16 );

	// check have mesh
	int haveMesh = 1;
	if ( node->ColladaMesh == NULL )
		haveMesh = 0;
	memStream.writeData( &haveMesh, sizeof(int) );
	
	if ( haveMesh )
	{		
		// is static mesh
		int staticMesh = node->ColladaMesh->IsStaticMesh?1:0;
		memStream.writeData( &staticMesh, sizeof(int) );

		// mesh id
		unsigned long meshID = (unsigned long) node->ColladaMesh;
		memStream.writeData( &meshID, sizeof(unsigned long) );
		listMesh.push_back( node->ColladaMesh );
	}

	// root node
	int isRootNode = node->isRootColladaNode()?1:0;
	memStream.writeData( &isRootNode, sizeof(int) );

	// fps
	float fps =	node->getFPS();
	memStream.writeData( &fps, sizeof(float) );

	
	SBinaryChunk trunk;
	trunk.size = memStream.getSize();
	trunk.type = k_binaryTypeNode;
	
	// write data to file
	file->write( &trunk, sizeof(SBinaryChunk) );
	file->write( memStream.getData(), memStream.getSize() );


	// write mesh info data to file
	vector<CGameColladaMesh*>::iterator iMesh =	listMesh.begin(), iEndMesh = listMesh.end();
	while ( iMesh != iEndMesh )
	{
		saveColladaMesh( file, (*iMesh) );
		iMesh++;
	}

}

void CBinaryUtils::saveColladaMesh( io::IWriteFile *file, CGameColladaMesh* mesh )
{
	
}

void CBinaryUtils::saveMaterial( io::IWriteFile *file, SMaterial* mat )
{

}


//////////////////////////////////////////////
// CBinaryUtils implement
//////////////////////////////////////////////

CMemoryReadWrite::CMemoryReadWrite(unsigned long initMem)
{
	m_memory = new unsigned char[initMem];
	m_size = 0;
	m_pos = 0;
}
	
CMemoryReadWrite::~CMemoryReadWrite()
{
}
	
void CMemoryReadWrite::writeData( const void* data, unsigned long size )
{
	memcpy( &m_memory[m_size], data, size );
	m_size += size;
}

unsigned long CMemoryReadWrite::readData( void* data, unsigned long size )
{
	unsigned long maxSize = m_size - m_pos;
	if ( size > maxSize  )
		size = maxSize;

	if ( size <= 0 )
		return 0;

	memcpy( data, &m_memory[m_pos], size );
	m_pos += size;

	return size;
}