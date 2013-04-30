#include "stdafx.h"
#include "CBinaryUtils.h"
#include "IView.h"
#include "core/TextureManager/CTextureManager.h"

//////////////////////////////////////////////
// function c implement
//////////////////////////////////////////////

void getArrayFromVector( const core::vector3df& v, float *floatArray )
{
	floatArray[0] = v.X;
	floatArray[1] = v.Y;
	floatArray[2] = v.Z;
}

void getArrayFromQuaternion( const core::quaternion& q, float *floatArray )
{
	floatArray[0] = q.X;
	floatArray[1] = q.Y;
	floatArray[2] = q.Z;
	floatArray[3] = q.W;
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
const char k_binaryTypeAnimation = 4;

struct SBinaryChunk
{
	int				sign;	
	char			type;
	unsigned int	size;
	int				data[CHUNK_DATASLOT];


	SBinaryChunk()
	{
		sign = CHUNK_SIGN;
		size = 0;
		type = 0;
		memset(data, 0, sizeof(int)*CHUNK_DATASLOT);
	}
#if defined(_MSC_VER)
};
#else
}__attribute__((aligned(4)));
#endif
//////////////////////////////////////////////
// CBinaryUtils implement
//////////////////////////////////////////////

extern float k_defaultAnimFPS;

CBinaryUtils::CBinaryUtils()
{	
}

CBinaryUtils::~CBinaryUtils()
{
}

void CBinaryUtils::saveCollada( io::IWriteFile *file, CGameObject* gameObject )
{
	CColladaMeshComponent *comp = m_component;
	CGameColladaContainerSceneNode *colladaNode = comp->getColladaNode();

	std::queue<CGameColladaSceneNode*> queueNode;

	if ( colladaNode )
	{
		const core::list<ISceneNode*>& childs = colladaNode->getChildren();
		core::list<ISceneNode*>::ConstIterator it = childs.begin(), end = childs.end();
		
		while ( it != end )
		{
			queueNode.push( (CGameColladaSceneNode*) (*it) );
			it++;
		}	
		
		while ( queueNode.size() )
		{
			CGameColladaSceneNode* node = queueNode.front();
			queueNode.pop();


			// save collada node
			saveColladaScene( file, node );

			const core::list<ISceneNode*>& childs = node->getChildren();
			core::list<ISceneNode*>::ConstIterator it = childs.begin(), end = childs.end();
			
			while ( it != end )
			{
				queueNode.push( (CGameColladaSceneNode*) (*it) );
				it++;
			}
		}
	}
}

void CBinaryUtils::saveAnimClip( io::IWriteFile *file, SColladaAnimClip* animClip )
{
	CMemoryReadWrite	memStream( 1024*1024*4 );

	float floatArray[4];
	char stringc[STRING_BUFFER_SIZE];

	// write clip name
	strcpy( stringc, animClip->animName.c_str() );
	memStream.writeData( stringc, STRING_BUFFER_SIZE );

	// write clip data info
	memStream.writeData( &animClip->time, sizeof(float) );
	memStream.writeData( &animClip->duration, sizeof(float) );	
	int loop = animClip->loop==true?1:0;
	memStream.writeData( &loop, sizeof(int) );

	// write number of node anim
	int nNodeAnim = animClip->getNodeAnimCount();
	memStream.writeData( &nNodeAnim, sizeof(int) );
	
	for ( int i = 0; i < nNodeAnim; i++ )
	{
		SColladaNodeAnim* nodeFrames = animClip->getAnimOfSceneNode(i);

		// write name of scenenode
		strcpy( stringc, nodeFrames->sceneNodeName.c_str() );
		memStream.writeData( stringc, STRING_BUFFER_SIZE );	

		// write default rot		
		floatArray[0] = nodeFrames->DefaultRot.X;
		floatArray[1] = nodeFrames->DefaultRot.Y;
		floatArray[2] = nodeFrames->DefaultRot.Z;
		floatArray[3] = nodeFrames->DefaultRot.W;
		memStream.writeData( floatArray, sizeof(float) * 4 );

		// write default pos
		getArrayFromVector( nodeFrames->DefaultPos, floatArray );
		memStream.writeData( floatArray, sizeof(float) * 3 );

		// write default scale
		getArrayFromVector( nodeFrames->DefaultScale, floatArray );
		memStream.writeData( floatArray, sizeof(float) * 3 );


		// write num frame
		int nPos = nodeFrames->PositionKeys.size();
		memStream.writeData( &nPos, sizeof(int) );
		for ( int j = 0; j < nPos; j++ )
		{
			CGameAnimationTrack::SPositionKey &key = nodeFrames->PositionKeys[j];

			memStream.writeData( &key.frame, sizeof(float) );

			getArrayFromVector( key.position, floatArray );
			memStream.writeData( floatArray, sizeof(float) * 3 );
		}

		// write num frame
		int nRot = nodeFrames->RotationKeys.size();
		memStream.writeData( &nRot, sizeof(int) );
		for ( int j = 0; j < nRot; j++ )
		{
			CGameAnimationTrack::SRotationKey &key = nodeFrames->RotationKeys[j];
			
			memStream.writeData( &key.frame, sizeof(float) );

			floatArray[0] = key.rotation.X;
			floatArray[1] = key.rotation.Y;
			floatArray[2] = key.rotation.Z;
			floatArray[3] = key.rotation.W;
			memStream.writeData( floatArray, sizeof(float) * 4 );
		}

		// write num frame
		int nScale = nodeFrames->ScaleKeys.size();
		memStream.writeData( &nScale, sizeof(int) );
		for ( int j = 0; j < nScale; j++ )
		{
			CGameAnimationTrack::SScaleKey &key = nodeFrames->ScaleKeys[j];
			memStream.writeData( &key.frame, sizeof(float) );

			getArrayFromVector( key.scale, floatArray );
			memStream.writeData( floatArray, sizeof(float) * 3 );
		}

	}

	SBinaryChunk chunk;
	chunk.size = memStream.getSize();
	chunk.type = k_binaryTypeAnimation;
	
	// write data to file
	file->write( &chunk, sizeof(SBinaryChunk) );
	file->write( memStream.getData(), memStream.getSize() );
}

