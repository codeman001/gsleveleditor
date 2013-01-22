#include "stdafx.h"
#include "CGameStaticShadowSceneNode.h"
#include "CShadowComponent.h"

CGameStaticShadowSceneNode::CGameStaticShadowSceneNode(CGameObject *owner, ISceneNode* parent, ISceneManager* mgr, 
		const core::vector3df& position, s32 id)
	:ISceneNode( parent, mgr, id, position, core::vector3df(0,0,0) )
{
	m_owner = owner;
	Buffer = new SMeshBuffer();
}

CGameStaticShadowSceneNode::~CGameStaticShadowSceneNode()
{
	Buffer->drop();
}

// OnRegisterSceneNode
// ISceneNode implement
void CGameStaticShadowSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

// render
// ISceneNode implement
void CGameStaticShadowSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver)
		return;

	const core::matrix4 &m = camera->getViewFrustum()->getTransform( video::ETS_VIEW );
	const core::vector3df view ( -m[2], -m[6] , -m[10] );

	// get shadow comp
	CShadowComponent* shadow = (CShadowComponent*)m_owner->getComponent(IObjectComponent::Shadow);
	if ( shadow == NULL )
		return;

	// make buffer by shadow pos
	std::vector<video::S3DVertex>& listShadow = shadow->getListShadow();
	int nShadow = (int)listShadow.size();

	if ( nShadow > 0 )
	{
		// create shadow mesh buffer
		reallocateBuffers(nShadow);
		
		s32 idx = 0;

		std::vector<video::S3DVertex>::iterator i = listShadow.begin(), end = listShadow.end();
		while (i != end)
		{
			core::vector3df pos = i->Pos;
			core::vector3df nor = i->Normal;
			float width		= 80.0f;
			float height	= 80.0f;

			// set texcoord
			Buffer->Vertices[0+idx].TCoords.set(0.0f, 0.0f);
			Buffer->Vertices[1+idx].TCoords.set(0.0f, 1.0f);
			Buffer->Vertices[2+idx].TCoords.set(1.0f, 1.0f);
			Buffer->Vertices[3+idx].TCoords.set(1.0f, 0.0f);
			
			// calc plane position
			f32 f = 0.5f * width;
			core::vector3df horizontal ( f, 0, 0 );

			f = -0.5f * height;
			core::vector3df vertical ( 0, 0, f );

			// rotate plane
			core::quaternion quaternion; 			
			quaternion.rotationFromTo( core::vector3df(0.0f,1.0f,0.0f), nor );

			core::matrix4 matrix = quaternion.getMatrix(); 
			matrix.rotateVect(horizontal); 
			matrix.rotateVect(vertical);


			// update buffer position
			Buffer->Vertices[0+idx].Pos = pos + horizontal + vertical;
			Buffer->Vertices[0+idx].Color = SColor(255,255,255,255);
			Buffer->Vertices[0+idx].Normal = nor;

			Buffer->Vertices[1+idx].Pos = pos + horizontal - vertical;
			Buffer->Vertices[1+idx].Color = SColor(255,255,255,255);
			Buffer->Vertices[1+idx].Normal = nor;

			Buffer->Vertices[2+idx].Pos = pos - horizontal - vertical;
			Buffer->Vertices[2+idx].Color = SColor(255,255,255,255);
			Buffer->Vertices[2+idx].Normal = nor;

			Buffer->Vertices[3+idx].Pos = pos - horizontal + vertical;
			Buffer->Vertices[3+idx].Color = SColor(255,255,255,255);
			Buffer->Vertices[3+idx].Normal = nor;

			idx += 4;

			Buffer->BoundingBox.addInternalPoint(pos);
			++i;
		}

		// render all
		core::matrix4 mat;
		driver->setTransform(video::ETS_WORLD, mat);

		// render 2 face on nonbillboard particle
		Buffer->Material.BackfaceCulling = false;
		Buffer->Material.FrontfaceCulling = false;

		driver->setMaterial(Buffer->Material);

		driver->drawVertexPrimitiveList(
				Buffer->getVertices(), 
				nShadow*4,
				Buffer->getIndices(), 
				nShadow*2,
				video::EVT_STANDARD, EPT_TRIANGLES,
				Buffer->getIndexType()
			);
	}

	listShadow.clear();
}

// getBoundingBox
// ISceneNode implement
const core::aabbox3d<f32>& CGameStaticShadowSceneNode::getBoundingBox() const
{
	return Buffer->getBoundingBox();
}

// getMaterialCount
// ISceneNode implement
u32 CGameStaticShadowSceneNode::getMaterialCount() const
{
	return 1;
}

// getMaterial
// ISceneNode implement
video::SMaterial& CGameStaticShadowSceneNode::getMaterial(u32 i)
{
	return Buffer->Material;
}

// reallocateBuffers
// calc buffer size
void CGameStaticShadowSceneNode::reallocateBuffers(int numShadow)
{
	if ( numShadow*4 > (int)Buffer->getVertexCount() || numShadow*6 > (int)Buffer->getIndexCount() )
	{
		u32 oldSize = Buffer->getVertexCount();
		Buffer->Vertices.set_used(numShadow * 4);

		u32 i;
	
		// fill remaining indices
		u32 oldIdxSize = Buffer->getIndexCount();
		u32 oldvertices = oldSize;
		Buffer->Indices.set_used(numShadow * 6);

		for (i=oldIdxSize; i<Buffer->Indices.size(); i+=6)
		{
			Buffer->Indices[0+i] = (u16)0+oldvertices;
			Buffer->Indices[1+i] = (u16)2+oldvertices;
			Buffer->Indices[2+i] = (u16)1+oldvertices;
			Buffer->Indices[3+i] = (u16)0+oldvertices;
			Buffer->Indices[4+i] = (u16)3+oldvertices;
			Buffer->Indices[5+i] = (u16)2+oldvertices;
			oldvertices += 4;
		}
	}
}