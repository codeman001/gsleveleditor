#include "stdafx.h"
#include "CGameWaterSceneNode.h"
#include "IView.h"

CGameWaterSceneNode::CGameWaterSceneNode(CGameObject *owner, IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, 
		const core::vector3df& position,
		s32 id)
	:ISceneNode( parent, mgr, id, position, core::vector3df(0,0,0) )
{
	m_owner = owner;
	m_mesh = mesh;
	m_mesh->grab();

	static s32 s_materialWater = -1;
	if ( s_materialWater == -1 )
	{		
		CWaterGrassCallBack* waterShaderCB = new CWaterGrassCallBack();

		s_materialWater = mgr->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles
		(
			getIView()->getPath("shader/extern/water.vsh"), "vertexMain",	video::EVST_VS_1_1,
			getIView()->getPath("shader/extern/water.fsh"), "pixelMain",	video::EPST_PS_1_1,
			waterShaderCB, 
			video::EMT_TRANSPARENT_ALPHA_CHANNEL
		);

		waterShaderCB->drop();		
	}
	
	setMaterialType( (E_MATERIAL_TYPE)s_materialWater );

}

CGameWaterSceneNode::~CGameWaterSceneNode()
{
	m_mesh->drop();
}

// OnRegisterSceneNode
// ISceneNode implement
void CGameWaterSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

// render
// ISceneNode implement
void CGameWaterSceneNode::render()
{
	ISceneManager *smgr = getIView()->getSceneMgr();
	IVideoDriver *driver = smgr->getVideoDriver();

#ifdef GSEDITOR
	CGameObject::EObjectState state = m_owner->getObjectState();
	
	// draw bbox on select
	if ( 
			state == CGameObject::Move ||
			state == CGameObject::Review		
		)
		setDebugDataVisible( EDS_BBOX );
	else
		setDebugDataVisible( 0 );

	// call object draw
	m_owner->drawObject();
#endif

	driver->setTransform( video::ETS_WORLD, AbsoluteTransformation );    	

    for(int i=0; i < (int)m_mesh->getMeshBufferCount();i++)
    {
        driver->setMaterial( m_mesh->getMeshBuffer(i)->getMaterial() );
        driver->drawMeshBuffer(m_mesh->getMeshBuffer(i));
    }

	// draw bouding box
	if ( DebugDataVisible & scene::EDS_BBOX )
	{
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		video::SMaterial deb_m;
		deb_m.Lighting = false;
		driver->setMaterial(deb_m);

		core::aabbox3d<f32> tbox = m_mesh->getBoundingBox();
		getAbsoluteTransformation().transformBoxEx(tbox);

		driver->draw3DBox( tbox, video::SColor(255,255,255,255));
	}

#ifdef GSEDITOR	
	// draw move
	if ( 
			state == CGameObject::Move || 
			state == CGameObject::Rotation ||
			state == CGameObject::Scale
		)
		m_owner->drawFrontUpLeftVector();	
	
	if ( state == CGameObject::Rotation )
		m_owner->drawCircleAroundObject();	
#endif

}

// getBoundingBox
// ISceneNode implement
const core::aabbox3d<f32>& CGameWaterSceneNode::getBoundingBox() const
{
	return m_mesh->getBoundingBox();
}

// getMaterialCount
// ISceneNode implement
u32 CGameWaterSceneNode::getMaterialCount() const
{
	return 1;
}

// getMaterial
// ISceneNode implement
video::SMaterial& CGameWaterSceneNode::getMaterial(u32 i)
{
	return m_mesh->getMeshBuffer(i)->getMaterial();
}