void CBinaryUtils::saveAnimation( io::IWriteFile *file, CColladaAnimation *anim )
{		
	int nAnim = anim->getAnimCount();
	for ( int i = 0; i < nAnim; i++ )
	{
		SColladaAnimClip *animClip = anim->getAnim(i);
		saveAnimClip( file, animClip );
	}	
}

void CBinaryUtils::saveColladaScene( io::IWriteFile *file, CGameColladaSceneNode* node )
{	
	CMemoryReadWrite	memStream( 1024*1024*1 );
	std::vector<CGameColladaMesh*>	listMesh;


	char	stringc[STRING_BUFFER_SIZE];
	float	floatArray[4];

	// identity id
	unsigned int pointerID = (unsigned int) ((unsigned long)node);
	memStream.writeData( &pointerID, sizeof(unsigned int) );

	// parent identity id
	pointerID = (unsigned int) ((unsigned long)node->getParent());
	memStream.writeData( &pointerID, sizeof(unsigned int) );

	// name
	strcpy( stringc, node->getName() );
	memStream.writeData( stringc, STRING_BUFFER_SIZE );

	// sid name
	strcpy( stringc, node->getSIDName().c_str() );
	memStream.writeData( stringc, STRING_BUFFER_SIZE );

	// id
	int id = node->getID();
	memStream.writeData(&id, sizeof(int));
	
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
	
	// local transform
	getArrayFromVector( node->LocalPosition, floatArray );
	memStream.writeData( floatArray, sizeof(float)*3 );
	
	getArrayFromVector( node->LocalScale, floatArray );
	memStream.writeData( floatArray, sizeof(float)*3 );

	getArrayFromQuaternion( node->LocalRotation, floatArray);
	memStream.writeData( floatArray, sizeof(float)*4 );	

	memStream.writeData(node->LocalMatrix.pointer(), sizeof(float)*16);

	int useLocalMtx = node->UseLocalMatrix == true?1:0;
	memStream.writeData( &useLocalMtx, sizeof(int) );	

	// check have mesh
	int haveMesh = 1;
	if ( node->ColladaMesh == NULL )
		haveMesh = 0;
	memStream.writeData( &haveMesh, sizeof(int) );
	
	if ( haveMesh )
	{		
		// mesh id
		unsigned int meshID = (unsigned int)((unsigned long)node->ColladaMesh);
		memStream.writeData( &meshID, sizeof(unsigned int) );
		listMesh.push_back( node->ColladaMesh );
	}

	// root node
	int isRootNode = node->isRootColladaNode()?1:0;
	memStream.writeData( &isRootNode, sizeof(int) );

	// fps
	float fps =	k_defaultAnimFPS;
	memStream.writeData( &fps, sizeof(float) );

	
	SBinaryChunk chunk;
	chunk.size = memStream.getSize();
	chunk.type = k_binaryTypeNode;
	
	// write data to file
	file->write( &chunk, sizeof(SBinaryChunk) );
	file->write( memStream.getData(), memStream.getSize() );


	// write mesh info data to file
	std::vector<CGameColladaMesh*>::iterator iMesh =	listMesh.begin(), iEndMesh = listMesh.end();
	while ( iMesh != iEndMesh )
	{
		saveColladaMesh( file, (*iMesh) );
		iMesh++;
	}

}

