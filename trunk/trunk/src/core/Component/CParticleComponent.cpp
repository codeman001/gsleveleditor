#include "stdafx.h"
#include "CParticleComponent.h"
#include "IView.h"

#include "core/TextureManager/CTextureManager.h"

#include "CGameParticleContainerSceneNode.h"

std::map<std::string, SParticleCacheItem*>	CParticleCache::s_nodeCache;


CParticleComponent::CParticleComponent(CGameObject *pObj)
	:IObjectComponent(pObj, IObjectComponent::Particle)
{
}

CParticleComponent::~CParticleComponent()
{
}
	
// init
// run when init object
void CParticleComponent::initComponent()
{
}

// update
// run when update per frame
void CParticleComponent::updateComponent()
{
	if ( m_stopEmitter == true )
	{
		std::vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
		while (i != end )
		{
			SParticleInfo& p = (*i);
			p.ps->getEmitter()->setMinParticlesPerSecond( 0 );
			p.ps->getEmitter()->setMaxParticlesPerSecond( 0 );
			i++;
		}
		return;
	}

	if ( m_time == 0 )
	{				
		m_time = (long) irr::os::Timer::getTime();
		m_totalLifeTime = -1;

		fixParticlePosition();
		return;
	}
	
	long currentTime = (long) irr::os::Timer::getTime();
	long deltaTime = currentTime - m_time;

	bool isStop = true;
	
	std::vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
	while (i != end )
	{
		SParticleInfo& p = (*i);
		
		if ( p.startTime > deltaTime )
		{
			// wait to start time
			p.ps->getEmitter()->setMinParticlesPerSecond( 0 );
			p.ps->getEmitter()->setMaxParticlesPerSecond( 0 );
			isStop = false;
		}
		else if ( p.lifeTime > 0 && p.startTime + p.lifeTime < deltaTime )
		{
			// end life time
			p.ps->getEmitter()->setMinParticlesPerSecond( 0 );
			p.ps->getEmitter()->setMaxParticlesPerSecond( 0 );
			p.isStop = true;
			isStop = true;			
		}
		else
		{
			// update particle
			p.ps->getEmitter()->setMinParticlesPerSecond( p.minParticle );
			p.ps->getEmitter()->setMaxParticlesPerSecond( p.maxParticle );
			p.isStop = false;
			isStop = false;
		}

		i++;
	}

	if ( isStop == true )
	{
		// calc total time
		if ( m_totalLifeTime == -1 )		
			m_totalLifeTime = currentTime - m_time;
		
#ifdef GSEDITOR
		// need reset after 1s
		if ( currentTime - m_time > m_totalLifeTime + 1000 )
			m_time = 0;
#else
		m_stopEmitter = true;
#endif
	}

}

// saveData
// save data to serializable
void CParticleComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );
	pObj->addPath( "particleXml", m_xmlPath.c_str() );
}

// loadData
// load data to serializable
void CParticleComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
	m_xmlPath = pObj->readString();
	initParticle();
}

// initParticle
// init with file name
void CParticleComponent::initParticle(const char* lpFileName)
{
	m_xmlPath = lpFileName;
	initParticle();
}

