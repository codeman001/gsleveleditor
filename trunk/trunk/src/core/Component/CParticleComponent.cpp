#include "stdafx.h"
#include "CParticleComponent.h"
#include "IView.h"

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
		return;

	bool isStop = false;

	if ( m_time == 0 )
	{				
		m_time = (long) irr::os::Timer::getTime();
		return;
	}
	
	long currentTime = (long) irr::os::Timer::getTime();
	long deltaTime = currentTime - m_time;

	vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
	while (i != end )
	{
		SParticleInfo& p = (*i);
		
		if ( 
				p.startTime > deltaTime || 
				( p.lifeTime > 0 && p.startTime + p.lifeTime < deltaTime )
			)
		{
			p.ps->getEmitter()->setMinParticlesPerSecond( 0 );
			p.ps->getEmitter()->setMaxParticlesPerSecond( 0 );
			p.isStop = true;
		}
		else
		{
			p.ps->getEmitter()->setMinParticlesPerSecond( p.minParticle );
			p.ps->getEmitter()->setMaxParticlesPerSecond( p.maxParticle );
			p.isStop = false;
		}

		i++;
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
// create empty particle
void CParticleComponent::initParticle()
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	// create an empty node
	m_gameObject->m_node = smgr->addEmptySceneNode( smgr->getRootSceneNode() );
	m_gameObject->m_node->grab();

	// load particle
	loadXML( m_xmlPath.c_str() );

	m_time = 0;
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
	vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();

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
	vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
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
	vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
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

	vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
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
		
		// time start
		uiString::format<wchar_t>( lpTemp, L"%d", p.startTime);
		propertyName.push_back( L"startTime" );
		propertyValue.push_back( core::stringw(lpTemp) );

		// life time
		uiString::format<wchar_t>( lpTemp, L"%d", p.lifeTime);
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
		return;	

	IParticleSystemSceneNode *particle = NULL;
	SParticleInfo*	particleInfo = NULL;

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
							char attribValueA[1024] = {0};
							uiString::convertUnicodeToUTF8( (unsigned short*)attribValue, attribValueA );
							particle->setName( attribValue );

							attribValue = xmlRead->getAttributeValue(L"texture");							
							uiString::convertUnicodeToUTF8( (unsigned short*)attribValue, attribValueA );							

							// set texture
							particleInfo->texture = attribValueA;

							ITexture *pTex = driver->getTexture( attribValueA );

#if defined(GSEDITOR) || defined(PARTICLE_EDITOR)
							if ( pTex == NULL )
							{
								WCHAR appPath[MAX_PATH];
								char  appPathA[MAX_PATH];

								uiApplication::getAppPath(appPath, MAX_PATH);
								uiString::copy<char, WCHAR>( appPathA, appPath  );
								
								std::string path = appPathA;
								path += "\\";
								path += std::string(attribValueA);
								
								pTex = driver->getTexture(path.c_str());
							}
#endif
							if ( pTex && particle )
							{
								particle->setMaterialTexture(0, pTex );
								particle->setMaterialFlag(	video::EMF_LIGHTING, false );
								particle->setMaterialType(	video::EMT_TRANSPARENT_ADD_COLOR);
							}

							// set start time
							attribValue = xmlRead->getAttributeValue(L"startTime");							
							uiString::convertUnicodeToUTF8( (unsigned short*)attribValue, attribValueA );	
							sscanf(attribValueA, "%d", &particleInfo->startTime );

							// set life time
							attribValue = xmlRead->getAttributeValue(L"lifeTime");							
							uiString::convertUnicodeToUTF8( (unsigned short*)attribValue, attribValueA );
							sscanf(attribValueA, "%d", &particleInfo->lifeTime );							

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
}

// stopParticle
// stop particle emitter
void CParticleComponent::stopParticle()
{
	m_time = 0;
	m_stopEmitter = true;

	vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
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
}