void CBinaryUtils::saveColladaMesh( io::IWriteFile *file, CGameColladaMesh* mesh )
{
	CMemoryReadWrite	memStream( 1024*1024*4 );
	
	float floatArray[3];
	char stringc[STRING_BUFFER_SIZE];

	// write meshID
	unsigned int meshID = (unsigned int)((unsigned long)mesh);
	memStream.writeData( &meshID, sizeof(int) );

	// static mesh
	int staticMesh = mesh->IsStaticMesh?1:0;
	memStream.writeData( &staticMesh, sizeof(int) );

	// write num joint
	int nJoint = mesh->Joints.size();
	memStream.writeData( &nJoint, sizeof(int) );

	// write bindshapematrix
	memStream.writeData( mesh->BindShapeMatrix.pointer(), sizeof(f32)*16 );	

	// write joint buffer (arrayWeight in bone)
	for (int i = 0; i < nJoint; i++ )
	{
		CGameColladaMesh::SJoint& joint = mesh->Joints[i];

		// joint name
		uiString::convertUnicodeToUTF8(joint.name.c_str(), stringc ); 
		memStream.writeData( stringc, STRING_BUFFER_SIZE );
		
		memStream.writeData(  joint.globalInversedMatrix.pointer(), sizeof(f32)*16 );
		memStream.writeData(  joint.skinningMatrix.pointer(), sizeof(f32)*16 );

		// save bone bbox
		int haveBBox = 0;
		if ( joint.haveBBox == true )
			haveBBox = 1;
		memStream.writeData( &haveBBox, sizeof(int) );
		
		if ( haveBBox )
		{
			getArrayFromVector( joint.bbBox.MaxEdge, floatArray );
			memStream.writeData( floatArray, sizeof(float)*3 );

			getArrayFromVector( joint.bbBox.MinEdge, floatArray );
			memStream.writeData( floatArray, sizeof(float)*3 );
		}

	}	

	
	// bouding box	
	getArrayFromVector( mesh->BoundingBox.MaxEdge, floatArray );
	memStream.writeData( floatArray, sizeof(float)*3 );

	getArrayFromVector( mesh->BoundingBox.MinEdge, floatArray );
	memStream.writeData( floatArray, sizeof(float)*3 );

	// write mesh buffer
	int nMeshBuffer = mesh->getMeshBufferCount();
	memStream.writeData( &nMeshBuffer, sizeof(int) );

	std::vector<SMaterial*>	listMaterial;

	for ( int i = 0; i < nMeshBuffer; i++ )
	{
		// get mesh buffer		
		IMeshBuffer *buffer = mesh->getMeshBuffer(i);		
		
		int vertexType = buffer->getVertexType();
		int vertexCount = buffer->getVertexCount();
		int indexCount = buffer->getIndexCount();

		// write meshbuffer ID
		unsigned int bufferID = (unsigned int)((unsigned long) buffer);
		memStream.writeData( &bufferID, sizeof(unsigned int) );

		// write buffer type
		memStream.writeData( &vertexType, sizeof(int) );
		memStream.writeData( &vertexCount, sizeof(int) );
		memStream.writeData( &indexCount, sizeof(int) );

		// write material id
		SMaterial& mat = buffer->getMaterial();
		unsigned int matID = (unsigned int)((unsigned long)&mat);
		memStream.writeData( &matID, sizeof(unsigned int) );


		long bufferSize = 0;

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
		else if ( vertexType == video::EVT_SKIN )
		{
			bufferSize = sizeof(video::S3DVertexSkin) * vertexCount;
						
			// add to fix if the vertex is modify
			SColladaSkinMeshBuffer *skinBuffer = (SColladaSkinMeshBuffer*)buffer;
			for ( int i = 0; i < vertexCount; i++ )
			{
				skinBuffer->Vertices[i].Pos		= skinBuffer->Vertices[i].StaticPos;
				skinBuffer->Vertices[i].Normal	= skinBuffer->Vertices[i].StaticNormal;
			}
		}

		memStream.writeData( buffer->getVertices(), (unsigned int)bufferSize );

		u16* indices = buffer->getIndices();
		memStream.writeData( indices, (unsigned int)(sizeof(u16)*indexCount) );

		// save list material
		listMaterial.push_back(  &mat );
	}
	

	SBinaryChunk chunk;
	chunk.size = memStream.getSize();
	chunk.type = k_binaryTypeMesh;
	
	// write data to file
	file->write( &chunk, sizeof(SBinaryChunk) );
	file->write( memStream.getData(), memStream.getSize() );


	// save list material
	std::vector<SMaterial*>::iterator iMat = listMaterial.begin(), iMatEnd = listMaterial.end();
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
	unsigned int matID = (unsigned int)((unsigned long)mat);
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
	
	SBinaryChunk chunk;
	chunk.size = memStream.getSize();
	chunk.type = k_binaryTypeMaterial;
	
	// write data to file
	file->write( &chunk, sizeof(SBinaryChunk) );
	file->write( memStream.getData(), memStream.getSize() );

}

