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
#include "CDaeUtils.h"

#include "CGameCamera.h"
#ifdef GSGAMEPLAY
#include "gameLevel/CGameLevel.h"
#endif


#pragma region DAE_COLLADA_ANIMATION_IMPLEMENT

//////////////////////////////////////////////////////////
// CColladaAnimation implement
//////////////////////////////////////////////////////////

CColladaAnimation::CColladaAnimation()
{
}

CColladaAnimation::~CColladaAnimation()
{
	removeAll();
}

void CColladaAnimation::removeAll()
{
	// release all anim clip
	std::vector<SColladaAnimClip*>::iterator it = m_colladaAnim.begin(), end = m_colladaAnim.end();
	while ( it != end )
	{
		delete (*it);
		it++;
	}

	m_colladaAnim.clear();
	m_animWithName.clear();
}

void CColladaAnimation::loadDae( const char *lpFileName )
{		
	// parse binary scene
	CDaeUtils::getInstance()->setCurrentComponent(NULL);
	CDaeUtils::getInstance()->loadDaeAnim( lpFileName, this );
}

void CColladaAnimation::loadDotAnim( const char *lpFileName )
{
	// todo load file
	io::IReadFile *file = getIView()->getFileSystem()->createAndOpenFile( lpFileName );
	
	if ( file == NULL )
	{
		file = getIView()->getFileSystem()->createAndOpenFile( getIView()->getPath(lpFileName) );

		if ( file == NULL )
			return;
	}

	// parse binary scene
	CBinaryUtils::getInstance()->setCurrentComponent(NULL);
	CBinaryUtils::getInstance()->loadAnim( file, this );

	// close file
	file->drop();
}

void CColladaAnimation::loadFile( const char *lpFileName )
{
	char ext[10] = {0};
	uiString::getFileNameExt<const char, char>( lpFileName, ext );
	uiString::toLower( ext );
	
	m_animFileName	= lpFileName;

	if ( strcmp(ext, "dae") == 0 )
		loadDae( lpFileName );
	else if ( strcmp(ext,"anim") == 0 )
		loadDotAnim( lpFileName );	
}

void CColladaAnimation::removeClip( const std::string& clipName )
{
	SColladaAnimClip *clip = m_animWithName[ clipName ];
	if ( clip == NULL )
		return;

	std::vector<SColladaAnimClip*>::iterator it = m_colladaAnim.begin(), end = m_colladaAnim.end();
	while ( it != end )
	{
		if ( (*it) == clip )
		{
			delete (*it);
			m_colladaAnim.erase(it);
			m_animWithName[ clipName ] = NULL;
			return;
		}
		it++;
	}
}

void CColladaAnimation::cloneAnim( const char *lpAnimName, const char *lpNewAnimName )
{
	SColladaAnimClip *clip = m_animWithName[ lpAnimName ];
	if ( clip == NULL )
		return;

	SColladaAnimClip *newClip = new SColladaAnimClip();
	newClip->animName	= lpNewAnimName;
	newClip->time		= clip->time;
	newClip->duration	= clip->duration;
	newClip->loop		= clip->loop;

	for ( int i = 0, nNodeAnims = (int)clip->animInfo.size(); i < nNodeAnims; i++ )
	{
		SColladaNodeAnim* nodeAnim = new SColladaNodeAnim();
		*nodeAnim = *clip->animInfo[i];
		newClip->animInfo.push_back( nodeAnim );
	}

	removeClip( lpNewAnimName );
	addClip( newClip );
}

