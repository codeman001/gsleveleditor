#include "stdafx.h"
#include "CTrigger.h"
#include "IView.h"
#include "CGameBoxSceneNode.h"

CTrigger::CTrigger()
{
	m_objectType	= CGameObject::TriggerObject;

	// init box
	ISceneManager *smgr = getIView()->getSceneMgr();

	m_boxSize = 20.0f;

	// create node
	m_node = new CGameBoxSceneNode(this, m_boxSize, smgr->getRootSceneNode(), smgr, m_objectID);

#ifdef GSEDITOR
	video::SMaterial& mat = m_node->getMaterial(0);	
	mat.Lighting = true;
	mat.DiffuseColor	= video::SColor(100, 0,255,0 );
	mat.AmbientColor	= video::SColor(100, 0,255,0 );
	mat.SpecularColor	= video::SColor(100, 0,255,0 );

	mat.setFlag( EMF_COLOR_MATERIAL, false );	
	mat.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
#endif

	// add collision
	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( m_node );
	m_node->setTriangleSelector(selector);
	selector->drop();

#ifdef GSEDITOR
	setVisible ( true );	
#else
	setVisible ( false );
#endif

}

CTrigger::~CTrigger()
{
}

// saveData
// save data to serializable ( use for save in game .sav )
void CTrigger::saveData( CSerializable* pObj )
{
	pObj->addGroup( s_stringObjType[m_objectType] );
	CGameObject::saveData( pObj );
}

// loadData
// load data to serializable ( use for load in game .sav )
void CTrigger::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
	CGameObject::loadData( pObj );
}

// getData
// get basic data to serializable
void CTrigger::getData( CSerializable* pObj )
{
	pObj->addGroup( s_stringObjType[m_objectType] );
	CGameObject::getData( pObj );
}

// updateData
// update data
void CTrigger::updateData( CSerializable* pObj )
{
	pObj->nextRecord();	
	CGameObject::updateData( pObj );
}