void CBinaryUtils::loadAnim( io::IReadFile *file, CColladaAnimation* anim )
{
	SBinaryChunk chunk;	
	
	
	unsigned int maxChunkSize = 1024*1024*4;
	unsigned char *chunkData = new unsigned char[maxChunkSize];

	// read all chunk
	while ( file->read( &chunk, sizeof(SBinaryChunk) ) > 0 )
	{
		if ( chunk.sign != CHUNK_SIGN || chunk.size > maxChunkSize )
			break;		
		
		file->read(chunkData, chunk.size);

		if ( chunk.type == k_binaryTypeAnimation )
		{
			readAnimClip( chunkData, chunk.size, anim );
		}
	}

	delete chunkData;
}

void CBinaryUtils::readAnimClip( unsigned char *data, unsigned int size, CColladaAnimation *anim )
{
	CMemoryReadWrite	memStream(data, size);

	float floatArray[4];
	char stringc[STRING_BUFFER_SIZE];

	SColladaAnimClip *animClip = new SColladaAnimClip();
	
	// read clip name	
	memStream.readData( stringc, STRING_BUFFER_SIZE );

	// this anim has in package
	if ( anim->getAnim( stringc ) != NULL )
		return;

	animClip->animName = stringc;
	anim->addClip( animClip );

	// read clip data info
	memStream.readData( &animClip->time, sizeof(float) );
	memStream.readData( &animClip->duration, sizeof(float) );		

	int loop = 0;
	memStream.readData( &loop, sizeof(int) );
	animClip->loop = (loop == 1 );

	// read number of node anim
	int nNodeAnim = 0;
	memStream.readData( &nNodeAnim, sizeof(int) );

	for ( int i = 0; i < nNodeAnim; i++ )
	{		
		SColladaNodeAnim* nodeFrames = new SColladaNodeAnim();
		
		// read name of scenenode		
		memStream.readData( stringc, STRING_BUFFER_SIZE );
		nodeFrames->sceneNodeName = stringc;		

		// read default rot	
		memStream.readData( floatArray, sizeof(float) * 4 );
		nodeFrames->DefaultRot.X = floatArray[0];
		nodeFrames->DefaultRot.Y = floatArray[1];
		nodeFrames->DefaultRot.Z = floatArray[2];
		nodeFrames->DefaultRot.W = floatArray[3];

		// write default pos
		memStream.readData( floatArray, sizeof(float) * 3 );
		nodeFrames->DefaultPos = core::vector3df( floatArray[0], floatArray[1], floatArray[2] );		

		// write default scale
		memStream.readData( floatArray, sizeof(float) * 3 );
		nodeFrames->DefaultScale = core::vector3df( floatArray[0], floatArray[1], floatArray[2] );

		animClip->addNodeAnim( nodeFrames );


		// read num frame
		int nPos = 0;
		memStream.readData( &nPos, sizeof(int) );
		if ( nPos > 0 )
			nodeFrames->PositionKeys.set_used( nPos );

		for ( int j = 0; j < nPos; j++ )
		{						
			CGameAnimationTrack::SPositionKey &key = nodeFrames->PositionKeys[j];

			memStream.readData( &key.frame, sizeof(float) );			
			memStream.readData( floatArray, sizeof(float) * 3 );
			key.position = core::vector3df( floatArray[0], floatArray[1], floatArray[2] );
		}
		
		// write num frame
		int nRot = 0;
		memStream.readData( &nRot, sizeof(int) );
		if ( nRot > 0 )
			nodeFrames->RotationKeys.set_used( nRot );

		for ( int j = 0; j < nRot; j++ )
		{
			CGameAnimationTrack::SRotationKey &key = nodeFrames->RotationKeys[j];
			
			memStream.readData( &key.frame, sizeof(float) );			
			memStream.readData( floatArray, sizeof(float) * 4 );

			key.rotation.X = floatArray[0];
			key.rotation.Y = floatArray[1];
			key.rotation.Z = floatArray[2];
			key.rotation.W = floatArray[3];
		}

		// write num frame
		int nScale = 0;
		memStream.readData( &nScale, sizeof(int) );
		if ( nScale > 0 )
			nodeFrames->ScaleKeys.set_used( nScale );

		for ( int j = 0; j < nScale; j++ )
		{
			CGameAnimationTrack::SScaleKey &key = nodeFrames->ScaleKeys[j];
			memStream.readData( &key.frame, sizeof(float) );
			memStream.readData( floatArray, sizeof(float) * 3 );

			key.scale = core::vector3df( floatArray[0], floatArray[1], floatArray[2] );
		}		
	}
}