void CColladaAnimation::createSynchronizedAnim( const char *lpNewName, const char *lpAnim1, const char *lpAnim2, float w )
{
	SColladaAnimClip *clip1 = m_animWithName[ lpAnim1 ];
	SColladaAnimClip *clip2 = m_animWithName[ lpAnim2 ];
	if ( clip1 == NULL || clip2 == NULL || clip1->duration == 0.0f || clip2->duration == 0.0f )
		return;

	w = core::clamp<float>( w, 0.0f, 1.0f );
	float time1 = clip1->duration;
	float time2 = clip2->duration;
	 	
	// interpolate speed ratio 2 channel
	float time = time1 + ( ( time2 - time1 ) * w );

	SColladaAnimClip *newClip = new SColladaAnimClip();
	newClip->animName	= lpNewName;
	newClip->time		= clip1->time;
	newClip->duration	= time;
	newClip->loop		= true;

	for ( int i = 0, n = (int)clip1->animInfo.size(); i < n; i++ )
	{
		SColladaNodeAnim* nodeAnim1 = clip1->animInfo[i];
		SColladaNodeAnim* nodeAnim2 = clip2->getAnimOfSceneNode( nodeAnim1->sceneNodeName.c_str() );
		
		SColladaNodeAnim* newNode = new SColladaNodeAnim();

		// set name
		newNode->sceneNodeName = nodeAnim1->sceneNodeName;

		// set node info
		if ( nodeAnim2 == NULL )
		{
			float ratio = time/time1;

			for ( int key = 0, nKey = nodeAnim1->PositionKeys.size(); key < nKey; key++ )
			{
				newNode->PositionKeys.push_back( nodeAnim1->PositionKeys[key] );
				newNode->PositionKeys[key].frame = newNode->PositionKeys[key].frame * ratio;
			}

			for ( int key = 0, nKey = nodeAnim1->RotationKeys.size(); key < nKey; key++ )
			{
				newNode->RotationKeys.push_back( nodeAnim1->RotationKeys[key] );
				newNode->RotationKeys[key].frame = newNode->RotationKeys[key].frame * ratio;
			}
		}
		else
		{
			CGameAnimationTrack track1, track2;
			core::vector3df		pos1, pos2, scale1, scale2;
			core::quaternion	rot1, rot2;			

			track1.RotationKeys = nodeAnim1->RotationKeys;
			track1.PositionKeys = nodeAnim1->PositionKeys;
			track1.ScaleKeys = nodeAnim1->ScaleKeys;

			track2.RotationKeys = nodeAnim2->RotationKeys;
			track2.PositionKeys = nodeAnim2->PositionKeys;
			track2.ScaleKeys = nodeAnim2->ScaleKeys;

			CGameAnimationTrack::SRotationKey	rotKey;
			CGameAnimationTrack::SPositionKey	posKey;
			CGameAnimationTrack::SScaleKey		scaleKey;

			float frameSplit = 2.0f;
			float f = 0.0f;
			float ratio = time/time1;

			while ( 1 )
			{				
				track1.getFrameData(f, pos1, scale1, rot1, core::IdentityMatrix);
				track2.getFrameData(time2*f/time1, pos2, scale2, rot2, core::IdentityMatrix);
				
				posKey.frame = f*ratio;
				posKey.position.interpolate(pos1, pos2, w );
				newNode->PositionKeys.push_back( posKey );

				scaleKey.frame = posKey.frame;
				scaleKey.scale.interpolate(scale1, scale2, w);
				newNode->ScaleKeys.push_back( scaleKey );

				rotKey.frame = posKey.frame;
				rotKey.rotation.slerp( rot1, rot2, w);				
				newNode->RotationKeys.push_back( rotKey );				

				if ( f == time1 )
					break;

				f += frameSplit;

				if ( f > time1 )
					f = time1;
			}		
		}

		// add node anim
		newClip->addNodeAnim( newNode );
	}
	
	removeClip( lpNewName );
	addClip( newClip );
}

#pragma endregion


#pragma region DAE_COLLADA_ANIMATION_FACTORY_IMPLEMENT
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

CColladaAnimation* CColladaAnimationFactory::loadAnimation( const char *name, const char *lpFileName )
{
	CColladaAnimation *colladaAnim = m_animPackage[name];
	
	if ( colladaAnim == NULL )
		colladaAnim = new CColladaAnimation();

	colladaAnim->loadFile( lpFileName );

	m_animPackage[name] = colladaAnim;
	return colladaAnim;
}