// initParticle
// create empty particle
void CParticleComponent::initParticle()
{
	// release scenenode
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	ISceneManager *smgr = getIView()->getSceneMgr();

	// create an container node
	m_gameObject->m_node = new CGameParticleContainerSceneNode
		( 
			m_gameObject, 
			m_gameObject->getParentSceneNode(), 
			smgr, 
			(s32)m_gameObject->getID() 
		);

	m_gameObject->setLighting( false );

#ifdef GSEDITOR
	// add collision
	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox(m_gameObject->m_node);
	m_gameObject->m_node->setTriangleSelector(selector);
	selector->drop();
#endif

	SParticleCacheItem *cache = CParticleCache::getNodeInCache( m_xmlPath );

	if ( cache == NULL )
	{
		// load particle from file
		loadXML( m_xmlPath.c_str() );
	}
	else
	{
		// list particle
		m_arrayParticle = cache->arrayParticle;

		io::IFileSystem *fs = getIView()->getDevice()->getFileSystem();
		io::IAttributes *attrb = fs->createEmptyAttributes();	

		// init particle
		std::vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
		while ( i != end )
		{
			// get particle info from cache
			IParticleSystemSceneNode *cache = (*i).ps;
			cache->serializeAttributes(attrb);		

			// create new particle
			(*i).ps = smgr->addParticleSystemSceneNode( false, m_gameObject->m_node );
			(*i).ps->deserializeAttributes(attrb);

			// apply material
			(*i).ps->setMaterialTexture(0, cache->getMaterial(0).getTexture(0) );
			(*i).ps->setMaterialFlag(	video::EMF_LIGHTING, false );
			(*i).ps->setMaterialType(	cache->getMaterial(0).MaterialType );

			attrb->clear();
			i++;
		}

		attrb->drop();
	}

	m_time = 0;
	m_totalLifeTime = -1;
	m_stopEmitter = false;
}

// createParticle
// create an empty particle
IParticleSystemSceneNode* CParticleComponent::createParticle()
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	// create an empty particle
	IParticleSystemSceneNode *ps = smgr->addParticleSystemSceneNode( false, m_gameObject->m_node );

	SParticleInfo psInfo;
	psInfo.ps = ps;

	m_arrayParticle.push_back( psInfo );

	// reset particle
	m_time = 0;

	return ps;
}

// removeParticle
// remove particle at i
void CParticleComponent::removeParticle( int i )
{
	IParticleSystemSceneNode* ps = getParticle(i);
	if ( ps )	
	{
		ps->remove();
		m_arrayParticle.erase( m_arrayParticle.begin() + i );
	}
}

// removeAllParticle
// remove all particle
void CParticleComponent::removeAllParticle()
{
	std::vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();

	while ( i != end )
	{
		(*i).ps->remove();
		i++;
	}
	
	m_arrayParticle.clear();
}

// getParticleInfo
// get particle info
SParticleInfo* CParticleComponent::getParticleInfo( IParticleSystemSceneNode *ps )
{
	std::vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
	while (i != end )
	{
		SParticleInfo& p = (*i);
		
		if ( p.ps == ps )		
			return &p;		

		i++;
	}
	return NULL;
}

// getParticleInfo
// get the id of particle
int CParticleComponent::getParticleID( IParticleSystemSceneNode *ps )
{
	int id = 0;
	std::vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
	while (i != end )
	{
		SParticleInfo& p = (*i);
		
		if ( p.ps == ps )		
			return id;

		id++;
		i++;
	}
	return -1;
}

