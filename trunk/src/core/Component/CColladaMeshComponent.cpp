#include "stdafx.h"
#include "CColladaMeshComponent.h"
#include "IView.h"

#include "SAnimatedMesh.h"
#include "ISkinnedMesh.h"
#include "CSkinnedMesh.h"

#include "CGameAnimatedMeshSceneNode.h"
#include "CGameMeshSceneNode.h"
#include "CGameContainerSceneNode.h"

#include "CBinaryUtils.h"

void			uriToId(std::wstring& str);
wstring			readId(io::IXMLReader *xmlRead);
void			findNextNoneWhiteSpace(const c8** start);
inline f32		readFloat(const c8** p);

void			readIntsInsideElement(io::IXMLReader* reader, s32* ints, u32 count);
void			readIntsInsideElement(io::IXMLReader* reader, vector<s32>& arrayInt);
void			readFloatsInsideElement(io::IXMLReader* reader, f32* floats, u32 count);
void			readStringInsideElement(io::IXMLReader* reader, vector<std::wstring>& arrayString);

SColorf			readColorNode(io::IXMLReader* reader);
f32				readFloatNode(io::IXMLReader* reader);
core::matrix4	readTranslateNode(io::IXMLReader* reader, bool flip);
core::matrix4	readRotateNode(io::IXMLReader* reader, bool flip);
core::matrix4	readScaleNode(io::IXMLReader* reader, bool flip);

ITexture*		getTextureFromImage( std::string& basePath, std::wstring& uri, ArrayEffectParams& listEffectParam);
int				getBufferWithUri( std::wstring& uri, SMeshParam* mesh );
int				getVerticesWithUri( std::wstring& uri, SMeshParam* mesh );
int				getEffectWithUri( std::wstring& uri, ArrayEffects& listEffectParam );
int				getMeshWithUri( std::wstring& uri, ArrayMeshParams& listMeshParam );
int				getMeshWithControllerName( std::wstring& controllerName, ArrayMeshParams& listMeshParam );


//////////////////////////////////////////////////////////
// CColladaCache implement
//////////////////////////////////////////////////////////

map<string, CGameChildContainerSceneNode*>	CColladaCache::s_nodeCache;


//////////////////////////////////////////////////////////
// CColladaAnimation implement
//////////////////////////////////////////////////////////

const float k_defaultAnimFPS = 40.0f;

CColladaAnimation::CColladaAnimation()
{
}

CColladaAnimation::~CColladaAnimation()
{
	// release all anim clip
	vector<SColladaAnimClip*>::iterator it = m_colladaAnim.begin(), end = m_colladaAnim.end();
	while ( it != end )
	{
		delete (*it);
		it++;
	}

	m_colladaAnim.clear();
	m_animWithName.clear();
}


// getFrameAtTime
// get a frame at time
bool CColladaAnimation::getRotationFrameID( SColladaNodeAnim* frames, float frame, int *frameRotID, core::quaternion *rotateData )
{	
	int nRotFrames = frames->RotationKeys.size();
		
	int first = 0, last = nRotFrames - 2;
	int mid = 0;
		
	while (first <= last)
	{
		mid = (first + last) / 2;

		if ( frame > frames->RotationKeys[mid].frame && frame > frames->RotationKeys[mid + 1].frame )
			first = mid + 1;
		else if ( frame < frames->RotationKeys[mid].frame )
			last = mid - 1;
		else
		{			
			CGameColladaSceneNode::SRotationKey &frame1 = frames->RotationKeys[ mid ];
			CGameColladaSceneNode::SRotationKey &frame2 = frames->RotationKeys[ mid + 1 ];

			core::quaternion q1, q2;
			q1 = frame1.rotation;
			q2 = frame2.rotation;

			float f = (frame - frame1.frame)/(frame2.frame - frame1.frame);

			// calc rotate
			rotateData->slerp( q1, q2, f );
	
			// set frame id
			*frameRotID = mid + 1;
			return true;
		}
	}
	*frameRotID = nRotFrames - 1;
	return false;
}

bool CColladaAnimation::getPositionFrameID( SColladaNodeAnim* frames, float frame, int *framePosID, core::vector3df  *positionData )
{
	int nPosFrames = frames->PositionKeys.size();
		
	int first = 0, last = nPosFrames - 2;
	int mid = 0;
		
	while (first <= last)
	{
		mid = (first + last) / 2;

		if ( frame > frames->PositionKeys[mid].frame && frame > frames->PositionKeys[mid + 1].frame )
			first = mid + 1;
		else if ( frame < frames->PositionKeys[mid].frame )
			last = mid - 1;
		else
		{			
			CGameColladaSceneNode::SPositionKey &frame1 = frames->PositionKeys[ mid ];
			CGameColladaSceneNode::SPositionKey &frame2 = frames->PositionKeys[ mid + 1 ];

			
			core::vector3df v1 = frame1.position;
			core::vector3df v2 = frame2.position;
					
			float f = (frame - frame1.frame)/(frame2.frame - frame1.frame);

			*positionData = v1 + (v2 - v1) * f;			

			// set frame id
			*framePosID = mid + 1;
			return true;
		}
	}

	*framePosID = nPosFrames - 1;
	return false;
}


// parseClipNode
// parse clip time node
void CColladaAnimation::parseClipNode( io::IXMLReader *xmlRead )
{
	char charBuffer[1024];

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				core::stringw nodeName = xmlRead->getNodeName();

				// read anamation clip
				if ( core::stringw(L"animation_clip") == nodeName )
				{
					const wchar_t *idClip = xmlRead->getAttributeValue(L"id");
					uiString::copy<char, const wchar_t>( charBuffer, idClip );

					float start = xmlRead->getAttributeValueAsFloat(L"start");
					float end	= xmlRead->getAttributeValueAsFloat(L"end");

					// add clip
					SColladaAnimClip *clip = new SColladaAnimClip();
					clip->animName = charBuffer;
					clip->time = start*k_defaultAnimFPS;
					clip->duration = (end - start)*k_defaultAnimFPS;

					m_colladaAnim.push_back( clip );
					m_animWithName[ charBuffer ] = m_colladaAnim.back();					
				}
				break;
			}
		case io::EXN_ELEMENT_END:
			{
				core::stringw nodeName = xmlRead->getNodeName();

				if ( core::stringw(L"library_animation_clips") == nodeName )
					return;
			}		
		}
	}
}

// parseAnimationNode
// parse anim node
void CColladaAnimation::parseAnimationNode( io::IXMLReader *xmlRead )
{	
	std::wstring idNodeName = xmlRead->getAttributeValue(L"id");
	
	std::wstring jointName;
	bool isRotation = false;
	bool isTranslate = false;

	int pos = idNodeName.find( L"-rotation" );
	if ( pos > 0 )
	{
		isRotation = true;
		jointName = idNodeName.substr(0, pos);
	}
	else
	{
		pos = idNodeName.find( L"-translation" );
		if ( pos > 0 )
		{
			isTranslate = true;
			jointName = idNodeName.substr(0, pos);
		}
	}

	if ( isRotation == false && isTranslate == false )
		return;

	char stringBuffer[1024];
	uiString::copy<char, const wchar_t>( stringBuffer, jointName.c_str() );
	
	// create anim node
	SColladaNodeAnim *nodeAnim = m_globalClip.getAnimOfSceneNode( stringBuffer );
	if ( nodeAnim == NULL )
	{
		nodeAnim = new SColladaNodeAnim();
		nodeAnim->sceneNodeName = stringBuffer;
	
		// add node anim
		m_globalClip.addNodeAnim( nodeAnim );	
	}

	int		readState = 0;
	
	float	*arrayTime = NULL;
	int		numArrayTime = 0;

	float	*arrayFloat = NULL;

	wstring arrayID;
	int count = 0;

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				core::stringw nodeName = xmlRead->getNodeName();

				if ( core::stringw(L"float_array") == nodeName )
				{
					arrayID = xmlRead->getAttributeValue(L"id");
					count = xmlRead->getAttributeValueAsInt(L"count");

					if ( (int)arrayID.find( L"-input-array" ) > 0 )
					{
						readState = 1;
						arrayTime = new float[count];						
					}
					else if ( (int)arrayID.find( L"-output-array" ) > 0 )
					{						
						readState = 2;
						arrayFloat = new float[count];
					}
				}
				else if ( core::stringw(L"accessor") == nodeName && arrayFloat != NULL )
				{
					int stride = xmlRead->getAttributeValueAsInt(L"stride");
					int nFrame = count/stride;

					float fvector[4];

					for ( int i = 0; i < nFrame; i++ )
					{						
						if ( isRotation && stride == 4 )
						{				
							if ( m_needFlip == true )
							{
								fvector[0] = arrayFloat[i*4];
								fvector[1] = arrayFloat[i*4 + 2];
								fvector[2] = arrayFloat[i*4 + 1];
								fvector[3] = arrayFloat[i*4 + 3];
							}
							else
							{
								fvector[0] = arrayFloat[i*4];
								fvector[1] = arrayFloat[i*4 + 1];
								fvector[2] = arrayFloat[i*4 + 2];
								fvector[3] = -arrayFloat[i*4 + 3];
							}

							CGameColladaSceneNode::SRotationKey key;
							key.frame = arrayTime[i]*k_defaultAnimFPS;
							key.rotation.fromAngleAxis(
									fvector[3] * core::DEGTORAD, 
									core::vector3df(fvector[0], fvector[1], fvector[2])
								);
							nodeAnim->RotationKeys.push_back(key);
						}
						else if ( isTranslate && stride == 3 )
						{							
							if ( m_needFlip == true )
							{
								fvector[0] = arrayFloat[i*3];
								fvector[1] = arrayFloat[i*3 + 2];
								fvector[2] = arrayFloat[i*3 + 1];									
							}
							else
							{
								fvector[0] = arrayFloat[i*3];
								fvector[1] = arrayFloat[i*3 + 1];
								fvector[2] = arrayFloat[i*3 + 2];	
							}

							CGameColladaSceneNode::SPositionKey key;
							key.frame = arrayTime[i]*k_defaultAnimFPS;
							key.position = core::vector3df(fvector[0], fvector[1], fvector[2] );
							nodeAnim->PositionKeys.push_back(key);
						}
						else
						{
							printf("Warning: May be not support some animation!\n");
						}

					}

					delete arrayTime;
					arrayTime = NULL;

					delete arrayFloat;
					arrayFloat = NULL;
				}
			}
		case io::EXN_ELEMENT_END:
			{
				core::stringw nodeName = xmlRead->getNodeName();
				if ( core::stringw(L"animation") == nodeName )
					return;
			}
			break;
		case io::EXN_TEXT:
			{
				if ( readState == 1 || readState == 2 )
				{
					const wchar_t *data = xmlRead->getNodeData();
					const wchar_t *p = data;
					
					char number[512];
					int i = 0;
					
					int		numArray = 0;
						
					while ( *p != NULL )
					{
						if ( *p == L' ' || *(p+1) == NULL  )
						{
							if ( *p == L' '  )
								number[i++] = NULL;
							else
							{
								number[i++] = (char) *p;
								number[i++] = NULL;
							}

							
							float f;
							sscanf(number,"%f", &f);																			
														
							if ( readState == 1 )
							{
								// read time
								arrayTime[numArrayTime++] = f;								
							}
							else if ( readState == 2 )
							{
								// add to list
								arrayFloat[numArray++] = f;
							}

							i = 0;
							number[i] = NULL;
						}
						else
							number[i++] = (char) *p;

						p++;
					
					}
				}
				readState = 0;
			}
			break;
		}
	}	

}