// freeAllAnimationPackage
// release all package animation
void CColladaAnimationFactory::freeAllAnimationPackage()
{
	std::map<std::string, CColladaAnimation*>::iterator i = m_animPackage.begin(), end = m_animPackage.end();
	while ( i != end )
	{
		delete (*i).second;
		i++;
	}
	m_animPackage.clear();
}

#pragma endregion



//////////////////////////////////////////////////////////
// CColladaCache implement
//////////////////////////////////////////////////////////

std::map<std::string, CGameColladaContainerSceneNode*>	CColladaCache::s_nodeCache;


//////////////////////////////////////////////////////////
// CColladaMeshComponent implement
//////////////////////////////////////////////////////////

CColladaMeshComponent::CColladaMeshComponent( CGameObject *pObj )
	:IObjectComponent( pObj, (int)IObjectComponent::ColladaMesh )
{
	m_colladaNode = NULL;

	m_animeshFile = "";

	m_currentAnim = NULL;
	m_colladaAnimation = NULL;	
}

CColladaMeshComponent::~CColladaMeshComponent()
{
	if ( m_colladaNode )
		m_colladaNode->setOwner( NULL );
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
	// update gameplay lod geometry 
	updateLod();
}

 
// saveData
// save data to serializable
void CColladaMeshComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	pObj->addPath("meshFile", m_animeshFile.c_str(), true);
	pObj->addString("defaultNode", m_defaultNodeString.c_str(), true);
}

// loadData
// load data to serializable
void CColladaMeshComponent::loadData( CSerializable* pObj )
{		
	pObj->nextRecord();	
	
	// clear array node list
	m_defaultNode.clear();

	// read mesh file
	char *lpFilename = pObj->readString();
	
	// read default node
	m_defaultNodeString = pObj->readString();

	// begin parse file
	loadFromFile( lpFilename );

	// init default node
	CGameColladaSceneNode* defaultNode = m_mapNode[m_defaultNodeString];
	if ( defaultNode )
		m_defaultNode.push_back( defaultNode );
}


#pragma region PARSE_DAE_DATA

// loadFromFile
// load anim object from dae file
void CColladaMeshComponent::loadFromFile( const char *lpFilename )
{
	if ( m_animeshFile == lpFilename )
		return;

	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	m_animeshFile = lpFilename;

	// search in cache	
	CGameColladaContainerSceneNode* cacheNode = CColladaCache::getNodeInCache( m_animeshFile );
	if ( cacheNode != NULL )
	{
		initFromNode( cacheNode );
		return;
	}

	char ext[10] = {0};
	uiString::getFileNameExt<const char, char>( lpFilename, ext );
	uiString::toLower( ext );
	
	if ( strcmp(ext, "dae") == 0 )
		loadDae( lpFilename );
	else if ( strcmp(ext,"scene") == 0 )
		loadScene( lpFilename );
}