// saveXML
// save list particle to xml file
void CParticleComponent::saveXML( const char *lpFileName )
{
	IrrlichtDevice *device = getIView()->getDevice();
	io::IFileSystem *fs = device->getFileSystem();

	io::IWriteFile *file = fs->createAndWriteFile( lpFileName );
	if ( file == NULL )
		return;

	const unsigned short UTF16_LE = 0xFEFF;
	file->write( (void*)&UTF16_LE, sizeof(unsigned short) );

	io::IXMLWriter *xmlWrite = fs->createXMLWriter( file );
	if ( xmlWrite == NULL )
		return;	
	
	xmlWrite->writeElement(L"particles");
	xmlWrite->writeLineBreak();

	io::IAttributes *attrb = fs->createEmptyAttributes();
	
	wchar_t lpTemp[1024];	

	std::vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
	while (i != end )
	{
		SParticleInfo& p = (*i);	
		
		core::array<core::stringw>	propertyName;
		core::array<core::stringw>	propertyValue;
					
		p.ps->getEmitter()->setMinParticlesPerSecond( p.minParticle );
		p.ps->getEmitter()->setMaxParticlesPerSecond( p.maxParticle );

		p.ps->serializeAttributes( attrb );

		// particle name
		uiString::copy<wchar_t,const irr::c8>(lpTemp, p.ps->getName());
		propertyName.push_back( L"name" );
		propertyValue.push_back( core::stringw(lpTemp) );

		// particle texture
		uiString::copy<wchar_t,const char>(lpTemp, p.texture.c_str());
		propertyName.push_back( L"texture" );
		propertyValue.push_back( core::stringw(lpTemp) );
		
		// particle transparent
		if ( p.additiveTrans )
			uiString::copy<wchar_t, const wchar_t>( lpTemp, L"true" );
		else
			uiString::copy<wchar_t, const wchar_t>( lpTemp, L"false" );
		propertyName.push_back( L"additiveTrans" );
		propertyValue.push_back( core::stringw(lpTemp) );

		// time start
		uiString::format<wchar_t>( lpTemp, L"%ld", p.startTime);
		propertyName.push_back( L"startTime" );
		propertyValue.push_back( core::stringw(lpTemp) );

		// life time
		uiString::format<wchar_t>( lpTemp, L"%ld", p.lifeTime);
		propertyName.push_back( L"lifeTime" );
		propertyValue.push_back( core::stringw(lpTemp) );

		xmlWrite->writeElement(L"ps",false, propertyName, propertyValue);
		xmlWrite->writeLineBreak();

		// write <attribute>
		attrb->write( xmlWrite );

		xmlWrite->writeClosingTag(L"ps");
		xmlWrite->writeLineBreak();

		attrb->clear();

		i++;
	}

	xmlWrite->writeClosingTag(L"particles");
	
	xmlWrite->drop();
	file->drop();
}