// clippingDaeAnim
// clip a long clip to many clip
void CColladaAnimation::clipDaeAnim()
{
	// loop all animation clip
	vector<SColladaAnimClip*>::iterator it = m_colladaAnim.begin(), end = m_colladaAnim.end();
	while ( it != end )
	{
		SColladaAnimClip* clip = (*it);

		// loop all scenenode with anim
		vector<SColladaNodeAnim*>::iterator iNodeAnim = m_globalClip.animInfo.begin(), 
			iNodeEnd = m_globalClip.animInfo.end();

		while ( iNodeAnim != iNodeEnd )
		{
			SColladaNodeAnim* nodeAnim = (*iNodeAnim);			

			float currentFrame	= 0;
			float frameBegin	= clip->time;
			float frameEnd		= clip->time + clip->duration;
			
			core::vector3df v1, v2;
			core::quaternion q1, q2;

			int beginID, endID;

			// clip rotation
			getRotationFrameID( nodeAnim, frameBegin,	&beginID,	&q1 );
			getRotationFrameID( nodeAnim, frameEnd,		&endID,		&q2 );
			
			// add clip
			if ( beginID >= 0 && endID >= 0 )
			{
				SColladaNodeAnim *newNodeAnim = new SColladaNodeAnim();
				newNodeAnim->sceneNodeName = nodeAnim->sceneNodeName;

				// add new node anim
				clip->addNodeAnim( newNodeAnim );

				if ( beginID == endID )
				{													
					CGameColladaSceneNode::SRotationKey rotKey;
					
					// frame1
					rotKey.frame = 0;
					rotKey.rotation = q1;
					newNodeAnim->RotationKeys.push_back( rotKey );

					// frame2
					rotKey.frame = frameEnd - frameBegin;
					rotKey.rotation = q2;
					newNodeAnim->RotationKeys.push_back( rotKey );				
				}
				else
				{
					for ( int i = beginID; i <= endID; i++ )
					{						
						CGameColladaSceneNode::SRotationKey		rotKey;

						if ( i == beginID )
						{
							currentFrame = 0;							
							rotKey.rotation = q1;
						}
						else if ( i == endID )
						{
							currentFrame = frameEnd - frameBegin;
							rotKey.rotation = q2;
						}
						else
						{
							CGameColladaSceneNode::SRotationKey&	animFrame = nodeAnim->RotationKeys[i];

							currentFrame =  animFrame.frame - frameBegin;
							rotKey.rotation = animFrame.rotation;
						}				
																		
						// add key frame
						rotKey.frame = currentFrame;
						newNodeAnim->RotationKeys.push_back( rotKey );	
					}
				}
			}

			// clip position
			getPositionFrameID( nodeAnim, frameBegin,	&beginID,	&v1 );
			getPositionFrameID( nodeAnim, frameEnd,		&endID,		&v2 );

			if ( beginID >= 0 && endID >= 0 )
			{
				SColladaNodeAnim *newNodeAnim = clip->getAnimOfSceneNode( nodeAnim->sceneNodeName.c_str() );
				if ( newNodeAnim == NULL )
				{
					newNodeAnim = new SColladaNodeAnim();
					newNodeAnim->sceneNodeName = nodeAnim->sceneNodeName;

					// add new node anim
					clip->addNodeAnim( newNodeAnim );
				}

				if ( beginID == endID )
				{													
					CGameColladaSceneNode::SPositionKey posKey;
					
					// frame1
					posKey.frame = 0;
					posKey.position = v1;
					newNodeAnim->PositionKeys.push_back( posKey );

					// frame2
					posKey.frame = frameEnd - frameBegin;
					posKey.position = v2;
					newNodeAnim->PositionKeys.push_back( posKey );				
				}
				else
				{
					for ( int i = beginID; i <= endID; i++ )
					{						
						CGameColladaSceneNode::SPositionKey		posKey;

						if ( i == beginID )
						{
							currentFrame = 0;							
							posKey.position = v1;
						}
						else if ( i == endID )
						{
							currentFrame = frameEnd - frameBegin;
							posKey.position = v2;
						}
						else
						{
							CGameColladaSceneNode::SPositionKey&	animFrame = nodeAnim->PositionKeys[i];

							currentFrame	=  animFrame.frame - frameBegin;
							posKey.position = animFrame.position;
						}				
																		
						// add key frame
						posKey.frame = currentFrame;
						newNodeAnim->PositionKeys.push_back( posKey );	
					}
				}
			}

			iNodeAnim++;
		}

		clip->time = 0.0f;
		it++;
	}
	m_globalClip.freeAllNodeAnim();
}

void CColladaAnimation::loadDae( char *lpFileName )
{
	IrrlichtDevice	*device = getIView()->getDevice();
	IVideoDriver	*driver = getIView()->getDriver();
	io::IFileSystem *fs = device->getFileSystem();

	io::IXMLReader *xmlRead = fs->createXMLReader( lpFileName );

	if ( xmlRead == NULL )
	{			
		xmlRead = fs->createXMLReader( getIView()->getPath(lpFileName) );

		if ( xmlRead == NULL )
			return;
	}	

	m_needFlip = false;
	bool readLUpAxis = false;

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				core::stringw nodeName = xmlRead->getNodeName();
				if ( core::stringw(L"library_animation_clips") == nodeName )
				{
					parseClipNode( xmlRead );
				}				
				else if ( core::stringw(L"animation")  == nodeName )
				{					
					parseAnimationNode( xmlRead );				
				}
				else if ( nodeName == L"up_axis" )
				{
					readLUpAxis = true;
				}							
			}
			break;
		case io::EXN_ELEMENT_END:
			{
			}
			break;
		case io::EXN_TEXT:
			{
				if ( readLUpAxis == true )
				{
					std::wstring text = xmlRead->getNodeData();
					if ( text == L"Z_UP" )
						m_needFlip = true;
					else
						m_needFlip = false;

				}
				readLUpAxis = false;
			}
			break;
		}
	}
	
	xmlRead->drop();

	// clip global animation to frame
	clipDaeAnim();
	
}

void CColladaAnimation::loadDotAnim( char *lpFileName )
{
	// todo load file
	io::IReadFile *file = getIView()->getSceneMgr()->getFileSystem()->createAndOpenFile( lpFileName );
	if ( file == NULL )
		return;

	// parse binary scene
	CBinaryUtils::getInstance()->loadAnim( file, this );

	// close file
	file->drop();
}

void CColladaAnimation::loadFile( char *lpFileName )
{
	char ext[10] = {0};
	uiString::getFileNameExt<char, char>( lpFileName, ext );
	uiString::toLower( ext );
	
	if ( strcmp(ext, "dae") == 0 )
		loadDae( lpFileName );
	else if ( strcmp(ext,"anim") == 0 )
		loadDotAnim( lpFileName );	
}


//////////////////////////////////////////////////////////
// CColladaAnimationFactory implement
//////////////////////////////////////////////////////////

CColladaAnimationFactory::CColladaAnimationFactory()
{
}

CColladaAnimationFactory::~CColladaAnimationFactory()
{
	freeAllAnimationPackage();
}

CColladaAnimation* CColladaAnimationFactory::loadAnimation( char *name, char *lpFileName )
{
	CColladaAnimation *colladaAnim = new CColladaAnimation();

	colladaAnim->loadFile( lpFileName );

	m_animPackage[name] = colladaAnim;
	return colladaAnim;
}

// freeAllAnimationPackage
// release all package animation
void CColladaAnimationFactory::freeAllAnimationPackage()
{
	map<std::string, CColladaAnimation*>::iterator i = m_animPackage.begin(), end = m_animPackage.end();
	while ( i != end )
	{
		delete (*i).second;
		i++;
	}
	m_animPackage.clear();
}


//////////////////////////////////////////////////////////
// CColladaMeshComponent implement
//////////////////////////////////////////////////////////

