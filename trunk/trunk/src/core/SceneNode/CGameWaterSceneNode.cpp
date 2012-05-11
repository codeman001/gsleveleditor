#include "stdafx.h"
#include "CGameWaterSceneNode.h"
#include "IView.h"

CGameWaterSceneNode::CGameWaterSceneNode(CGameObject *owner, IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, 
		const core::vector3df& position,
		s32 id)
	:ISceneNode( parent, mgr, id, position, core::vector3df(0,0,0) )
{
	m_mesh = mesh;
	m_mesh->grab();

	static s32 s_materialWater = -1;
	if ( s_materialWater == -1 )
	{		
		CWaterGrassCallBack* waterShaderCB = new CWaterGrassCallBack();

		s_materialWater = mgr->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles
		(
			"shader/extern/water.vert", "vertexMain",	video::EVST_VS_2_0,
			"shader/extern/water.frag", "pixelMain",	video::EPST_PS_2_0,
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

	driver->setTransform( video::ETS_WORLD, AbsoluteTransformation );    

    for(int i=0; i < (int)m_mesh->getMeshBufferCount();i++)
    {
        driver->setMaterial( m_mesh->getMeshBuffer(i)->getMaterial() );
        driver->drawMeshBuffer(m_mesh->getMeshBuffer(i));
    }
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