void CBinaryUtils::loadFile( io::IReadFile *file, CGameObject* obj )
{
	SBinaryChunk chunk;	
	
	// clear old scene node
	m_listSceneNode.clear();
	m_listMesh.clear();
	m_listMaterial.clear();
	
	m_constructSceneMesh.clear();
	m_constructMeshMaterial.clear();

	unsigned int maxChunkSize = 1024*1024*4;
	unsigned char *chunkData = new unsigned char[maxChunkSize];
    
	// read all chunk
	while ( file->read( &chunk, sizeof(SBinaryChunk) ) > 0 )
	{
		if ( chunk.sign != CHUNK_SIGN || chunk.size > maxChunkSize )
			break;		
		
		file->read(chunkData, chunk.size);

		switch( chunk.type )
		{
		case k_binaryTypeNode:			
			readColladaScene( chunkData, chunk.size, obj );
			break;
		case k_binaryTypeMesh:
			readColladaMesh( chunkData, chunk.size );
			break;
		case k_binaryTypeMaterial:
			{
				std::string path = file->getFileName().c_str();
				readMaterial( chunkData, chunk.size, path );
			}
			break;
		}
	}

	delete chunkData;

	// current component
	CColladaMeshComponent *comp = m_component;
	CGameColladaContainerSceneNode *pParent = comp->getColladaNode();

	// update scenenode with mesh
	std::vector< SPairID >::iterator it = m_constructSceneMesh.begin(), end = m_constructSceneMesh.end();
	while ( it != end )
	{
		unsigned int nodeID = (*it).first;
		unsigned int meshID = (*it).second;

		CGameColladaSceneNode *node = m_listSceneNode[ nodeID ];
		
		// register node to scene
		if ( m_component )
			m_component->registerName( std::string( node->getName() ), node );

		CGameColladaMesh *mesh = m_listMesh[ meshID ];
		
		if ( node != NULL && mesh != NULL )
		{			
			// set material to buffer
			int nBufferCount = mesh->getMeshBufferCount();
			for ( int i = 0; i < nBufferCount; i++ )
			{
				IMeshBuffer* buffer = mesh->getMeshBuffer(i);
				unsigned int meshbufferID = m_constructMeshBufferID[buffer];

				if ( meshbufferID != 0 )
				{
					// get material of mesh
					unsigned int matID = m_constructMeshMaterial[meshbufferID];
					
					SMaterial *mat = m_listMaterial[matID];
					if ( mat )
						buffer->getMaterial() = *mat;
				}
			}

			// set collada mesh
			node->setColladaMesh( mesh );

			// set child bouding box
			pParent->addBoundingMeshNode( node );
		}
		it++;
	}

	m_constructMeshBufferID.clear();
	m_constructSceneMesh.clear();
	
	// update component to mesh & drop ref of mesh
	std::map<unsigned int, CGameColladaMesh*>::iterator iMesh =	m_listMesh.begin(), iEnd = m_listMesh.end();
	while ( iMesh != iEnd )
	{
		CGameColladaMesh *mesh = (*iMesh).second;
		
		if ( mesh->IsStaticMesh == false )
		{
			mesh->Component = comp;
			mesh->updateJoint();
		}

		mesh->drop();
		iMesh++;
	}
	m_listMesh.clear();

	// release material
	std::map<unsigned int, SMaterial*>::iterator iMat = m_listMaterial.begin(), iEndMat = m_listMaterial.end();
	while ( iMat != iEndMat )
	{
		delete (*iMat).second;
		iMat++;
	}
	m_listMaterial.clear();
}