CColladaMeshComponent::CColladaMeshComponent( CGameObject *pObj )
	:IObjectComponent( pObj, (int)IObjectComponent::ColladaMesh )
{
	m_colladaNode = NULL;

	m_animeshFile = "";

	m_animFrames = 0.0f;	
	m_currentFrame = 0.0f;

	m_pauseAnimFrame = 0.0f;
	m_pauseAnim = false;

	m_currentAnim = NULL;
}

CColladaMeshComponent::~CColladaMeshComponent()
{

}


// init
// run when init object
void CColladaMeshComponent::initComponent()
{
}

// updateComponent
// update object by frame
void CColladaMeshComponent::updateComponent()
{
}

 
// saveData
// save data to serializable
void CColladaMeshComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	pObj->addPath("meshFile", m_animeshFile.c_str(), true);
	pObj->addString("defaultNode", m_defaultNode.c_str(), true);
}

// loadData
// load data to serializable
void CColladaMeshComponent::loadData( CSerializable* pObj )
{		
	pObj->nextRecord();

	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	// read mesh file
	char *lpFilename = pObj->readString();
	
	// read default node
	m_defaultNode = pObj->readString();

	// begin parse file
	loadFromFile( lpFilename );
}

// loadFromFile
// load anim object from dae file
void CColladaMeshComponent::loadFromFile( char *lpFilename )
{
	if ( m_animeshFile == lpFilename )
		return;

	m_animeshFile = lpFilename;

	// search in cache	
	CGameChildContainerSceneNode* cacheNode = CColladaCache::getNodeInCache( m_animeshFile );
	if ( cacheNode != NULL )
	{
		initFromNode( cacheNode );
		return;
	}

	char ext[10] = {0};
	uiString::getFileNameExt<char, char>( lpFilename, ext );
	uiString::toLower( ext );
	
	if ( strcmp(ext, "dae") == 0 )
		loadDae( lpFilename );
	else if ( strcmp(ext,"scene") == 0 )
		loadScene( lpFilename );
}


void CColladaMeshComponent::loadDae( char *lpFilename )
{
	IrrlichtDevice	*device = getIView()->getDevice();
	IVideoDriver	*driver = getIView()->getDriver();
	io::IFileSystem *fs = device->getFileSystem();

	io::IXMLReader *xmlRead = fs->createXMLReader( lpFilename );

	if ( xmlRead == NULL )
	{
		xmlRead = fs->createXMLReader( getIView()->getPath(lpFilename) );
		if ( xmlRead == NULL )
			return;
	}

	const std::wstring controllerSectionName(L"controller");
	const std::wstring effectSectionName(L"effect");
	const std::wstring geometrySectionName(L"geometry");	
	const std::wstring sceneSectionName(L"visual_scene");

	bool readLUpAxis = false;
	m_needFlip = false;

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				std::wstring nodeName = xmlRead->getNodeName();

				if ( nodeName == effectSectionName )
				{
					parseEffectNode( xmlRead );
				}
				else if ( nodeName == geometrySectionName )
				{
					parseGeometryNode( xmlRead );
				}
				else if ( nodeName == controllerSectionName )
				{				
					parseControllerNode( xmlRead );					
				}
				else if ( nodeName == sceneSectionName )
				{
					parseSceneNode( xmlRead );
				}
				else if ( nodeName == L"up_axis" )
				{
					readLUpAxis = true;
				}
				
			}
		case io::EXN_ELEMENT_END:
			{
			}
			break;
		case io::EXN_TEXT:
			{
				if ( readLUpAxis == true )
				{
					std::wstring text = xmlRead->getNodeData();
					if ( text == L"Z_UP" )
						m_needFlip = true;
					else
						m_needFlip = false;
				}
				readLUpAxis = false;
			}
			break;
		}
	}
	
	xmlRead->drop();

	// create scene node
	constructScene();

	// clean data
	cleanData();

	// cache node
	if ( m_colladaNode != NULL && CColladaCache::getNodeInCache(m_animeshFile) == NULL )
	{
		CColladaCache::cacheNode( m_animeshFile, m_colladaNode );
	}

#ifdef GSEDITOR
	if ( m_gameObject->m_node )
	{
		ISceneManager *smgr = getIView()->getSceneMgr();

		// add collision
		ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( m_gameObject->m_node );
		m_gameObject->m_node->setTriangleSelector(selector);
		selector->drop();
	}
#endif
}

void CColladaMeshComponent::loadScene( char *lpFilename )
{
	// todo load file
	io::IReadFile *file = getIView()->getSceneMgr()->getFileSystem()->createAndOpenFile( lpFilename );
	if ( file == NULL )
		return;

	// todo load scene
	ISceneManager *smgr = getIView()->getSceneMgr();

	m_colladaNode = new CGameChildContainerSceneNode
		(
			m_gameObject,
			m_gameObject->getParentSceneNode(),
			smgr,
			m_gameObject->getID()
		);
	
	m_gameObject->m_node = m_colladaNode;
	
	// parse binary scene
	CBinaryUtils::getInstance()->loadFile( file, m_gameObject );



	// close file
	file->drop();

	// cache node
	if ( m_colladaNode != NULL && CColladaCache::getNodeInCache(m_animeshFile) == NULL )
	{
		CColladaCache::cacheNode( m_animeshFile, m_colladaNode );
	}

#ifdef GSEDITOR
	if ( m_gameObject->m_node )
	{
		ISceneManager *smgr = getIView()->getSceneMgr();

		// add collision
		ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( m_gameObject->m_node );
		m_gameObject->m_node->setTriangleSelector(selector);
		selector->drop();
	}
#endif
}



// initFromNode
// init cache from node
struct SGroupNode
{
	ISceneNode *colladaParent;
	ISceneNode *initChild;

	SGroupNode( ISceneNode* p, ISceneNode* child )
	{
		colladaParent = p;
		initChild = child;
	}
};

void CColladaMeshComponent::initFromNode( CGameChildContainerSceneNode* node )
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	m_colladaNode = new CGameChildContainerSceneNode
		(
			m_gameObject,
			m_gameObject->getParentSceneNode(),
			smgr,
			m_gameObject->getID()
		);
	
	m_gameObject->m_node = m_colladaNode;

	std::queue< SGroupNode > queueNode;
	std::vector< CGameColladaMesh* > listSkinMesh;

	const core::list<ISceneNode*>& childs = node->getChildren();
	core::list<ISceneNode*>::ConstIterator it = childs.begin(), end = childs.end();
	while ( it != end )
	{
		queueNode.push( SGroupNode(m_colladaNode, (*it) ) );
		it++;
	}	
	
	while ( queueNode.size() )
	{
		SGroupNode& groupNode = queueNode.front();
		queueNode.pop();

		// clone new node
		CGameColladaSceneNode *newNode = (CGameColladaSceneNode*)groupNode.initChild->clone( groupNode.colladaParent, smgr );
		newNode->setComponent( this );

		m_colladaNode->addBoundingBoxOfChild( newNode );
				
		// store name this node
		std::string name = groupNode.initChild->getName();
		if ( name.length() > 0 )
			m_mapNode[ name ] = newNode;

		// store sid this node
		name = ((CGameColladaSceneNode*)groupNode.initChild)->getSIDName();
		if ( name.length() > 0 )
		{
			m_sidNode[name] = newNode;
			newNode->setSIDName( name );
		}

		// push skin mesh need update
		CGameColladaMesh* mesh = newNode->getMesh();
		if ( mesh && mesh->IsStaticMesh == false )
		{	
			mesh->Component = this;
			listSkinMesh.push_back( mesh );
		}
	

		const core::list<ISceneNode*>& childs = groupNode.initChild->getChildren();
		core::list<ISceneNode*>::ConstIterator it = childs.begin(), end = childs.end();
		while ( it != end )
		{
			queueNode.push( SGroupNode(newNode,(*it)) );
			it++;
		}

#ifdef GSANIMATION
		if ( newNode->getMesh() == NULL )
		{
			// add collision
			ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( newNode );
			newNode->setTriangleSelector(selector);
			selector->drop();
		}
#endif

		newNode->drop();	
	}

	// need map scenenode to joint
	vector<CGameColladaMesh*>::iterator iMesh = listSkinMesh.begin(), iMeshEnd = listSkinMesh.end();
	while ( iMesh != iMeshEnd )
	{
		(*iMesh)->updateJoint();
		iMesh++;
	}

#ifdef GSEDITOR
	if ( m_gameObject->m_node )
	{
		ISceneManager *smgr = getIView()->getSceneMgr();

		// add collision
		ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( m_gameObject->m_node );
		m_gameObject->m_node->setTriangleSelector(selector);
		selector->drop();
	}
#endif

}

