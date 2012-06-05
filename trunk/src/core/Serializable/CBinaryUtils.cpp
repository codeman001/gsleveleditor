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

#define CHUNK_SIGN			0x3214
#define CHUNK_DATASLOT		5
#define STRING_BUFFER_SIZE	256

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
	CMemoryReadWrite	memStream( 1024*1024*1 );
	vector<CGameColladaMesh*>	listMesh;


	char	stringc[STRING_BUFFER_SIZE];
	float	floatArray[3];

	// identity id
	unsigned long pointerID = (unsigned long) node;
	memStream.writeData( &pointerID, sizeof(unsigned long) );

	// parent identity id
	pointerID = (unsigned long) node->getParent();
	memStream.writeData( &pointerID, sizeof(unsigned long) );

	// name
	strcpy( stringc, node->getName() );
	memStream.writeData( stringc, STRING_BUFFER_SIZE );

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
	CMemoryReadWrite	memStream( 1024*1024*4 );
	
	// write meshID
	unsigned long meshID = (unsigned long)mesh;
	memStream.writeData( &meshID, sizeof(int) );

	// write num joint
	int nJoint = mesh->Joints.size();
	memStream.writeData( &nJoint, sizeof(int) );

	// write joint buffer (arrayWeight in bone)
	CGameColladaMesh::SJoint *jointBuffer =	mesh->Joints.pointer();	
	memStream.writeData( jointBuffer, sizeof(CGameColladaMesh::SJoint)*nJoint );

	// write num joint index
	int nJointIndex = mesh->JointIndex.size();
	memStream.writeData( &nJointIndex, sizeof(int) );
	
	// write joint index (boneID, weightID) buffer
	memStream.writeData( mesh->JointIndex.pointer(), sizeof(s32) * nJointIndex );

	// write num joint(bone) in vertex
	int nJointVertexIndex =	mesh->JointVertexIndex.size();
	memStream.writeData( &nJointVertexIndex, sizeof(int) );

	memStream.writeData( mesh->JointVertexIndex.pointer(), sizeof(s32) * nJointVertexIndex );

	
	// bouding box
	float floatArray[3];
	getArrayFromVector( mesh->BoundingBox.MaxEdge, floatArray );
	memStream.writeData( floatArray, sizeof(float)*3 );

	getArrayFromVector( mesh->BoundingBox.MinEdge, floatArray );
	memStream.writeData( floatArray, sizeof(float)*3 );

	// write mesh buffer
	int nMeshBuffer = mesh->getMeshBufferCount();
	memStream.writeData( &nMeshBuffer, sizeof(int) );

	vector<SMaterial*>	listMaterial;

	for ( int i = 0; i < nMeshBuffer; i++ )
	{
		// get mesh buffer
		IMeshBuffer *buffer = mesh->getMeshBuffer(i);
		
		int vertexType = buffer->getVertexType();
		int vertexCount = buffer->getVertexCount();
		int indexCount = buffer->getIndexCount();

		memStream.writeData( &vertexType, sizeof(int) );
		memStream.writeData( &vertexCount, sizeof(int) );
		memStream.writeData( &indexCount, sizeof(int) );

		SMaterial& mat = buffer->getMaterial();
		unsigned long matID = (unsigned long)&mat;
		memStream.writeData( &matID, sizeof(unsigned long) );


		int bufferSize = 0;

		if ( vertexType == video::EVT_STANDARD )
		{
			bufferSize = sizeof(video::S3DVertex) * vertexCount;
		}
		else if ( vertexType == video::EVT_2TCOORDS )
		{
			bufferSize = sizeof(video::S3DVertex2TCoords) * vertexCount;
		}
		else if ( vertexType == video::EVT_TANGENTS )
		{
			bufferSize = sizeof(video::S3DVertexTangents) * vertexCount;
		}

		memStream.writeData( buffer->getVertices(), bufferSize );

		u16* indices = buffer->getIndices();
		memStream.writeData( indices, sizeof(u16)*indexCount );

		// save list material
		listMaterial.push_back(  &mat );
	}
	

	SBinaryChunk trunk;
	trunk.size = memStream.getSize();
	trunk.type = k_binaryTypeMesh;
	
	// write data to file
	file->write( &trunk, sizeof(SBinaryChunk) );
	file->write( memStream.getData(), memStream.getSize() );


	// save list material
	vector<SMaterial*>::iterator iMat = listMaterial.begin(), iMatEnd = listMaterial.end();
	while ( iMat != iMatEnd )
	{
		saveMaterial( file, (*iMat) );
		iMat++;
	}

}