void CColladaMeshComponent::loadDae( const char *lpFilename )
{	
	// todo load scene
	ISceneManager *smgr = getIView()->getSceneMgr();

	m_colladaNode = new CGameColladaContainerSceneNode
		(
			m_gameObject,
			m_gameObject->getParentSceneNode(),
			smgr,
			(s32)m_gameObject->getID()
		);
	
	m_gameObject->m_node = m_colladaNode;
	
	// parse binary scene
	CDaeUtils::getInstance()->setCurrentComponent(this);
	CDaeUtils::getInstance()->loadFile( lpFilename, m_gameObject );
	

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



void CColladaMeshComponent::loadScene( const char *lpFilename )
{
	// todo load file
	io::IReadFile *file = getIView()->getFileSystem()->createAndOpenFile( lpFilename );
	if ( file == NULL )
	{
		file = getIView()->getFileSystem()->createAndOpenFile( getIView()->getPath(lpFilename) );

		if ( file == NULL )
			return;
	}

	// todo load scene
	ISceneManager *smgr = getIView()->getSceneMgr();

	m_colladaNode = new CGameColladaContainerSceneNode
		(
			m_gameObject,
			m_gameObject->getParentSceneNode(),
			smgr,
			(s32)m_gameObject->getID()
		);
	
	m_gameObject->m_node = m_colladaNode;
	
	// parse binary scene
	CBinaryUtils::getInstance()->setCurrentComponent(this);
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

void CColladaMeshComponent::initFromNode( CGameColladaContainerSceneNode* node )
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	m_colladaNode = new CGameColladaContainerSceneNode
		(
			m_gameObject,
			m_gameObject->getParentSceneNode(),
			smgr,
			(s32)m_gameObject->getID()
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
		SGroupNode groupNode = queueNode.front();
		queueNode.pop();

		// clone new node
		CGameColladaSceneNode *newNode = (CGameColladaSceneNode*)groupNode.initChild->clone( groupNode.colladaParent, smgr );
		newNode->setComponent( this );
						
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
	
		if ( mesh )
			m_colladaNode->addBoundingMeshNode( newNode );

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
	std::vector<CGameColladaMesh*>::iterator iMesh = listSkinMesh.begin(), iMeshEnd = listSkinMesh.end();
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


// getChildsOfSceneNode
// find all childs of scene node
void CColladaMeshComponent::getChildsOfSceneNode( const char *name, std::vector<CGameColladaSceneNode*>& listChilds )
{
	CGameColladaSceneNode *rootNode = getSceneNode( name );
	if ( rootNode == NULL )
		return;
	
	std::stack<ISceneNode*>	stackNodes;
	stackNodes.push( rootNode );

	while ( stackNodes.size() )
	{
		// get current node
		ISceneNode* node = stackNodes.top();
		stackNodes.pop();

		// add childs to list
		if ( node != rootNode )
			listChilds.push_back( (CGameColladaSceneNode*)node );

		// continue get all childs node
		const core::list<ISceneNode*> childs = node->getChildren();
		core::list<ISceneNode*>::ConstIterator i = childs.begin(), end = childs.end();
		while ( i != end )
		{
			stackNodes.push( (*i) );
			i++;
		}

	}

}

// setCrossFadeAnimation
// crossfade current animation to new animation
void CColladaMeshComponent::setCrossFadeAnimation(const char *lpAnimName, int trackChannel, float nFrames, bool loop, int animLayer)
{
	if ( m_colladaNode == NULL )
		return;

	SColladaAnimClip *animClip = m_colladaAnimation->getAnim( lpAnimName );
	if ( animClip == NULL )
		return;
	
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		const std::string& nodeName = (*i).first;
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}

		// get current frame data
		core::vector3df		currentPos;
		core::vector3df		currentScale;
		core::quaternion	currentRotate;
		CGameAnimationTrack *track = j->getAnimation(animLayer)->getTrack(trackChannel);

		track->getFrameData( track->getCurrentFrame(), currentPos, currentScale, currentRotate, j->LocalMatrix );

		CGameAnimationTrack::SRotationKey rot;
		rot.frame		= 0;
		rot.rotation	= currentRotate;

		CGameAnimationTrack::SPositionKey pos;
		pos.frame		= 0;
		pos.position	= currentPos;

        CGameAnimationTrack::SScaleKey scale;
        scale.frame     = 0;
        scale.scale     = currentScale;        
        
		// clear old key frame		
		track->clearAllKeyFrame();
		track->setLoop( false );

		// todo add animation key
		SColladaNodeAnim* anim = animClip->getAnimOfSceneNode( nodeName.c_str() );

		if ( anim )
		{			
			int nRotKey = anim->RotationKeys.size();
			if ( nRotKey > 0 )
			{
				// set current animation
				track->RotationKeys.push_back( rot );

				rot.frame		= nFrames;
				rot.rotation	= anim->RotationKeys[0].rotation;
				track->RotationKeys.push_back( rot );

				// set cross animation
				track->CrossAnimRotationKeys.set_used( nRotKey );
				for ( int i = 0; i < nRotKey; i++ )
				{
					track->CrossAnimRotationKeys[i] = anim->RotationKeys[i];
				}
			}

			int nPosKey = anim->PositionKeys.size();
			if ( nPosKey > 0 )
			{
				// set current animation
				track->PositionKeys.push_back( pos );

				pos.frame		= nFrames;
				pos.position	= anim->PositionKeys[0].position;
				track->PositionKeys.push_back( pos );

				// set cross animation
				track->CrossAnimPositionKeys.set_used( nPosKey );
				for ( int i = 0; i < nPosKey; i++ )
				{
					track->CrossAnimPositionKeys[i] = anim->PositionKeys[i];
				}
			}
            
            int nScaleKey = anim->ScaleKeys.size();
            if ( nScaleKey > 0 )
            {
                // set current animation
                track->ScaleKeys.push_back( scale );
                
                scale.frame     = nFrames;
                scale.scale     = anim->ScaleKeys[0].scale;
                
                // set cross animation
                track->CrossAnimScaleKeys.set_used( nScaleKey );
                for ( int i = 0; i < nScaleKey; i++ )
				{
					track->CrossAnimScaleKeys[i] = anim->ScaleKeys[i];
				}
            }


			// enable cross animation
			track->enableCrossAnimation( loop );
		}

		i++;
	}
			
	// current anim crossfade clip
	m_currentAnim = animClip;
	m_currentAnim->loop = false;	// hardcode to turn of loop on current anim!
}


void CColladaMeshComponent::setCrossFadeAnimationToLayer(const char *lpAnimName, float nFrames, bool loop, int fromAnimLayer, int fromChannel, int toAnimLayer, int toChannel)
{
	if ( m_colladaNode == NULL )
		return;
    
	SColladaAnimClip *animClip = m_colladaAnimation->getAnim( lpAnimName );
	if ( animClip == NULL )
		return;
	
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		const std::string& nodeName = (*i).first;
		CGameColladaSceneNode* j = (*i).second;
        
		if ( j == NULL )
		{
			i++;
			continue;
		}
        
		// get current frame data
		core::vector3df		currentPos;
		core::vector3df		currentScale;
		core::quaternion	currentRotate;
        
		CGameAnimationTrack *srcTrack = j->getAnimation(fromAnimLayer)->getTrack(fromChannel);
        
		srcTrack->getFrameData( srcTrack->getCurrentFrame(), currentPos, currentScale, currentRotate, j->LocalMatrix );
        
		CGameAnimationTrack::SRotationKey rot;
		rot.frame		= 0;
		rot.rotation	= currentRotate;
        
		CGameAnimationTrack::SPositionKey pos;
		pos.frame		= 0;
		pos.position	= currentPos;
        
        CGameAnimationTrack::SScaleKey scale;
        scale.frame     = 0;
        scale.scale     = currentScale;
        
		// clear old key frame
		CGameAnimationTrack *track = j->getAnimation(toAnimLayer)->getTrack(toChannel);        
		track->clearAllKeyFrame();
		track->setLoop( false );
        
		// todo add animation key
		SColladaNodeAnim* anim = animClip->getAnimOfSceneNode( nodeName.c_str() );
        
		if ( anim )
		{			
			int nRotKey = anim->RotationKeys.size();
			if ( nRotKey > 0 )
			{
				// set current animation
				track->RotationKeys.push_back( rot );
                
				rot.frame		= nFrames;
				rot.rotation	= anim->RotationKeys[0].rotation;
				track->RotationKeys.push_back( rot );
                
				// set cross animation
				track->CrossAnimRotationKeys.set_used( nRotKey );
				for ( int i = 0; i < nRotKey; i++ )
				{
					track->CrossAnimRotationKeys[i] = anim->RotationKeys[i];
				}
			}
            
			int nPosKey = anim->PositionKeys.size();
			if ( nPosKey > 0 )
			{
				// set current animation
				track->PositionKeys.push_back( pos );
                
				pos.frame		= nFrames;
				pos.position	= anim->PositionKeys[0].position;
				track->PositionKeys.push_back( pos );
                
				// set cross animation
				track->CrossAnimPositionKeys.set_used( nPosKey );
				for ( int i = 0; i < nPosKey; i++ )
				{
					track->CrossAnimPositionKeys[i] = anim->PositionKeys[i];
				}
			}
            
            int nScaleKey = anim->ScaleKeys.size();
            if ( nScaleKey > 0 )
            {
                // set current animation
                track->ScaleKeys.push_back( scale );
                
                scale.frame     = nFrames;
                scale.scale     = anim->ScaleKeys[0].scale;
                
                // set cross animation
                track->CrossAnimScaleKeys.set_used( nScaleKey );
                for ( int i = 0; i < nScaleKey; i++ )
				{
					track->CrossAnimScaleKeys[i] = anim->ScaleKeys[i];
				}
            }
            
			// enable cross animation
			track->enableCrossAnimation( loop );
		}
        
		i++;
	}
    
	// current anim crossfade clip
	m_currentAnim = animClip;
	m_currentAnim->loop = false;	// hardcode to turn of loop on current anim!    
}