// parseGeometryNode
// parse mesh data
void CColladaMeshComponent::parseGeometryNode( io::IXMLReader *xmlRead )
{
	const std::wstring geometrySectionName(L"geometry");
	const std::wstring sourceNode(L"source");
	const std::wstring verticesNode(L"vertices");
	const std::wstring trianglesNode(L"triangles");
	const std::wstring floatArrayNode(L"float_array");
	const std::wstring inputNode(L"input");
	const std::wstring accessorNode(L"accessor");

	SMeshParam mesh;
	
	mesh.Type = k_mesh;
	mesh.Name = readId( xmlRead );

	while(xmlRead->read())
	{
		if (xmlRead->getNodeType() == io::EXN_ELEMENT)
		{	
			std::wstring nodeName = xmlRead->getNodeName();

			//<source>
			if ( nodeName == sourceNode )
			{
				SBufferParam buffer;

				buffer.Name = readId( xmlRead );
				buffer.Type = k_positionBuffer;
				buffer.ArrayCount = 0;
				buffer.Strike = 0;
				buffer.FloatArray = NULL;

				while(xmlRead->read())
				{		
					if (xmlRead->getNodeType() == io::EXN_ELEMENT )
					{
						if ( xmlRead->getNodeName() == floatArrayNode )
						{
							buffer.ArrayCount = xmlRead->getAttributeValueAsInt(L"count");							
							buffer.FloatArray = new float[ buffer.ArrayCount ];

							readFloatsInsideElement( xmlRead, buffer.FloatArray, buffer.ArrayCount );							
						}
						else if ( xmlRead->getNodeName() == accessorNode )
						{
							buffer.Strike = xmlRead->getAttributeValueAsInt(L"stride");
						}
					}					
					else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
					{
						if ( xmlRead->getNodeName() == sourceNode )
							break;
					}
				}
				
				mesh.Buffers.push_back( buffer );
				
			}
			//<vertices>
			else if ( nodeName == verticesNode )
			{
				SVerticesParam	verticesParam;
				verticesParam.Name = readId( xmlRead );

				while(xmlRead->read())
				{		
					if (xmlRead->getNodeType() == io::EXN_ELEMENT )
					{
						if ( xmlRead->getNodeName() == inputNode )
						{
							std::wstring semantic = xmlRead->getAttributeValue(L"semantic");
							std::wstring source = xmlRead->getAttributeValue(L"source");
							source.erase( source.begin() );

							int bufferID = getBufferWithUri( source, &mesh );
														
							if ( bufferID != -1 )
							{
								SBufferParam *buffer = &mesh.Buffers[bufferID];

								if ( semantic == L"POSITION" )
								{
									buffer->Type = k_positionBuffer;
									verticesParam.PositionIndex = bufferID;
								}
								else if ( semantic == L"NORMAL" )
								{
									buffer->Type = k_normalBuffer;
									verticesParam.NormalIndex = bufferID;
								}
								else if ( semantic == L"TEXCOORD" )
								{
									buffer->Type = k_texCoordBuffer;
									
									if ( verticesParam.TexCoord1Index == -1 )
										verticesParam.TexCoord1Index = bufferID;
									else if ( verticesParam.TexCoord1Index != bufferID )
										verticesParam.TexCoord2Index = bufferID;
								}								
							}

						}
					}
					else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
					{
						if ( xmlRead->getNodeName() == verticesNode )
							break;
					}
				}

				// add vertex param
				mesh.Vertices.push_back( verticesParam );
			}
			//<triangles>
			else if ( nodeName == trianglesNode )
			{
				STrianglesParam triangle;

				// default is 1
				triangle.NumElementPerVertex = 1;

				triangle.NumPolygon = xmlRead->getAttributeValueAsInt(L"count");

				std::wstring materialName = xmlRead->getAttributeValue(L"material");
				triangle.EffectIndex = getEffectWithUri( materialName, m_listEffects );

				while(xmlRead->read())
				{							
					if (xmlRead->getNodeType() == io::EXN_ELEMENT )
					{
						if ( xmlRead->getNodeName() == inputNode )
						{
							std::wstring source = xmlRead->getAttributeValue(L"source");
							std::wstring semantic = xmlRead->getAttributeValue(L"semantic");
							
							int offset = 0;							
							offset = xmlRead->getAttributeValueAsInt(L"offset");

							source.erase( source.begin() );
														
							if ( semantic == L"NORMAL" )
							{
								int bufferID =	getBufferWithUri( source, &mesh );			
								mesh.Buffers[bufferID].Type = k_normalBuffer;
								
								if ( triangle.VerticesIndex != -1 )
								{
									mesh.Vertices[ triangle.VerticesIndex ].NormalIndex = bufferID;
									triangle.OffsetNormal = offset;
								}

								triangle.NumElementPerVertex++;
							}
							else if ( semantic == L"TEXCOORD" )
							{
								int bufferID =	getBufferWithUri( source, &mesh );
								mesh.Buffers[bufferID].Type = k_texCoordBuffer;
								if ( triangle.VerticesIndex != -1 )
								{		
									SVerticesParam &verticesParam = mesh.Vertices[ triangle.VerticesIndex ];
									if ( verticesParam.TexCoord1Index == -1 )
									{
										verticesParam.TexCoord1Index = bufferID;
										triangle.OffsetTexcoord1 = offset;
									}
									else if ( verticesParam.TexCoord1Index != bufferID )
									{
										verticesParam.TexCoord2Index = bufferID;
										triangle.OffsetTexcoord2 = offset;
									}
								}

								triangle.NumElementPerVertex++;
							}
							else if ( semantic == L"VERTEX" )
							{
								triangle.VerticesIndex = getVerticesWithUri( source, &mesh );
								triangle.OffsetVertex = offset;
							}
							else
							{
								triangle.NumElementPerVertex++;
							}
						}
						else if ( xmlRead->getNodeName() == std::wstring(L"p") && triangle.VerticesIndex != -1 )
						{
							triangle.IndexBuffer = new s32[triangle.NumPolygon * triangle.NumElementPerVertex * 3];
							readIntsInsideElement( xmlRead, triangle.IndexBuffer, triangle.NumPolygon * triangle.NumElementPerVertex *3 );
						}
					}					
					else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
					{
						if ( xmlRead->getNodeName() == trianglesNode )
							break;
					}				
				}

				// add triangles
				if ( triangle.IndexBuffer != NULL )
					mesh.Triangles.push_back( triangle );
			}
		}
		else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END)
		{		
			if ( xmlRead->getNodeName() == geometrySectionName )
				break;
		}
	}
	
	m_listMesh.push_back( mesh );

}

// parseControllersNode
// parse controllser
void CColladaMeshComponent::parseControllerNode( io::IXMLReader *xmlRead )
{	
	const std::wstring skinSectionName(L"skin");
	const std::wstring controllerSectionName(L"controller");

	std::wstring controllerName = xmlRead->getAttributeValue(L"id");	

	while(xmlRead->read())
	{							
		if (xmlRead->getNodeType() == io::EXN_ELEMENT )
		{
			std::wstring node = xmlRead->getNodeName();

			if ( node == skinSectionName )
			{
				SMeshParam *mesh = parseSkinNode( xmlRead );

				if ( mesh )
					mesh->ControllerName = controllerName;
			}
		}
		else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
		{
			if ( xmlRead->getNodeName() == controllerSectionName )
				break;
		}	
	}
}

// parseSkinNode
// parse skin data
SMeshParam* CColladaMeshComponent::parseSkinNode( io::IXMLReader *xmlRead )
{
	std::wstring source = xmlRead->getAttributeValue(L"source");
	source.erase( source.begin() );

	int meshID = getMeshWithUri( source, m_listMesh );
	if ( meshID == -1 )
		return NULL;
	
	SMeshParam *mesh = &m_listMesh[ meshID ];
	
	const std::wstring paramSectionName(L"param");
	const std::wstring skinSectionName(L"skin");
	const std::wstring jointsSectionName(L"joints");
	const std::wstring weightsSectionName(L"vertex_weights");
	const std::wstring nameArraySectionName(L"Name_array");
	const std::wstring floatArraySectionName(L"float_array");
	const std::wstring intArraySectionName(L"int_array");
	const std::wstring vcountNode(L"vcount");
	const std::wstring vNode(L"v");
	const std::wstring sourceNode(L"source");
	const std::wstring bindShapeMatrix(L"bind_shape_matrix");

	vector<std::wstring>	nameArray;

	int						numArray = 0;
	float					*jointArray = NULL;
	float					*transformArray = NULL;
	float					*weightArray = NULL;
	
	vector<s32>				&vCountArray	= mesh->JointVertexIndex;
	vector<s32>				vArray;

	while(xmlRead->read())
	{							
		if (xmlRead->getNodeType() == io::EXN_ELEMENT )
		{
			std::wstring node = xmlRead->getNodeName();
			
			if ( node == bindShapeMatrix )
			{
				float f[16];
				readFloatsInsideElement(  xmlRead, f, 16 );
				
				core::matrix4 mat;
				mat.setM( f );		
				if (m_needFlip)
				{
					core::matrix4 mat2(mat, core::matrix4::EM4CONST_TRANSPOSED);

					mat2[1]=mat[8];
					mat2[2]=mat[4];
					mat2[4]=mat[2];
					mat2[5]=mat[10];
					mat2[6]=mat[6];
					mat2[8]=mat[1];
					mat2[9]=mat[9];
					mat2[10]=mat[5];
					mat2[12]=mat[3];
					mat2[13]=mat[11];
					mat2[14]=mat[7];

					mesh->BindShapeMatrix = mat2;
				}
				else
					mesh->BindShapeMatrix = mat.getTransposed();				
			}
			// <source>
			else if ( node == sourceNode )
			{
				float *f = NULL;

				while(xmlRead->read())
				{
					if (xmlRead->getNodeType() == io::EXN_ELEMENT )
					{
						// <Name_array>
						if ( xmlRead->getNodeName() == nameArraySectionName )
						{
							int count = xmlRead->getAttributeValueAsInt(L"count");
							nameArray.resize( count );
							numArray = count;
							readStringInsideElement( xmlRead, nameArray );
						}
						// <float_array>
						else if ( xmlRead->getNodeName() == floatArraySectionName )
						{
							int count = xmlRead->getAttributeValueAsInt(L"count");
							numArray = count;
							f = new float[count];
							readFloatsInsideElement( xmlRead, f, count );
						}
						// <param>	inside <accessor>
						else if ( xmlRead->getNodeName() == paramSectionName )
						{
							std::wstring name = xmlRead->getAttributeValue(L"name");
							if ( name == L"TRANSFORM" )
								transformArray = f;
							else if ( name == L"WEIGHT" )
								weightArray = f;
							else
							{
								// delete float buffer on another accessor
								if ( f != NULL )
								{
									delete f;
									f = NULL;
								}
							}
						}
					}				
					else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
					{
						if ( xmlRead->getNodeName() == sourceNode )
							break;
					}
				}
			}
			// <vcount>
			else if ( node == vcountNode )
			{
				readIntsInsideElement( xmlRead, vCountArray );
			}
			// <vNode>
			else if ( node == vNode )
			{
				readIntsInsideElement( xmlRead, vArray );
			}
		}		
		else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
		{
			if ( xmlRead->getNodeName() == skinSectionName )
				break;
		}
	}
	
	mesh->Type = k_skinMesh;
	updateJointToMesh( mesh, nameArray, weightArray, transformArray, vCountArray, vArray, m_needFlip );

	if ( weightArray )
		delete weightArray;

	if ( transformArray )
		delete transformArray;

	return mesh;
}