void CBinaryUtils::saveMaterial( io::IWriteFile *file, SMaterial* mat )
{
	CMemoryReadWrite	memStream( 1024*1024*1 );

	// write matID
	unsigned long matID = (unsigned long)mat;
	memStream.writeData( &matID, sizeof(int) );

	int matType = mat->MaterialType;
	memStream.writeData( &matType, sizeof(int) );

	memStream.writeData( &mat->AmbientColor.color, sizeof(u32) );
	memStream.writeData( &mat->DiffuseColor.color, sizeof(u32) );
	memStream.writeData( &mat->EmissiveColor.color, sizeof(u32) );
	memStream.writeData( &mat->SpecularColor.color, sizeof(u32) );

	memStream.writeData( &mat->Shininess, sizeof(f32) );
	memStream.writeData( &mat->MaterialTypeParam, sizeof(f32) );
	memStream.writeData( &mat->MaterialTypeParam2, sizeof(f32) );
	memStream.writeData( &mat->Thickness, sizeof(f32) );

	char stringc[STRING_BUFFER_SIZE];

	for (u32 i=0; i<MATERIAL_MAX_TEXTURES; ++i)
	{
		if ( mat->TextureLayer[i].Texture != NULL )
		{
			irr::core::stringc name(mat->TextureLayer[i].Texture->getName());			
			uiString::getFileName<const irr::c8, char>(name.c_str(), stringc);
		}
		else
		{
			memset(stringc, 0, STRING_BUFFER_SIZE);		
		}
		memStream.writeData( stringc, STRING_BUFFER_SIZE );
	}

	int wireFrame = mat->Wireframe?1:0;
	memStream.writeData( &wireFrame, sizeof(int) );

	int pointCloud = mat->PointCloud?1:0;
	memStream.writeData( &pointCloud, sizeof(int) );

	int gouraudShading = mat->GouraudShading?1:0;
	memStream.writeData( &gouraudShading, sizeof(int) );

	int light = mat->Lighting?1:0;
	memStream.writeData( &light, sizeof(int) );

	int zWrite = mat->ZWriteEnable?1:0;
	memStream.writeData( &zWrite, sizeof(int) );

	int backCull = mat->BackfaceCulling?1:0;
	memStream.writeData( &backCull, sizeof(int) );

	int frontCull = mat->FrontfaceCulling?1:0;
	memStream.writeData( &frontCull, sizeof(int) );

	int fog = mat->FogEnable?1:0;
	memStream.writeData( &fog, sizeof(int) );

	int normalNormalize = mat->NormalizeNormals?1:0;
	memStream.writeData( &normalNormalize, sizeof(int) );

	int zBuffer = mat->ZBuffer?1:0;
	memStream.writeData( &zBuffer, sizeof(int) );
	
	memStream.writeData( &mat->AntiAliasing, sizeof(u8) );

	u8 colorMask = mat->ColorMask;
	memStream.writeData( &colorMask, sizeof(u8) );

	u8 colorMaterial = mat->ColorMaterial;
	memStream.writeData( &colorMaterial, sizeof(u8) );
	
	SBinaryChunk trunk;
	trunk.size = memStream.getSize();
	trunk.type = k_binaryTypeMaterial;
	
	// write data to file
	file->write( &trunk, sizeof(SBinaryChunk) );
	file->write( memStream.getData(), memStream.getSize() );

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
	delete m_memory;
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