// setAnimation
// apply Animation to skin joint
void CColladaMeshComponent::setAnimation(const char *lpAnimName, int trackChannel, bool loop, int animLayer)
{
	if ( m_colladaNode == NULL )
		return;
	
	SColladaAnimClip *animClip = m_colladaAnimation->getAnim( lpAnimName );
	if ( animClip == NULL )
		return;
	
	animClip->loop = loop;

	// set current anim clip
	m_currentAnim = animClip;

	// loop all node
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		const std::string& nodeName = (*i).first;
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		// clear old key frame
		CGameAnimationTrack *track = j->getAnimation(animLayer)->getTrack( trackChannel );
		track->clearAllKeyFrame();
						
		// todo add animation key
		SColladaNodeAnim* anim = animClip->getAnimOfSceneNode( nodeName.c_str() );

		if ( anim )
		{
			// add rotation key
			int nRotKey = anim->RotationKeys.size();
			if ( nRotKey > 0 )
			{
				track->RotationKeys.set_used( nRotKey );
				for ( int i = 0; i < nRotKey; i++ )
				{
					track->RotationKeys[i] = anim->RotationKeys[i];
				}
			}

			// add position key
			int nPosKey = anim->PositionKeys.size();
			if ( nPosKey > 0 )
			{
				track->PositionKeys.set_used( nPosKey );
				for ( int i = 0; i < nPosKey; i++ )
				{
					track->PositionKeys[i] = anim->PositionKeys[i];
				}
			}

			// add scale key
			int nScaleKey = anim->ScaleKeys.size();
			if ( nScaleKey > 0 )
			{
				track->ScaleKeys.set_used( nScaleKey );
				for ( int i = 0; i < nScaleKey; i++ )
				{
					track->ScaleKeys[i] = anim->ScaleKeys[i];
				}
			}

			track->setLoop( loop );
		}

		// next node
		i++;
	}
}