// parseSceneNode
// parse scene data
void CColladaMeshComponent::parseSceneNode( io::IXMLReader *xmlRead )
{
	const std::wstring sceneSectionName(L"visual_scene");
	const std::wstring nodeSectionName(L"node");

	while(xmlRead->read())
	{			
		std::wstring nodeName =	xmlRead->getNodeName();

		if (xmlRead->getNodeType() == io::EXN_ELEMENT )
		{
			if ( nodeName == nodeSectionName )
			{
				SNodeParam* pNewNode = parseNode( xmlRead, NULL );
				m_listNode.push_back( pNewNode );
			}
		}
		else if ( xmlRead->getNodeType() == io::EXN_ELEMENT_END )
		{
			if ( nodeName == sceneSectionName )
				break;
		}
	}
}

// parseNode
// parse <node> element
SNodeParam* CColladaMeshComponent::parseNode( io::IXMLReader *xmlRead, SNodeParam* parent )
{	
	const std::wstring nodeSectionName(L"node");
	const std::wstring translateSectionName(L"translate");
	const std::wstring rotateSectionName(L"rotate");
	const std::wstring scaleSectionName(L"scale");

	const std::wstring instanceGeometrySectionName(L"instance_geometry");
	const std::wstring instanceControllerSectionName(L"instance_controller");

	SNodeParam *pNode = new SNodeParam();
	
	pNode->Name = L"";
	if ( xmlRead->getAttributeValue(L"id") )
		pNode->Name = xmlRead->getAttributeValue(L"id");

	if ( xmlRead->getAttributeValue(L"sid") )
		pNode->SID	= xmlRead->getAttributeValue(L"sid");	
	
	pNode->Type = L"NODE";
	if ( xmlRead->getAttributeValue(L"type") )
		pNode->Type = xmlRead->getAttributeValue(L"type");
	
	pNode->Parent = parent;
	if ( parent )
	{
		parent->Childs.push_back( pNode );
		pNode->ChildLevel = parent->ChildLevel+1;
	}
	else
		pNode->ChildLevel = 0;


	while(xmlRead->read())
	{
		if (xmlRead->getNodeType() == io::EXN_ELEMENT )
		{
			if ( xmlRead->getNodeName() == nodeSectionName )
			{
				parseNode( xmlRead, pNode );
			}
			else if ( xmlRead->getNodeName() == translateSectionName )
			{
				// mul translate
				pNode->Transform *= readTranslateNode(xmlRead, m_needFlip);
			}
			else if ( xmlRead->getNodeName() == rotateSectionName )
			{
				// mul rotate
				pNode->Transform *= readRotateNode(xmlRead, m_needFlip);
			}
			else if ( xmlRead->getNodeName() == scaleSectionName )
			{
				// mul scale
				pNode->Transform *= readScaleNode(xmlRead, m_needFlip);
			}			
			else if ( xmlRead->getNodeName() == instanceGeometrySectionName )
			{
				// <instance_geometry url="#MESHNAME">
				pNode->Instance = xmlRead->getAttributeValue(L"url");
				uriToId( pNode->Instance );
			}
			else if ( xmlRead->getNodeName() == instanceControllerSectionName )
			{
				// <instance_controller url="#MESHNAME">
				pNode->Instance = xmlRead->getAttributeValue(L"url");
				uriToId( pNode->Instance );
			}

		}
		else if ( xmlRead->getNodeType() == io::EXN_ELEMENT_END )
		{
			if ( xmlRead->getNodeName() == nodeSectionName )
				break;
		}
	}

	return pNode;
}


// parseEffectNode
// parse effect material node
void CColladaMeshComponent::parseEffectNode( io::IXMLReader *xmlRead, SEffect* effect )
{
	if ( effect == NULL )
	{		
		m_listEffects.push_back( SEffect() );
		effect = &m_listEffects.back();

		effect->Id = readId(xmlRead);
		effect->Transparency = 1.f;
		effect->Mat.Lighting = true;
		effect->Mat.NormalizeNormals = true;		
	}

	const std::wstring constantNode(L"constant");
	const std::wstring lambertNode(L"lambert");
	const std::wstring phongNode(L"phong");
	const std::wstring blinnNode(L"blinn");
	const std::wstring bumpNode(L"bump");
	const std::wstring emissionNode(L"emission");
	const std::wstring ambientNode(L"ambient");
	const std::wstring diffuseNode(L"diffuse");
	const std::wstring specularNode(L"specular");
	const std::wstring shininessNode(L"shininess");
	const std::wstring reflectiveNode(L"reflective");
	const std::wstring reflectivityNode(L"reflectivity");
	const std::wstring transparentNode(L"transparent");
	const std::wstring transparencyNode(L"transparency");
	const std::wstring indexOfRefractionNode(L"index_of_refraction");
	
	const std::wstring effectSectionName(L"effect");
	const std::wstring profileCOMMONSectionName(L"profile_COMMON");
	const std::wstring techniqueNodeName(L"technique");
	const std::wstring colorNodeName(L"color");
	const std::wstring floatNodeName(L"float");
	const std::wstring textureNodeName(L"texture");
	const std::wstring extraNodeName(L"extra");
	const std::wstring newParamName(L"newparam");

	const std::wstring initFromNode(L"init_from");
	const std::wstring sourceNode(L"source");

	bool hasBumpMapping = false;

	while(xmlRead->read())
	{
		if (xmlRead->getNodeType() == io::EXN_ELEMENT)
		{			
			std::wstring nodeName = xmlRead->getNodeName();
			
			if ( nodeName == profileCOMMONSectionName || nodeName == techniqueNodeName )
			{
				parseEffectNode(xmlRead,effect);
			}
			else if ( nodeName == newParamName )
			{
				int m_readState = 0;
					
				SEffectParam effectParam;
				effectParam.Name = xmlRead->getAttributeValue(L"sid");

				while(xmlRead->read())
				{
					std::wstring node = xmlRead->getNodeName();

					if (xmlRead->getNodeType() == io::EXN_ELEMENT)
					{
						if (node == initFromNode)
							m_readState = 1;
						else if ( node == sourceNode )
							m_readState = 2;

					}
					else if (xmlRead->getNodeType() == io::EXN_TEXT)
					{
						if ( m_readState == 1 )
							effectParam.InitFromTexture = xmlRead->getNodeData();
						else if ( m_readState == 2 )
							effectParam.Source = xmlRead->getNodeData();

						m_readState = 0;
					}
					else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END)
					{
						if ( nodeName == newParamName )
							break;
					}
				}

				// add to list effect params
				m_listEffectsParam.push_back( effectParam );

			}
			else if ( constantNode == nodeName || lambertNode == nodeName || phongNode == nodeName || blinnNode == nodeName || bumpNode == nodeName )
			{
				// set phong shading
				effect->Mat.setFlag(irr::video::EMF_GOURAUD_SHADING, phongNode == nodeName || blinnNode == nodeName);
								
				// read color type
				while(xmlRead->read())
				{
					if (xmlRead->getNodeType() == io::EXN_ELEMENT)
					{
						std::wstring node = xmlRead->getNodeName();

						if ( emissionNode == node || ambientNode == node || diffuseNode == node || 
							 specularNode == node || reflectiveNode == node || transparentNode == node )
						{
							// read color
							while(xmlRead->read())
							{
								if (xmlRead->getNodeType() == io::EXN_ELEMENT) 									
								{
									if ( colorNodeName == xmlRead->getNodeName() )
									{
										const video::SColorf colorf = readColorNode( xmlRead );
										const video::SColor color = colorf.toSColor();

										if (emissionNode == node)
											effect->Mat.EmissiveColor = color;
										else if (ambientNode == node)
											effect->Mat.AmbientColor = color;
										else if (diffuseNode == node)
											effect->Mat.DiffuseColor = color;
										else if (specularNode == node)
											effect->Mat.SpecularColor = color;
										else if (transparentNode == node)
											effect->Transparency = colorf.getAlpha();
									}
									else if ( textureNodeName == xmlRead->getNodeName() )
									{
										wstring tname = xmlRead->getAttributeValue(L"texture");
										
										if ( node == diffuseNode )
											effect->Mat.setTexture(0, getTextureFromImage( m_animeshFile, tname, m_listEffectsParam ));									
										else if ( node == ambientNode )
										{
											// ambient lightmap texture: todo later
										}
									}								
								}
								else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
								{
									if ( node == xmlRead->getNodeName() )
										break;
								}
							}
						}
						else if ( textureNodeName == node )
						{						
							if ( nodeName == bumpNode )
							{
								wstring tname = xmlRead->getAttributeValue(L"texture");
								// bump mapping texture: todo later
							}
						}
						else if (shininessNode == node || reflectivityNode == node || transparencyNode == node || indexOfRefractionNode == node )
						{
							// float or param types
							while( xmlRead->read() )
							{
								if (xmlRead->getNodeType() == io::EXN_ELEMENT )
								{
									if ( floatNodeName == xmlRead->getNodeName() )
									{
										f32 f = readFloatNode(xmlRead);
										if (shininessNode == node)
											effect->Mat.Shininess = f;
										else if (transparencyNode == node)
											effect->Transparency *= f;
									}
								}
								else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
								{
									if ( node == xmlRead->getNodeName() )
										break;
								}
							}
						}						
					}
					else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
					{
						if ( constantNode == xmlRead->getNodeName() || 
							lambertNode == xmlRead->getNodeName() ||
							phongNode == xmlRead->getNodeName() || 
							blinnNode == xmlRead->getNodeName() || 
							bumpNode == xmlRead->getNodeName() )
							break;
					}
				}
			}
			else if ( extraNodeName == nodeName )
				parseEffectNode(xmlRead, effect);
		}
		else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END)
		{
			std::wstring nodeName = xmlRead->getNodeName();

			if (effectSectionName == nodeName)
				break;
			else if (profileCOMMONSectionName == nodeName)
				break;
			else if (techniqueNodeName == nodeName)
				break;
			else if (extraNodeName == nodeName)
				break;
		}
	}


	if (effect->Mat.AmbientColor == video::SColor(0) && effect->Mat.DiffuseColor != video::SColor(0))
		effect->Mat.AmbientColor = effect->Mat.DiffuseColor;
	
	if (effect->Mat.DiffuseColor == video::SColor(0) && effect->Mat.AmbientColor != video::SColor(0))
		effect->Mat.DiffuseColor = effect->Mat.AmbientColor;

	if ( effect->Transparency != 0.0f && effect->Transparency != 1.0f )
	{
		effect->Mat.MaterialType = irr::video::EMT_TRANSPARENT_VERTEX_ALPHA;
		effect->Mat.ZWriteEnable = false;
	}	

	effect->Mat.setFlag(video::EMF_BILINEAR_FILTER, true);
	effect->Mat.setFlag(video::EMF_TRILINEAR_FILTER, true);
	effect->Mat.setFlag(video::EMF_ANISOTROPIC_FILTER, true);
	effect->Mat.Shininess = 0.0f;
}