void CBinaryUtils::readColladaScene( unsigned char *data, unsigned int size, CGameObject* obj )
{
	CMemoryReadWrite memStream( data, size );

	char	stringc[STRING_BUFFER_SIZE];
	float	floatArray[4];
	float	matrix[16];

	// identity id
	unsigned int nodeID = 0;
	memStream.readData( &nodeID, sizeof(unsigned int) );

	// parent identity id
	unsigned int parentID = 0;
	memStream.readData( &parentID, sizeof(unsigned int) );

	// find parent
	ISceneNode *parent = obj->getSceneNode();
	if ( m_listSceneNode[parentID] != NULL )
		parent = m_listSceneNode[parentID];
	
	// create node
	CGameColladaSceneNode *newNode = new CGameColladaSceneNode( parent, parent->getSceneManager(), -1 );
	CColladaMeshComponent *comp = m_component;

	// assign component & register node ID
	newNode->setComponent( comp );
	m_listSceneNode[nodeID] = newNode;

	// name	
	memStream.readData( stringc, STRING_BUFFER_SIZE );
	newNode->setName( stringc );
	comp->registerName( std::string(stringc), newNode );

	// sid name
	memStream.readData( stringc, STRING_BUFFER_SIZE );
	newNode->setSIDName( stringc );	
	comp->registerSID( std::string(stringc), newNode );

	// id
	int id = 0;
	memStream.readData(&id, sizeof(int));
	newNode->setID( id );

	// position
	memStream.readData( floatArray, sizeof(float)*3 );
	newNode->setPosition( core::vector3df(floatArray[0], floatArray[1], floatArray[2]) );

	// rotation	
	memStream.readData( floatArray, sizeof(float)*3 );
	newNode->setRotation( core::vector3df(floatArray[0], floatArray[1], floatArray[2]) );

	// scale	
	memStream.readData( floatArray, sizeof(float)*3 );
	newNode->setScale( core::vector3df(floatArray[0], floatArray[1], floatArray[2]) );

	// cull
	int cull = 0;
	memStream.readData( &cull, sizeof(int) );
	newNode->setAutomaticCulling( (E_CULLING_TYPE)cull);

	// visible
	int visible = 1;
	memStream.readData( &visible, sizeof(int) );
	newNode->setVisible( visible == 1 );
	
	// bouding box
	memStream.readData( floatArray, sizeof(float)*3 );
	newNode->getBoundingBox().MaxEdge = core::vector3df(floatArray[0], floatArray[1], floatArray[2]);
	
	memStream.readData( floatArray, sizeof(float)*3 );
	newNode->getBoundingBox().MinEdge = core::vector3df(floatArray[0], floatArray[1], floatArray[2]);

	// animation matrix	
	memStream.readData( matrix, sizeof(f32)*16 );
	newNode->AnimationMatrix.setM( matrix );

	memStream.readData( matrix, sizeof(f32)*16 );
	newNode->AbsoluteAnimationMatrix.setM( matrix );


	// local transform
	memStream.readData( floatArray, sizeof(float)*3 );
	newNode->LocalPosition = core::vector3df(floatArray[0], floatArray[1], floatArray[2]);

	memStream.readData( floatArray, sizeof(float)*3 );
	newNode->LocalScale = core::vector3df(floatArray[0], floatArray[1], floatArray[2]);

	memStream.readData( floatArray, sizeof(float)*4 );
	newNode->LocalRotation.X = floatArray[0];
	newNode->LocalRotation.Y = floatArray[1];
	newNode->LocalRotation.Z = floatArray[2];
	newNode->LocalRotation.W = floatArray[3];
	
	memStream.readData( newNode->LocalMatrix.pointer(), sizeof(float)*16 );	
	
	int useLocalMtx = 1;	
	memStream.readData( &useLocalMtx, sizeof(int) );
	newNode->UseLocalMatrix = (useLocalMtx == 1);

	// check have mesh
	int haveMesh = 1;	
	memStream.readData( &haveMesh, sizeof(int) );

	if ( haveMesh )
	{		
		// mesh id
		unsigned int meshID = 0;
		memStream.readData( &meshID, sizeof(unsigned int) );	

		// push to list scenenode need construct mesh
		SPairID pairScene;
		pairScene.first = nodeID;
		pairScene.second = meshID;
		m_constructSceneMesh.push_back( pairScene );
	}

	// root node
	int isRootNode = 0;
	memStream.readData( &isRootNode, sizeof(int) );
	newNode->setRootColladaNode( isRootNode == 1 );

	// fps
	float fps =	k_defaultAnimFPS;
	memStream.readData( &fps, sizeof(float) );	
	//newNode->setFPS( fps );

#ifdef GSANIMATION
	if ( haveMesh == 0 )
	{
		// add collision
		ITriangleSelector *selector = getIView()->getSceneMgr()->createTriangleSelectorFromBoundingBox( newNode );
		newNode->setTriangleSelector(selector);
		selector->drop();
	}
#endif

	// drop ref
	newNode->drop();
}

