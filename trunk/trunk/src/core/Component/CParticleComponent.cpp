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
	
	wchar_t lpTemp1[1024];
	wchar_t lpTemp2[1024];

	vector<SParticleInfo>::iterator i = m_arrayParticle.begin(), end = m_arrayParticle.end();
	while (i != end )
	{
		SParticleInfo& p = (*i);	

					
		p.ps->serializeAttributes( attrb );

		uiString::copy<wchar_t,const irr::c8>(lpTemp1, p.ps->getName());
		uiString::copy<wchar_t,const char>(lpTemp2, p.texture.c_str());

		xmlWrite->writeElement(L"ps",false, L"name", lpTemp1, L"texture", lpTemp2);
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
					
							const wchar_t *attribValue = xmlRead->getAttributeValue(L"name");
							char attribValueA[1024] = {0};
							uiString::convertUnicodeToUTF8( (unsigned short*)attribValue, attribValueA );
							particle->setName( attribValue );

							attribValue = xmlRead->getAttributeValue(L"texture");							
							uiString::convertUnicodeToUTF8( (unsigned short*)attribValue, attribValueA );							

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
						}
						else if (core::stringw(L"attributes") == xmlRead->getNodeName())
						{
							io::IAttributes* attributes = fs->createEmptyAttributes();
							attributes->read(xmlRead, true);

							if ( particle )
								particle->deserializeAttributes( attributes );
							
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