// updateJointToMesh
// update joint
void CColladaMeshComponent::updateJointToMesh( SMeshParam *mesh, vector<wstring>& arrayName, float *arrayWeight, float *arrayTransform, vector<s32>& vCountArray, vector<s32>& vArray, bool flipZ )
{
	int numJoint = (int)arrayName.size();

	// get array float
	f32* f = (f32*)arrayTransform;

	for ( int i = 0; i < numJoint; i++ )
	{
		SJointParam newJoint;

		newJoint.Name = arrayName[i];

		core::matrix4 mat;
		mat.setM( f + i*16 );
		
		if (flipZ)
		{
			core::matrix4 mat2(mat, core::matrix4::EM4CONST_TRANSPOSED);

			mat2[1]=mat[8];
			mat2[2]=mat[4];
			mat2[4]=mat[2];
			mat2[5]=mat[10];
			mat2[6]=mat[6];
			mat2[8]=mat[1];
			mat2[9]=mat[9];
			mat2[10]=mat[5];
			mat2[12]=mat[3];
			mat2[13]=mat[11];
			mat2[14]=mat[7];

			newJoint.InvMatrix = mat2;
		}
		else
			newJoint.InvMatrix = mat.getTransposed();

		
		// add joint to controller
		mesh->Joints.push_back( newJoint );
	}
	
	// set vertex weight
	int nVertex = (int)vCountArray.size();
	int id = 0;
	
	for ( int i = 0; i < nVertex; i++ )
	{
		// num of bone in vertex
		int nBone = vCountArray[i];

		// loop on bone in vertex		
		for ( int iBone = 0; iBone < nBone; iBone++, id+=2 )
		{
			u32 boneId		= vArray[id];
			u32 weightId	= vArray[id + 1];
			f32 f			= arrayWeight[weightId];
			
			SWeightParam weightParam;

			weightParam.VertexID = i;
			weightParam.Strength = f;

			// add weight on bone
			mesh->Joints[boneId].Weights.push_back( weightParam );
			
			mesh->JointIndex.push_back( boneId );
			mesh->JointIndex.push_back( mesh->Joints[boneId].Weights.size() - 1 );
		}

	}
}

// setAnimation
// apply Animation to skin joint
void CColladaMeshComponent::setAnimation(const char *lpAnimName)
{
	if ( m_colladaNode == NULL )
		return;
	
	SColladaAnimClip *animClip = m_colladaAnimation->getAnim( lpAnimName );

	map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();

	// set begin frame
	setCurrentFrame(0);

	while ( i != end )
	{
		const std::string& nodeName = (*i).first;
		CGameColladaSceneNode* j = (*i).second;
				
		// clear old key frame
		j->clearAllKeyFrame();
				
		// get local matrix of skin joint
		const core::matrix4& mat =	j->getLocalMatrix();

		// todo add animation key
		SColladaNodeAnim* anim = animClip->getAnimOfSceneNode( nodeName.c_str() );

		if ( anim )
		{
			int nRotKey = anim->RotationKeys.size();
			for ( int i = 0; i < nRotKey; i++ )
			{
				j->RotationKeys.push_back( anim->RotationKeys[i] );	
			}

			int nPosKey = anim->PositionKeys.size();
			for ( int i = 0; i < nPosKey; i++ )
			{
				j->PositionKeys.push_back( anim->PositionKeys[i] );
			}

		}

		// next node
		i++;
	}	

}
void CColladaMeshComponent::constructScene()
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	// clear all map
	m_mapNode.clear();

	// create new scene node
	m_colladaNode = new CGameChildContainerSceneNode( 
			m_gameObject, 
			m_gameObject->getParentSceneNode(),
			smgr,
			m_gameObject->getID() 
		);
	

	// collada node
	m_gameObject->m_node = m_colladaNode;


	std::list<SNodeParam*>	stackScene;
	std::list<SNodeParam*>	listScene;

	int nNode = m_listNode.size();
	for ( int i = 0; i < nNode; i++ )
	{
		SNodeParam* root = m_listNode[i];
		stackScene.push_back( root );
	}

	while ( stackScene.size() )
	{
		SNodeParam *node = stackScene.back();
		
		// save to list bone prefab
		listScene.push_back( node );
		
		// to do create node
		char name[1024];
		uiString::copy<char, const wchar_t>( name, node->Name.c_str() );

		
		// create new scene node
		ISceneNode *parent = m_colladaNode;
		if ( node->Parent && node->Parent->SceneNode )
			parent = node->Parent->SceneNode;

		// crate new scene node
		CGameColladaSceneNode *colladaSceneNode = new CGameColladaSceneNode( parent, smgr, -1 );
		colladaSceneNode->setComponent( this );

		colladaSceneNode->setName( name );
		
		if ( node->Parent == NULL )
			colladaSceneNode->setRootColladaNode( true );
		else
			colladaSceneNode->setRootColladaNode( false );

		// get position from transform		
		node->SceneNode = colladaSceneNode;
		
		// store this node
		m_mapNode[name] = colladaSceneNode;

		// store joint sid node
		if ( node->SID.size() > 0 )
		{
			uiString::copy<char, const wchar_t>( name, node->SID.c_str() );
			m_sidNode[name] = colladaSceneNode;
			colladaSceneNode->setSIDName( name );
		}

		// set relative position		
		colladaSceneNode->setLocalMatrix( node->Transform );
		
		
		// construct geometry & controller in node
		if ( node->Instance.size() > 0 )
		{
			int meshID = getMeshWithUri( node->Instance, m_listMesh );
			if ( meshID == -1 )			
				meshID = getMeshWithControllerName( node->Instance, m_listMesh );

			if ( meshID != -1 )
			{
				SMeshParam *pMesh = &m_listMesh[meshID];

				CGameColladaMesh *pColladaMesh = new CGameColladaMesh();

				// need store component
				pColladaMesh->Component = this;
				pColladaMesh->BindShapeMatrix = pMesh->BindShapeMatrix;

				// add mesh buffer to skin mesh
				int nBuffer = pMesh->Triangles.size();

				for ( int i = 0; i < nBuffer; i++ )
				{
					STrianglesParam& tri = pMesh->Triangles[i];
					
					// create mesh buffer
					scene::SMeshBuffer* meshBuffer = new SMeshBuffer();
					constructMeshBuffer( pMesh, &tri, meshBuffer, m_needFlip );
					
					// add mesh buffer								
					pColladaMesh->addMeshBuffer( meshBuffer );
					pColladaMesh->recalculateBoundingBox();

					meshBuffer->drop();
				}
				
				if ( pMesh->Type == k_skinMesh )
					pColladaMesh->IsStaticMesh = false;
				else
				{
					pColladaMesh->IsStaticMesh = true;
					pColladaMesh->setHardwareMappingHint( EHM_STATIC );
				}

				// set mesh for scene node
				colladaSceneNode->setColladaMesh( pColladaMesh );
				
				if ( m_defaultNode == "" )
					m_defaultNode = colladaSceneNode->getName();

				pColladaMesh->drop();

			}

			// add update bounding box with this child
			((CGameChildContainerSceneNode*)m_gameObject->m_node)->addBoundingBoxOfChild( colladaSceneNode );

		}
#ifdef GSANIMATION
		else
		{
			// add collision
			ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( colladaSceneNode );
			colladaSceneNode->setTriangleSelector(selector);
			selector->drop();
		}
#endif

		// pop stack
		stackScene.erase( --stackScene.end() );
		
		// add child to continue loop
		int nChild = (int)node->Childs.size();
		for ( int i = 0; i < nChild; i++ )
		{			
			SNodeParam *childNode = node->Childs[i];

			// set parent
			childNode->Parent = node;
			stackScene.push_back( childNode );		
		}
	}

	// clear node data
	std::list<SNodeParam*>::iterator i = listScene.begin(), end = listScene.end();
	while ( i != end )
	{
		SNodeParam *pNode = (*i);
		
		// apply skin
		CGameColladaMesh* pMesh = pNode->SceneNode->getMesh();
		if ( pMesh != NULL && pMesh->IsStaticMesh == false )
		{
			int meshID = getMeshWithControllerName( pNode->Instance, m_listMesh );
			constructSkinMesh( &m_listMesh[meshID], pMesh);			
		}

		// clear node
		pNode->Joint = NULL;		
		pNode->SceneNode->drop();
		pNode->SceneNode = NULL;
		i++;
	}
	listScene.clear();
}