void CBinaryUtils::readColladaMesh( unsigned char *data, unsigned int size )
{
	CMemoryReadWrite memStream( data, size );
	float floatArray[3];	
	char	stringc[STRING_BUFFER_SIZE];
	wchar_t stringw[STRING_BUFFER_SIZE];

	CGameColladaMesh *newMesh = new CGameColladaMesh();

	// read meshID
	unsigned int meshID = 0;
	memStream.readData( &meshID, sizeof(int) );
	m_listMesh[ meshID ] = newMesh;

	// static mesh
	int staticMesh = 0;
	memStream.readData( &staticMesh, sizeof(int) );
	newMesh->IsStaticMesh = (staticMesh == 1);

	// read num joint
	int nJoint = 0;
	memStream.readData( &nJoint, sizeof(int) );
	newMesh->Joints.reallocate( nJoint );

	// read bindshapematrix
	float	matrix[16];
	memStream.readData( matrix, sizeof(f32)*16 );
	newMesh->BindShapeMatrix.setM( matrix );
	newMesh->InvBindShapeMatrix = newMesh->BindShapeMatrix;
	newMesh->InvBindShapeMatrix.makeInverse();

	// read joint buffer (bone matrix)	
	for (int i = 0; i < nJoint; i++ )
	{
		newMesh->Joints.push_back( CGameColladaMesh::SJoint() );
		CGameColladaMesh::SJoint& joint = newMesh->Joints[i];

		// joint name
		memStream.readData( stringc, STRING_BUFFER_SIZE );
		uiString::convertUTF8ToUnicode( stringc, stringw );
		joint.name = stringw;		
		
		memStream.readData(  joint.globalInversedMatrix.pointer(), sizeof(f32)*16 );
		memStream.readData(  joint.skinningMatrix.pointer(), sizeof(f32)*16 );
        
		// load bone bbox
		int haveBBox = 0;
		memStream.readData( &haveBBox, sizeof(int) );
		joint.haveBBox = haveBBox > 0;
		if ( haveBBox > 0 )
		{
			memStream.readData( floatArray, sizeof(float)*3 );
			joint.bbBox.MaxEdge = core::vector3df( floatArray[0], floatArray[1], floatArray[2] );

			memStream.readData( floatArray, sizeof(float)*3 );
			joint.bbBox.MinEdge = core::vector3df( floatArray[0], floatArray[1], floatArray[2] );
		}
	}	

	// read bouding box	
	memStream.readData( floatArray, sizeof(float)*3 );
	newMesh->BoundingBox.MaxEdge = core::vector3df( floatArray[0], floatArray[1], floatArray[2] );
	
	memStream.readData( floatArray, sizeof(float)*3 );
	newMesh->BoundingBox.MinEdge = core::vector3df( floatArray[0], floatArray[1], floatArray[2] );

	// read mesh buffer
	int nMeshBuffer = 0;
	memStream.readData( &nMeshBuffer, sizeof(int) );

	for ( int i = 0; i < nMeshBuffer; i++ )
	{		
		scene::IMeshBuffer* meshBuffer = NULL;

		if ( newMesh->IsStaticMesh )
			meshBuffer = new SColladaMeshBuffer();
		else
			meshBuffer = new SColladaSkinMeshBuffer();


		// write meshbuffer ID
		unsigned int bufferID = 0;
		memStream.readData( &bufferID, sizeof(unsigned int) );

		// save mesh buffer id
		m_constructMeshBufferID[meshBuffer] = bufferID;

		// get mesh buffer				
		int vertexType = 0;
		int vertexCount = 0;
		int indexCount = 0;		

		memStream.readData( &vertexType, sizeof(int) );
		memStream.readData( &vertexCount, sizeof(int) );
		memStream.readData( &indexCount, sizeof(int) );

		unsigned int matID = 0;
		memStream.readData( &matID, sizeof(unsigned int) );

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
		else if ( vertexType == video::EVT_SKIN )
		{
			bufferSize = sizeof(video::S3DVertexSkin) * vertexCount;
		}
				
		if ( newMesh->IsStaticMesh )
		{
			SColladaMeshBuffer *mesh = (SColladaMeshBuffer*)meshBuffer;

			// read vertices
			mesh->Vertices.set_used( vertexCount );
			memStream.readData( mesh->Vertices.pointer(), bufferSize );

			// read indices
			mesh->Indices.set_used( indexCount );
			memStream.readData( mesh->Indices.pointer(), sizeof(u16)*indexCount );                     
		}
		else
		{
			SColladaSkinMeshBuffer *mesh = (SColladaSkinMeshBuffer*)meshBuffer;

			// read vertices
			mesh->Vertices.set_used( vertexCount );
			memStream.readData( mesh->Vertices.pointer(), bufferSize );

			// read indices
			mesh->Indices.set_used( indexCount );
			memStream.readData( mesh->Indices.pointer(), sizeof(u16)*indexCount );            
		}		

		// map meshID & matID		
		m_constructMeshMaterial[bufferID] = matID;

		newMesh->addMeshBuffer( meshBuffer );
		meshBuffer->drop();
	}

	// set static mesh
	if ( newMesh->IsStaticMesh == true )
		newMesh->setHardwareMappingHint( EHM_STATIC );

}