// synchronizedByTimeScale
// sync 2 animation
void CColladaMeshComponent::synchronizedByTimeScale(int animLayer, float speedRatio)
{
	// loop all node
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		j->getAnimation(animLayer)->synchronizedByTimeScale(speedRatio);

		i++;
	}
}

// pauseAtFrame
// pause anim at frame id
void CColladaMeshComponent::pauseAtFrame( float frame, int trackChannel, int animLayer)
{
	// loop all node
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation(animLayer)->getTrack( trackChannel );

		track->setCurrentFrame( frame );
		track->setPause( true );

		i++;
	}
}
	
// setAnimationLoop
// loop the anim
void CColladaMeshComponent::setAnimationLoop( bool loop, int trackChannel, int animLayer)
{
    // loop all node
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
        
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation(animLayer)->getTrack(trackChannel);
		track->setLoop(loop);
        
		i++;
	}
}

// isEndAnimation
// check is end of anim
bool CColladaMeshComponent::isEndAnimation(int trackChannel, int animLayer)
{
    // loop all node
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
        
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation(animLayer)->getTrack(trackChannel);
        if ( track->isEndTrack() == false )
            return false;
        
		i++;
	}
    
    return true;
}

// getCurrentFrame
// get current frame of anim
float CColladaMeshComponent::getCurrentFrame(int trackChannel, int animLayer)
{
	// loop all node
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation(animLayer)->getTrack(trackChannel);

		if ( track->getTotalFrame() > 0 )
			return track->getCurrentFrame();

		i++;
	}

	return 0.0f;
}