// constructMeshBuffer
// create mesh buffer
void CColladaMeshComponent::constructMeshBuffer( SMeshParam *mesh, STrianglesParam* tri, IMeshBuffer *buffer, bool flip )
{
	scene::SMeshBuffer *mbuffer = (scene::SMeshBuffer*) buffer;
					
	SVerticesParam	*vertices = &mesh->Vertices[ tri->VerticesIndex ];
	
	SEffect	*effect = NULL;
	SBufferParam *position	= NULL;
	SBufferParam *normal	= NULL;
	SBufferParam *texCoord1 = NULL;
	SBufferParam *texCoord2 = NULL;
		
	if ( tri->EffectIndex != -1 )
		effect = &m_listEffects[ tri->EffectIndex ];

	if ( vertices->PositionIndex == -1 )
		return;

	// position buffer
	position = &mesh->Buffers[vertices->PositionIndex];

	if ( vertices->NormalIndex != -1 )
		normal = &mesh->Buffers[vertices->NormalIndex];

	if ( vertices->TexCoord1Index != -1 )
		texCoord1 =	&mesh->Buffers[vertices->TexCoord1Index];

	if ( vertices->TexCoord2Index != -1 )
		texCoord2 =	&mesh->Buffers[vertices->TexCoord2Index];

	// alloc vertex
	int vertexCount = position->ArrayCount/3;
	mbuffer->Vertices.reallocate(mbuffer->Vertices.size() + vertexCount);
		
	core::array<u16> indices;

	for ( int i = 0; i < vertexCount; i++ )
	{
		video::S3DVertex vtx;
		vtx.Color = SColor(0xFFFFFFFF);

		int vIndex = i;
		int idx = vIndex * position->Strike;

		// set position
		vtx.Pos.X = position->FloatArray[idx+0];
		if (flip)
		{
			vtx.Pos.Z = position->FloatArray[idx+1];
			vtx.Pos.Y = position->FloatArray[idx+2];
		}
		else
		{
			vtx.Pos.Y = position->FloatArray[idx+1];
			vtx.Pos.Z = position->FloatArray[idx+2];
		}

		if ( tri->NumElementPerVertex == 1 )
		{
			// set normal
			if ( normal != NULL )
			{		
				int idx = vIndex * position->Strike;

				vtx.Normal.X = normal->FloatArray[idx+0];
				if (flip)
				{
					vtx.Normal.Z = normal->FloatArray[idx+1];
					vtx.Normal.Y = normal->FloatArray[idx+2];
				}
				else
				{
					vtx.Normal.Y = normal->FloatArray[idx+1];
					vtx.Normal.Z = normal->FloatArray[idx+2];
				}
			}

			// set texcoord
			if ( texCoord1 ) 
			{
				idx = vIndex * texCoord1->Strike;
				if ( texCoord1 != NULL )
				{
					vtx.TCoords.X = texCoord1->FloatArray[idx+0];
					vtx.TCoords.Y = texCoord1->FloatArray[idx+1];
				}
			}
		}
		mbuffer->Vertices.push_back( vtx );
	}
		
	indices.set_used( tri->NumPolygon * 3 );

	int totalElement = tri->NumPolygon * tri->NumElementPerVertex * 3;	
	int index = 0;
	for ( int i = 0; i < totalElement; i+= tri->NumElementPerVertex)
	{
		indices[index] = tri->IndexBuffer[i];

		if ( tri->NumElementPerVertex != 1 )
		{
			video::S3DVertex &vtx = mbuffer->Vertices[ tri->IndexBuffer[i] ];
			if ( normal != NULL )
			{
				int idx = tri->IndexBuffer[i + tri->OffsetNormal] * normal->Strike;
				if (flip)
				{
					vtx.Normal.Z = normal->FloatArray[idx+1];
					vtx.Normal.Y = normal->FloatArray[idx+2];
				}
				else
				{
					vtx.Normal.Y = normal->FloatArray[idx+1];
					vtx.Normal.Z = normal->FloatArray[idx+2];
				}
			}

			if ( texCoord1 ) 
			{
				int idx = tri->IndexBuffer[i + tri->OffsetTexcoord1] * texCoord1->Strike;
				vtx.TCoords.X = texCoord1->FloatArray[idx+0];
				vtx.TCoords.Y = texCoord1->FloatArray[idx+1];
			}
		}

		index++;
	}


	// it's just triangles
	u32 nPoly = indices.size()/3;

	for (u32 i = 0; i < nPoly; i++)
	{
		u32 ind = i * 3;

		if (flip)
		{
			mbuffer->Indices.push_back(indices[ind+2]);
			mbuffer->Indices.push_back(indices[ind+1]);
			mbuffer->Indices.push_back(indices[ind+0]);
		}
		else
		{
			mbuffer->Indices.push_back(indices[ind+0]);
			mbuffer->Indices.push_back(indices[ind+1]);
			mbuffer->Indices.push_back(indices[ind+2]);
		}
	}
	
	// set material
	if ( effect )
	{
		mbuffer->getMaterial() = effect->Mat;
		if ( effect->Mat.TextureLayer[0].Texture )
		{
			if ( effect->Mat.TextureLayer[0].Texture->hasAlpha() == true )
				mbuffer->getMaterial().MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		}
	}

	// calc normal vector
	if (!normal)
		getIView()->getSceneMgr()->getMeshManipulator()->recalculateNormals(mbuffer, true);

	// recalculate bounding box
	mbuffer->recalculateBoundingBox();
}

// constructSkinMesh
// apply bone to vertex
void CColladaMeshComponent::constructSkinMesh( SMeshParam *meshParam, CGameColladaMesh *mesh )
{	
	int nJoint = meshParam->Joints.size();

	char sidName[1024];

	for ( int i = 0; i < nJoint; i++ )
	{
		SJointParam& joint = meshParam->Joints[i];
		
		// add joint to mesh
		mesh->Joints.push_back( CGameColladaMesh::SJoint() );

		// get last joint
		CGameColladaMesh::SJoint& newJoint = mesh->Joints.getLast();

		video::S3DVertex *vertex = (video::S3DVertex*)mesh->getMeshBuffer(0)->getVertices();

		// set weight data
		int nWeight = joint.Weights.size();
		for ( int j = 0; j < nWeight; j++ )
		{						
			SWeightParam& weight = joint.Weights[j];

			newJoint.weights.push_back( CGameColladaMesh::SWeight() );
			CGameColladaMesh::SWeight &newWeight = newJoint.weights.getLast();

			newWeight.buffer_id = 0;
			newWeight.strength = weight.Strength;
			newWeight.vertex_id = weight.VertexID;
			
			newWeight.staticPos = vertex[ weight.VertexID ].Pos;
			newWeight.staticNormal = vertex[ weight.VertexID ].Normal;
		}
		
		// set node data
		uiString::copy<char, const wchar_t>( sidName, joint.Name.c_str() );
		newJoint.name = joint.Name;
		newJoint.node = m_sidNode[ std::string(sidName) ];

		// set invert matrix
		newJoint.globalInversedMatrix = joint.InvMatrix;
		newJoint.node->GlobalInversedMatrix = joint.InvMatrix;
	}
	
	// set joint index
	mesh->JointIndex.set_used( meshParam->JointIndex.size() );
	for ( int i = 0; i < (int)meshParam->JointIndex.size(); i++ )
		mesh->JointIndex[i] = meshParam->JointIndex[i];

	mesh->JointVertexIndex.set_used( meshParam->JointVertexIndex.size() );
	for ( int i = 0; i < (int)meshParam->JointVertexIndex.size(); i++ )
		mesh->JointVertexIndex[i] = meshParam->JointVertexIndex[i];
}

// cleanData
// free all data from parse dae
void CColladaMeshComponent::cleanData()
{
	m_listEffects.clear();
	
	m_listEffectsParam.clear();

	ArrayMeshParams::iterator i = m_listMesh.begin(), end = m_listMesh.end();
	while ( i != end )
	{
		SMeshParam& mesh = (*i);
		
		int n = (int)mesh.Buffers.size();
		int j = 0;

		for ( j = 0; j < n; j++ )
			delete mesh.Buffers[j].FloatArray;

		n = (int)mesh.Triangles.size();
		for ( j = 0; j < n; j++ )
			delete mesh.Triangles[j].IndexBuffer;

		i++;
	}
	m_listMesh.clear();

	for ( int j = 0; j < (int)m_listNode.size(); j++ )
	{
		SNodeParam* pNode = m_listNode[j];
		
		stack<SNodeParam*>	stackNode;
		stackNode.push( pNode );
		while ( stackNode.size() )
		{
			pNode = stackNode.top();
			stackNode.pop();

			for ( int i = 0; i < (int)pNode->Childs.size(); i++ )
				stackNode.push( pNode->Childs[i] );

			delete pNode;
		}		
	}
	m_listNode.clear();
}


// saveSceneToBinary
// save collada mesh info to binary file
void CColladaMeshComponent::saveSceneToBinary( const char *lpFileName )
{
	io::IFileSystem *fileSystem = getIView()->getSceneMgr()->getFileSystem();
	io::IWriteFile *file = fileSystem->createAndWriteFile( io::path(lpFileName) );
	
	CBinaryUtils::getInstance()->saveCollada( file, m_gameObject );
	
	file->drop();
}

