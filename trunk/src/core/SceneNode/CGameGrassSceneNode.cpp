#include "stdafx.h"
#include "CGameGrassSceneNode.h"
#include "CGameObject.h"

CGameGrassSceneNode::CGameGrassSceneNode(CGameObject *owner, IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, const core::vector3df& position, s32 id)
	:ISceneNode( parent, mgr, id, position, core::vector3df(0,0,0), core::vector3df(0,0,0) )
{
	m_owner = owner;
	m_mesh	= mesh;
	
	if ( m_mesh == NULL )
		return;

	// grab mesh
	m_mesh->grab();
	
	static s32 s_materialGrass = -1;
	if ( s_materialGrass == -1 )
	{
		CShaderGrassCallBack* grassShaderCB = new CShaderGrassCallBack();

		s_materialGrass = mgr->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles
		(
			"shader/extern/grass.vert", "vertexMain",	video::EVST_VS_1_1,
			"shader/extern/grass.frag", "pixelMain",	video::EPST_PS_1_1,
			grassShaderCB, 
			video::EMT_TRANSPARENT_ALPHA_CHANNEL
		);

		grassShaderCB->drop();
	}
	
	m_Material = m_mesh->getMeshBuffer(0)->getMaterial();
	m_Material.MaterialType = (E_MATERIAL_TYPE)s_materialGrass;
    m_Material.MaterialTypeParam = 0.3f;

	setScale( core::vector3df(1,1,1) );

}

CGameGrassSceneNode::~CGameGrassSceneNode()
{
	m_mesh->drop();
}

// OnRegisterSceneNode
// irr scenenode impl
void CGameGrassSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

// render
// irr scenenode impl
void CGameGrassSceneNode::render()
{
	ISceneManager	*smgr = getSceneManager();
	IVideoDriver	*driver = smgr->getVideoDriver();
	
	// set world transform
	driver->setTransform( video::ETS_WORLD, getAbsoluteTransformation());

	// set current material with config shader
    driver->setMaterial( m_Material);

	// draw mesh with grass shader
	int meshCount = m_mesh->getMeshBufferCount();
	for ( int i = 0; i < meshCount; i++ )
		driver->drawMeshBuffer(m_mesh->getMeshBuffer(i));
}

// getBoundingBox
// irr scenenode impl
const core::aabbox3d<f32>& CGameGrassSceneNode::getBoundingBox() const
{
	return m_mesh->getBoundingBox();
}

// getMaterialCount
// irr scenenode impl
u32 CGameGrassSceneNode::getMaterialCount() const
{
	return 1;
}

// getMaterial
// irr scenenode impl
video::SMaterial& CGameGrassSceneNode::getMaterial(u32 i)
{
	return m_Material;
}