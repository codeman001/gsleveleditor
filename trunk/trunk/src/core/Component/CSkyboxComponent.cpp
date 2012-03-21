#include "stdafx.h"
#include "CSkyboxComponent.h"
#include "IView.h"

CSkyboxComponent::CSkyboxComponent( CGameObject *pObj )
	:IObjectComponent( pObj, IObjectComponent::Skybox )
{
}

CSkyboxComponent::~CSkyboxComponent()
{
}

// init
// run when init object
void CSkyboxComponent::initComponent()
{
}

// updateComponent
// update object by frame
void CSkyboxComponent::updateComponent()
{
}

// loadSkyTextureFile
// load file
void CSkyboxComponent::loadSkyTextureFile( char *lpFileName )
{
	if ( m_skyboxTexture == lpFileName )
		return;

	m_skyboxTexture = lpFileName;

	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	ISceneManager *smgr = getIView()->getSceneMgr();
	IVideoDriver *driver= getIView()->getDriver();

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	ITexture *pTex = driver->getTexture(lpFileName);

#ifdef GSEDITOR
	if ( pTex == NULL )
	{
		WCHAR appPath[MAX_PATH];
		char  appPathA[MAX_PATH];

		uiApplication::getAppPath(appPath, MAX_PATH);
		uiString::copy<char, WCHAR>( appPathA, appPath  );
		
		std::string path = appPathA;
		path += "\\";
		path += lpFileName;
		
		pTex = driver->getTexture(path.c_str());
	}
#endif

	scene::ISceneNode* skydome = smgr->addSkyDomeSceneNode( pTex, 16,8, 0.95f, 2.0f);
	
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	// set scenenode
	m_gameObject->m_node = skydome;
	skydome->grab();

	// set visibke
	m_gameObject->m_node->setVisible( true );
	
	// update position
	m_gameObject->updateNodePosition();
	m_gameObject->updateNodeRotation();
}

// saveData
// save data to serializable
void CSkyboxComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	// add path to mesh file
	pObj->addPath("skyTexture", m_skyboxTexture.c_str(), true);
}

// loadData
// load data to serializable
void CSkyboxComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	// read mesh file
	char *string = pObj->readString();
	loadSkyTextureFile( string );
}