// saveAnimToBinary
// save animation track to binary file
void CColladaMeshComponent::saveAnimToBinary( const char *lpFileName )
{
	io::IFileSystem *fileSystem = getIView()->getSceneMgr()->getFileSystem();
	io::IWriteFile *file = fileSystem->createAndWriteFile( io::path(lpFileName) );

	CBinaryUtils::getInstance()->saveAnimation( file, m_colladaAnimation );

	file->drop();
}



//////////////////////////////////////////////////////////
// c function implement
//////////////////////////////////////////////////////////

//! changes the XML URI into an internal id
void uriToId(std::wstring& str)
{	
	if (!str.size())
		return;

	if (str[0] == L'#')
		str.erase( str.begin() );
}


std::wstring readId(io::IXMLReader *xmlRead)
{
	std::wstring str = xmlRead->getAttributeValue(L"id");
	if (str.size()==0)
		str = xmlRead->getAttributeValue(L"name");
	
	return str;
}

void findNextNoneWhiteSpace(const c8** start)
{
	const c8* p = *start;

	while(*p && (*p==' ' || *p=='\n' || *p=='\r' || *p=='\t'))
		++p;

	*start = p;
}

inline f32 readFloat(const c8** p)
{
	f32 ftmp;
	*p = core::fast_atof_move(*p, ftmp);
	return ftmp;
}

void readFloatsInsideElement(io::IXMLReader* reader, f32* floats, u32 count)
{
	if (reader->isEmptyElement())
		return;

	while(reader->read())
	{
		if (reader->getNodeType() == io::EXN_TEXT)
		{
			// parse float data
			core::stringc data = reader->getNodeData();
			data.trim();
			const c8* p = &data[0];

			for (u32 i=0; i<count; ++i)
			{
				findNextNoneWhiteSpace(&p);
				if (*p)
					floats[i] = readFloat(&p);
				else
					floats[i] = 0.0f;
			}
		}
		else if (reader->getNodeType() == io::EXN_ELEMENT_END)
			break; // end parsing text
	}
}


//! reads ints from inside of xml element until end of xml element
void readIntsInsideElement(io::IXMLReader* reader, s32* ints, u32 count)
{
	if (reader->isEmptyElement())
		return;

	while(reader->read())
	{
		// TODO: check for comments inside the element
		// and ignore them.

		if (reader->getNodeType() == io::EXN_TEXT)
		{
			// parse float data
			core::stringc data = reader->getNodeData();
			data.trim();
			const c8* p = &data[0];

			for (u32 i=0; i<count; ++i)
			{
				findNextNoneWhiteSpace(&p);
				if (*p)
					ints[i] = (s32)readFloat(&p);
				else
					ints[i] = 0;
			}
		}
		else
		if (reader->getNodeType() == io::EXN_ELEMENT_END)
			break; // end parsing text
	}
}

//! reads ints from inside of xml element until end of xml element
void readIntsInsideElement(io::IXMLReader* reader, vector<s32>& arrayInt)
{
	if (reader->isEmptyElement())
		return;

	while(reader->read())
	{
		// TODO: check for comments inside the element
		// and ignore them.

		if (reader->getNodeType() == io::EXN_TEXT)
		{
			core::stringw data = reader->getNodeData();
			data.trim();
			
			wchar_t* p = &data[0];
			wchar_t* begin = &data[0];

			int value = 0;
					

			while ( *p )
			{
				while(*p && !(*p==L' ' || *p==L'\n' || *p==L'\r' || *p==L'\t'))
					++p;

				*p = NULL;

				if (*begin)
				{
					swscanf(begin,L"%d", &value);
					arrayInt.push_back( value );
				}				

				p++;
				begin = p;
			}

		}
		else
		if (reader->getNodeType() == io::EXN_ELEMENT_END)
			break; // end parsing text
	}
}				

void readStringInsideElement(io::IXMLReader* reader, vector<std::wstring>& arrayString)
{
	if (reader->isEmptyElement())
		return;

	while(reader->read())
	{
		// TODO: check for comments inside the element
		// and ignore them.

		if (reader->getNodeType() == io::EXN_TEXT)
		{
			core::stringw data = reader->getNodeData();
			data.trim();
			
			wchar_t* p = &data[0];
			wchar_t* begin = &data[0];

			for (u32 i=0; i< arrayString.size(); ++i)
			{
				while(*p && !(*p==L' ' || *p==L'\n' || *p==L'\r' || *p==L'\t'))
					++p;

				*p = NULL;

				if (*begin)
					arrayString[i] = std::wstring(begin);
				else
					arrayString[i] = std::wstring(L"");

				p++;
				begin = p;
			}
		}
		else
		if (reader->getNodeType() == io::EXN_ELEMENT_END)
			break; // end parsing text
	}
}

video::SColorf readColorNode(io::IXMLReader* reader)
{
	const core::stringc colorNodeName = "color";

	if (reader->getNodeType() == io::EXN_ELEMENT && colorNodeName == reader->getNodeName())
	{
		f32 color[4];
		readFloatsInsideElement(reader,color,4);
		return video::SColorf(color[0], color[1], color[2], color[3]);
	}

	return video::SColorf();
}

f32 readFloatNode(io::IXMLReader* reader)
{
	const core::stringc floatNodeName =        "float";
	
	f32 result = 0.0f;
	if (reader->getNodeType() == io::EXN_ELEMENT && floatNodeName == reader->getNodeName())
	{
		readFloatsInsideElement(reader,&result,1);
	}

	return result;
}

//! reads a <scale> element and its content and creates a matrix from it
core::matrix4 readScaleNode(io::IXMLReader* reader, bool flip)
{
	core::matrix4 mat;
	if (reader->isEmptyElement())
		return mat;

	f32 floats[3];
	readFloatsInsideElement(reader, floats, 3);

	if (flip)
		mat.setScale(core::vector3df(floats[0], floats[2], floats[1]));
	else
		mat.setScale(core::vector3df(floats[0], floats[1], floats[2]));

	return mat;
}

//! reads a <translate> element and its content and creates a matrix from it
core::matrix4 readTranslateNode(io::IXMLReader* reader, bool flip)
{
	core::matrix4 mat;
	if (reader->isEmptyElement())
		return mat;

	f32 floats[3];
	readFloatsInsideElement(reader, floats, 3);

	if (flip)
		mat.setTranslation(core::vector3df(floats[0], floats[2], floats[1]));
	else
		mat.setTranslation(core::vector3df(floats[0], floats[1], floats[2]));

	return mat;
}

//! reads a <rotate> element and its content and creates a matrix from it
core::matrix4 readRotateNode(io::IXMLReader* reader, bool flip)
{
	core::matrix4 mat;
	if (reader->isEmptyElement())
		return mat;

	f32 floats[4];
	readFloatsInsideElement(reader, floats, 4);

	if (!core::iszero(floats[3]))
	{
		core::quaternion q;
		if (flip)
			q.fromAngleAxis(floats[3]*core::DEGTORAD, core::vector3df(floats[0], floats[2], floats[1]));
		else
			q.fromAngleAxis(floats[3]*-core::DEGTORAD, core::vector3df(floats[0], floats[1], floats[2]));
		
		q.normalize();
		return q.getMatrix();
	}
	else
		return core::IdentityMatrix;
}

video::ITexture* getTextureFromImage( std::string& basePath, std::wstring& uri, ArrayEffectParams& listEffectParam)
{	
	int n = listEffectParam.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listEffectParam[i].Name == uri )
		{
			if ( listEffectParam[i].InitFromTexture.size() > 0 )
			{
				std::wstring textureName = listEffectParam[i].InitFromTexture;
				
				std::string path = basePath;

				int i = basePath.length() - 1;
				while ( i > 0 )
				{
					if ( basePath[i] == '\\' || basePath[i] == '/' )
					{
						path = basePath.substr(0, i + 1);
						break;
					}
					i--;
				}

				char textureNameA[1024];
				uiString::copy<char, const char>( textureNameA, path.c_str() );
				uiString::cat<char, const wchar_t>( textureNameA, textureName.c_str() );
					
				ITexture *tex =	getIView()->getDriver()->getTexture( textureNameA );

				if ( tex == NULL )
				{
					tex = getIView()->getDriver()->getTexture( getIView()->getPath(path) );
				}

				return tex;
			}
			else if ( listEffectParam[i].Source.size() > 0 )
				return getTextureFromImage( basePath,listEffectParam[i].Source, listEffectParam );

			return NULL;
		}
	}
	return NULL;
}

int getBufferWithUri( std::wstring& uri, SMeshParam* mesh )
{
	int n = mesh->Buffers.size();
	for ( int i =0; i < n; i++ )
	{
		if ( mesh->Buffers[i].Name == uri )
		{
			return i;
		}
	}
	return -1;
}

int getVerticesWithUri( std::wstring& uri, SMeshParam* mesh )
{
	int n = mesh->Vertices.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( mesh->Vertices[i].Name == uri )
		{
			return i;
		}
	}

	return -1;
}

int getEffectWithUri( std::wstring& uri, ArrayEffects& listEffectParam )
{
	std::wstring fxName = uri + L"-fx";

	int n = listEffectParam.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listEffectParam[i].Id == fxName )
		{
			return i;
		}
	}
	return -1;
}

int	getMeshWithUri( std::wstring& uri, ArrayMeshParams& listMeshParam )
{
	int n = listMeshParam.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listMeshParam[i].Name == uri )
		{
			return i;
		}
	}

	return -1;
}

int getMeshWithControllerName( std::wstring& controllerName, ArrayMeshParams& listMeshParam )
{
	int n = listMeshParam.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listMeshParam[i].ControllerName == controllerName )
		{
			return i;
		}
	}

	return -1;
}