void CBinaryUtils::readMaterial( unsigned char *data, unsigned int size, std::string currentPath )
{
	CMemoryReadWrite memStream( data, size );
	
	SMaterial *mat = new SMaterial();
	
	char stringc[STRING_BUFFER_SIZE];
	
	// write matID
	unsigned int matID = 0;
	memStream.readData( &matID, sizeof(int) );	
	m_listMaterial[matID] = mat;

	int matType = 0;
	memStream.readData( &matType, sizeof(int) );
	mat->MaterialType = (E_MATERIAL_TYPE) matType;

	
	memStream.readData( &mat->AmbientColor.color, sizeof(u32) );
	memStream.readData( &mat->DiffuseColor.color, sizeof(u32) );
	memStream.readData( &mat->EmissiveColor.color, sizeof(u32) );
	memStream.readData( &mat->SpecularColor.color, sizeof(u32) );

	memStream.readData( &mat->Shininess, sizeof(f32) );
	memStream.readData( &mat->MaterialTypeParam, sizeof(f32) );
	memStream.readData( &mat->MaterialTypeParam2, sizeof(f32) );
	memStream.readData( &mat->Thickness, sizeof(f32) );	

	for (u32 i=0; i<MATERIAL_MAX_TEXTURES; ++i)
	{
		// texture
		memStream.readData( stringc, STRING_BUFFER_SIZE );
		if ( strlen(stringc) > 0 )
		{
			// load texture here
			char fullPath[512];

#ifdef _IRR_COMPILE_WITH_OGLES2_
			char fileName[512];

			// replace texture is PVR
			uiString::getFileNameNoExt<char,char>(stringc, fileName);
			uiString::copy<char,char>(stringc,fileName);
			uiString::cat<char, const char>(stringc,".pvr");
#endif

#ifdef USE_ZIPPACKAGE
			uiString::copy<char, char>( fullPath, stringc );
#else
			uiString::getFolderPath<const char,char>( currentPath.c_str(), fullPath );
			uiString::cat<char, const char>( fullPath , "/" );
			uiString::cat<char,char>( fullPath , stringc );
#endif
            // load texture of material
            ITexture *tex = getIView()->getDriver()->getTexture( fullPath );            
			mat->setTexture(i, tex);
            
            // register the texture
            CTextureManager::getInstance()->registerTexture(tex);
		}
	}

	int wireFrame = 0;
	memStream.readData( &wireFrame, sizeof(int) );
	mat->Wireframe = (wireFrame == 1);

	int pointCloud = 0;
	memStream.readData( &pointCloud, sizeof(int) );
	mat->PointCloud = (pointCloud == 1);

	int gouraudShading = 0;
	memStream.readData( &gouraudShading, sizeof(int) );
	mat->GouraudShading = (gouraudShading == 1);

	int light = 0;
	memStream.readData( &light, sizeof(int) );
	mat->Lighting = (light == 1 );

	int zWrite = 0;
	memStream.readData( &zWrite, sizeof(int) );
	mat->ZWriteEnable = (zWrite == 1);

	int backCull = 0;
	memStream.readData( &backCull, sizeof(int) );
	mat->BackfaceCulling = (backCull == 1);

	int frontCull = 0;
	memStream.readData( &frontCull, sizeof(int) );
	mat->FrontfaceCulling = (frontCull == 1);


	int fog = 0;
	memStream.readData( &fog, sizeof(int) );
	mat->FogEnable = (fog == 1);

	int normalNormalize = 0;
	memStream.readData( &normalNormalize, sizeof(int) );
	mat->NormalizeNormals = (normalNormalize == 1);

	int zBuffer = 0;
	memStream.readData( &zBuffer, sizeof(int) );
	mat->ZBuffer = (zBuffer == 1 );
	
	memStream.readData( &mat->AntiAliasing, sizeof(u8) );

	u8 colorMask = 0;
	memStream.readData( &colorMask, sizeof(u8) );
	mat->ColorMask = colorMask;

	u8 colorMaterial = 0;
	memStream.readData( &colorMaterial, sizeof(u8) );
	mat->ColorMaterial = colorMaterial;
}


//////////////////////////////////////////////
// CBinaryUtils implement
//////////////////////////////////////////////

CMemoryReadWrite::CMemoryReadWrite(unsigned int initMem)
{
	m_memory = new unsigned char[initMem];
	m_size = 0;
	m_pos = 0;
	m_fromMemory = false;
}

CMemoryReadWrite::CMemoryReadWrite(unsigned char *fromMem, unsigned int size)
{
	m_memory = fromMem;
	m_size = size;
	m_pos = 0;
	m_fromMemory = true;
}
	
CMemoryReadWrite::~CMemoryReadWrite()
{
	if ( m_fromMemory == false )
		delete m_memory;
}
	
void CMemoryReadWrite::writeData( const void* data, unsigned int size )
{
	memcpy( &m_memory[m_size], data, size );
	m_size += size;
}

unsigned int CMemoryReadWrite::readData( void* data, unsigned int size )
{
	unsigned int maxSize = m_size - m_pos;
	if ( size > maxSize  )
		size = maxSize;

	if ( size <= 0 )
		return 0;

	memcpy( data, &m_memory[m_pos], size );
	m_pos += size;

	return size;
}