// get current anim speed
float CColladaMeshComponent::getAnimSpeed(int trackChannel, int animLayer)
{
	// loop all node
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation(animLayer)->getTrack( trackChannel );

		if ( track->getTotalFrame() > 0 )
			return track->getSpeedRatio();

		i++;
	}
	return 1.0f;
}

// setCurrentFrame	
void CColladaMeshComponent::setCurrentFrame(float f, int trackChannel, int animLayer)
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation(animLayer)->getTrack( trackChannel );

		track->setCurrentFrame(f);
		i++;
	}
}

// resumeAnim
// resume animation
void CColladaMeshComponent::resumeAnim(int trackChannel, int animLayer)
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation(animLayer)->getTrack( trackChannel );

		track->setPause( false );
		i++;
	}
}

// setAnimWeight
void CColladaMeshComponent::setAnimWeight(float w, int trackChannel, int animLayer)
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
		if ( j != NULL )
			j->getAnimation(animLayer)->getTrack(trackChannel)->setAnimWeight(w);
		i++;
	}
}

// setAnimSpeed
void CColladaMeshComponent::setAnimSpeed(float s, int trackChannel, int animLayer)
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
		if ( j != NULL )
			j->getAnimation(animLayer)->getTrack(trackChannel)->setSpeedRatio(s);
		i++;
	}
}

void CColladaMeshComponent::enableAnimTrackChannel( int trackChannel, bool b, int animLayer)
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
		if ( j != NULL )
			j->getAnimation(animLayer)->getTrack(trackChannel)->setEnable( b );		
		i++;
	}
}

void CColladaMeshComponent::onlyEnableAnimTrackChannel( int trackChannel, int animLayer)
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
		if ( j != NULL )
			j->getAnimation(animLayer)->onlyEnableTrack( trackChannel );
		i++;
	}
}

// setAnimLayer
// set animation layer
void CColladaMeshComponent::setAnimLayer(int layer)
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
		if ( j != NULL )
		{
			j->setAnimLayer(layer);
		}
        i++;
    }
}

void CColladaMeshComponent::setAnimLayer( std::vector<CGameColladaSceneNode*>& listNodes, int animLayer)
{
	std::vector<CGameColladaSceneNode*>::iterator i = listNodes.begin(), end = listNodes.end();
    while ( i != end )
    {
        (*i)->setAnimLayer(animLayer);
        i++;
    }
}

void CColladaMeshComponent::enableAnimLayer(int layer, bool b)
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
		if ( j != NULL )
		{
			j->getAnimation(layer)->setEnable(b);
		}
        i++;
    }
}

void CColladaMeshComponent::enableAnimLayer(std::vector<CGameColladaSceneNode*>& listNodes, int layer, bool b)
{
	std::vector<CGameColladaSceneNode*>::iterator i = listNodes.begin(), end = listNodes.end();
    while ( i != end )
    {
        (*i)->getAnimation(layer)->setEnable(b);
        i++;
    }
}


// isPauseAnim
// check anim is pause??
bool CColladaMeshComponent::isPauseAnim(int trackChanel, int animLayer)
{
    std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
		if ( j != NULL )
		{
			if ( j->getAnimation(animLayer)->getTrack(trackChanel)->isPause() == true )
				return true;
		}
        i++;
    }
	return false;
}


// setNodeReferenceByAnimLayer
void CColladaMeshComponent::setNodeReferenceByAnimLayer(CGameColladaSceneNode* node, CGameColladaSceneNode* parent)
{
    m_colladaNode->addNodeReferenceByAnimLayer(node, parent);
}

void CColladaMeshComponent::removeNodeReferenceByAnimLayer(CGameColladaSceneNode* node, CGameColladaSceneNode* parent)
{
    m_colladaNode->removeNodeReferenceByAnimLayer(node, parent);
}