// loadXML
// load particle from xml file
void CParticleComponent::loadXML( const char *lpFileName )
{
	removeAllParticle();
	
	IrrlichtDevice *device = getIView()->getDevice();
	IVideoDriver *driver = getIView()->getDriver();
	io::IFileSystem *fs = device->getFileSystem();

	io::IXMLReader *xmlRead = fs->createXMLReader( lpFileName );
	if ( xmlRead == NULL )
	{
		xmlRead = fs->createXMLReader( getIView()->getPath(lpFileName) );
		if ( xmlRead == NULL )
			return;	
	}
    
	IParticleSystemSceneNode *particle = NULL;
	SParticleInfo*	particleInfo = NULL;
    
    char attribValueA[1024] = {0};
    
	while ( xmlRead->read() )
	{
		if (core::stringw(L"particles") == xmlRead->getNodeName())
		{
			while( xmlRead->read() )
			{
				switch (xmlRead->getNodeType())
				{
				case io::EXN_ELEMENT_END:
					break;
				case io::EXN_ELEMENT:
					{
						if (core::stringw(L"ps") == xmlRead->getNodeName())
						{
							particle = createParticle();							
							particleInfo = CParticleComponent::getParticleInfo( particle );

							const wchar_t *attribValue = xmlRead->getAttributeValue(L"name");
							uiString::convertUnicodeToUTF8(attribValue, attribValueA );
							particle->setName( attribValue );

                            
							attribValue = xmlRead->getAttributeValue(L"texture");
							uiString::convertUnicodeToUTF8(attribValue, attribValueA );

#ifdef _IRR_COMPILE_WITH_OGLES2_
                            char fileName[512];
                            
                            // replace texture is PVR
                            uiString::getFileNameNoExt<char,char>(attribValueA, fileName);
                            uiString::copy<char,char>(attribValueA,fileName);
                            uiString::cat<char, const char>(attribValueA,".pvr");
#endif
                            
							// set texture
							particleInfo->texture = attribValueA;

							ITexture *pTex = driver->getTexture( attribValueA );
							if ( pTex == NULL )
								pTex = driver->getTexture( getIView()->getPath(attribValueA) );
							if ( pTex == NULL )
							{
								// search with xml file
								char path[512];
								uiString::getFolderPath<const char, char>(lpFileName, path);
								uiString::cat<char,const char>(path, "/");
								uiString::cat<char,char>(path, attribValueA);

								pTex = driver->getTexture( getIView()->getPath(path) );
							}

							if ( pTex )
								CTextureManager::getInstance()->registerTexture(pTex);

							attribValue = xmlRead->getAttributeValue(L"additiveTrans");
							uiString::convertUnicodeToUTF8(attribValue, attribValueA );
                            
							if ( strcmp( attribValueA, "true") == 0 )
								particleInfo->additiveTrans = true;
							else
								particleInfo->additiveTrans = false;
							

							if ( pTex && particle )
							{
								particle->setMaterialTexture(0, pTex );
								particle->setMaterialFlag(video::EMF_LIGHTING, false);

								if ( particleInfo->additiveTrans )
									particle->setMaterialType( video::EMT_TRANSPARENT_ADD_COLOR );
								else
									particle->setMaterialType( video::EMT_TRANSPARENT_ALPHA_CHANNEL );
							}

							// set start time
							attribValue = xmlRead->getAttributeValue(L"startTime");
							uiString::convertUnicodeToUTF8(attribValue, attribValueA );
                            
							sscanf(attribValueA, "%ld", &particleInfo->startTime );

							// set life time
							attribValue = xmlRead->getAttributeValue(L"lifeTime");
							uiString::convertUnicodeToUTF8(attribValue, attribValueA );
                            
							sscanf(attribValueA, "%ld", &particleInfo->lifeTime );							

						}
						else if (core::stringw(L"attributes") == xmlRead->getNodeName())
						{
							io::IAttributes* attributes = fs->createEmptyAttributes();
							attributes->read(xmlRead, true);

							if ( particle )
								particle->deserializeAttributes( attributes );
							
							if ( particleInfo )
							{
								particleInfo->minParticle =	particle->getEmitter()->getMinParticlesPerSecond();
								particleInfo->maxParticle =	particle->getEmitter()->getMaxParticlesPerSecond();
							}

							attributes->drop();
							particle = NULL;
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}

	xmlRead->drop();


	// cache this node
	SParticleCacheItem *item = new SParticleCacheItem();
	item->arrayParticle = m_arrayParticle;
	item->node = m_gameObject->m_node;

	CParticleCache::cacheNode( std::string(lpFileName), item );
}

// stopParticle
// stop particle emitter
void CParticleComponent::stopParticle()
{
	m_time = 0;
	m_stopEmitter = true;

	std::vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
	while (i != end )
	{
		SParticleInfo& p = (*i);
		
		p.ps->getEmitter()->setMinParticlesPerSecond(0);
		p.ps->getEmitter()->setMaxParticlesPerSecond(0);

		i++;
	}

}

// startParticle
// begin particle emitter
void CParticleComponent::startParticle()
{
	m_time = 0;
	m_stopEmitter = false;

	m_time = (long) irr::os::Timer::getTime();
	m_totalLifeTime = -1;
}

// fixParticlePosition
// fix affector rotation & affector attraction
void CParticleComponent::fixParticlePosition()
{
	// ajust affector
	std::vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
	while (i != end )
	{
		SParticleInfo& p = (*i);	
		int idAffector = 0;
		IParticleAffector *affector = p.ps->getAffector(idAffector);

		// calc position
		m_gameObject->m_node->updateAbsolutePosition();
		p.ps->updateAbsolutePosition();

		while ( affector )
		{						
			E_PARTICLE_AFFECTOR_TYPE affectorType = affector->getType();

			if ( affectorType == EPAT_ATTRACT )
			{
				IParticleAttractionAffector *af = (IParticleAttractionAffector*) affector;
				af->setPoint( p.ps->getAbsolutePosition() );
			}
			else if ( affectorType == EPAT_ROTATE )
			{
				IParticleRotationAffector *af = (IParticleRotationAffector*) affector;
				af->setPivotPoint( p.ps->getAbsolutePosition() );
			}

			idAffector++;
			affector = p.ps->getAffector(idAffector);
		}
		i++;
	}
}