// saveSceneToBinary
// save collada mesh info to binary file
void CColladaMeshComponent::saveSceneToBinary( const char *lpFileName )
{
	io::IFileSystem *fileSystem = getIView()->getSceneMgr()->getFileSystem();
	io::IWriteFile *file = fileSystem->createAndWriteFile( io::path(lpFileName) );
	
	if ( file == NULL )
		file = fileSystem->createAndWriteFile( io::path( getIView()->getPath(lpFileName) ));

	if ( file )
	{
		CBinaryUtils::getInstance()->setCurrentComponent(this);
		CBinaryUtils::getInstance()->saveCollada( file, m_gameObject );	
		file->drop();
	}
}

// saveAnimToBinary
// save animation track to binary file
void CColladaMeshComponent::saveAnimToBinary( const char *lpFileName )
{
	io::IFileSystem *fileSystem = getIView()->getSceneMgr()->getFileSystem();
	io::IWriteFile *file = fileSystem->createAndWriteFile( io::path(lpFileName) );

	if ( file == NULL )
		file = fileSystem->createAndWriteFile( io::path( getIView()->getPath(lpFileName) ));

	if ( file )
	{
		CBinaryUtils::getInstance()->setCurrentComponent(this);
		CBinaryUtils::getInstance()->saveAnimation( file, m_colladaAnimation );
		file->drop();
	}
}

//////////////////////////////////////////////////////////
// CColladaMeshComponent gameplay function implement
//////////////////////////////////////////////////////////


// sortLodData
// sort lod data by distance
void CColladaMeshComponent::sortLodData()
{
	int len = (int)m_colladaLodNode.size();
	
	SColladaLodNode obj;
		
	for (int i = 1; i <= len - 1; i++)
	{
		obj = m_colladaLodNode[i];
		
		int j = i - 1;
		int done = 0;

		do
		{
			if ( m_colladaLodNode[j].distance > obj.distance )
			{
				m_colladaLodNode[j + 1] = m_colladaLodNode[j];

				j = j - 1;
				if ( j < 0 )
					done = 1;
			}
			else
				done = 1;
		}
		while (done == 0);
		
		m_colladaLodNode[j + 1] = obj;		
	}
}

// clearLodData
// clear all lod data
void CColladaMeshComponent::clearLodData()
{
	CColladaMeshComponent* collada = (CColladaMeshComponent*) m_gameObject->getComponent( IObjectComponent::ColladaMesh );
	
	if ( collada != NULL )
	{
		for ( int i = 0, nLod = (int)m_colladaLodNode.size(); i < nLod; i++ )
		{
			SColladaLodNode& lod = m_colladaLodNode[i];
			CGameColladaSceneNode* node = collada->getSceneNode( lod.node.c_str() );
			node->setVisible( true );
		}
	}

	m_colladaLodNode.clear();
}

// addLodData
// set lod data
void CColladaMeshComponent::addLodData( float distanceCam, const char* node )
{
	SColladaLodNode lod;
	lod.distance = distanceCam;
	lod.node = node;

	m_colladaLodNode.push_back( lod );
	sortLodData();
}

// updateLod
// update lod mesh
void CColladaMeshComponent::updateLod()
{
	if ( m_colladaLodNode.size() == 0 )
		return;

#ifdef GSGAMEPLAY
	CGameCamera* cam = CGameLevel::getCurrentLevel()->getCamera();
#else
	CGameCamera* cam = getIView()->getActiveCamera();
#endif

	if ( cam == NULL )
		return;

	float distance = m_gameObject->getPosition().getDistanceFrom( cam->getPosition() );
	bool hasLod = false;

	for ( int nLod = (int)m_colladaLodNode.size(), i = 0; i < nLod; i++ )
	{
		SColladaLodNode& lod = m_colladaLodNode[i];
		CGameColladaSceneNode* node = getSceneNode( lod.node.c_str() );
		
		if ( node )
		{
			if ( distance < lod.distance )				
			{
				if ( hasLod == false )
				{
					// we find the current lod
					node->setVisible( true );
					hasLod = true;
				}
				else
				{
					node->setVisible( false );
				}
			}
			else
				node->setVisible( false );
		}

	}
}