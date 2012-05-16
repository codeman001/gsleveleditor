#include "stdafx.h"
#include "CGameColladaSceneNode.h"
#include "CColladaMeshComponent.h"
#include "IView.h"

//////////////////////////////////////////////////////////
// CGameColladaMesh implement
//////////////////////////////////////////////////////////

// clone
// clone this mesh
CGameColladaMesh* CGameColladaMesh::clone()
{
	CGameColladaMesh* newMesh = new CGameColladaMesh();

	newMesh->Joints = Joints;
	newMesh->JointIndex = JointIndex;
	newMesh->JointVertexIndex = JointVertexIndex;
	newMesh->BoundingBox = BoundingBox;
	
	ISceneManager *smgr = getIView()->getSceneMgr();
	SMesh *mesh = smgr->getMeshManipulator()->createMeshCopy( this );

	int n = mesh->getMeshBufferCount();
	for ( int i = 0; i < n; i++ )
	{
		IMeshBuffer *buffer = mesh->getMeshBuffer(i);
		buffer->getMaterial() = getMeshBuffer(i)->getMaterial();

		newMesh->addMeshBuffer( buffer );
	}

	mesh->drop();
	return newMesh;
}

// updateJoint
// remap joint to node
void CGameColladaMesh::updateJoint()
{
	int nJoint = Joints.size();
	char nameA[1024];

	for ( int i = 0; i < nJoint; i++ )
	{
		uiString::copy<char, const wchar_t>( nameA, Joints[i].name.c_str() );
		Joints[i].node = Component->getSceneNodeBySID( nameA ) ;
	}
}

//////////////////////////////////////////////////////////
// CGameColladaSceneNode implement
//////////////////////////////////////////////////////////

CGameColladaSceneNode::CGameColladaSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
	:ISceneNode( parent, mgr, id )
{
	m_currentFrame = 0;
	m_totalFrame = 0;
	m_framePerSecond = 24.0f;
	m_isRootColladaNode = true;
	m_enableAnim = true;
	m_timer = 0;

	m_posHint = 0;
	m_scaleHint = 0;
	m_rotHint = 0;

	ColladaMesh = NULL;	
	m_component = NULL;

#ifdef GSANIMATION
	m_isShowName = false;
#endif
}

CGameColladaSceneNode::~CGameColladaSceneNode()
{
	if ( ColladaMesh )
		ColladaMesh->drop();
}

void CGameColladaSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);		
	}
	ISceneNode::OnRegisterSceneNode();
	
}

void CGameColladaSceneNode::updateAbsolutePosition()
{
	// calc absolute transform
	if ( Parent )
		AbsoluteTransformation = Parent->getAbsoluteTransformation() * AnimationMatrix;
	else
		AbsoluteTransformation = AnimationMatrix;

	// calc absolute animation
	if ( m_isRootColladaNode == true )
		AbsoluteAnimationMatrix = AnimationMatrix;
	else
		AbsoluteAnimationMatrix = ((CGameColladaSceneNode*)Parent)->AbsoluteAnimationMatrix * AnimationMatrix;

}

void CGameColladaSceneNode::OnAnimate(u32 timeMs)
{
	ISceneNode::OnAnimate( timeMs );
	
	// reset timer
	if ( m_timer == 0 )
	{
		m_timer = timeMs;
		return;
	}

	// skin mesh
	if ( ColladaMesh != NULL && ColladaMesh->IsStaticMesh == false )		
		skin();
	
	// get last frame
	m_totalFrame  = 0;
	if ( RotationKeys.size() > 0 )
		m_totalFrame = RotationKeys.getLast().frame;
	else if ( PositionKeys.size() > 0 )
		m_totalFrame = PositionKeys.getLast().frame;

	if ( m_totalFrame == 0 )
		return;

	if ( m_component && m_component->isPauseAnim() )
	{
		m_currentFrame = m_component->getPauseAnim();
		if ( m_currentFrame > m_totalFrame )
			m_currentFrame = m_totalFrame;
	}
	else
	{
		// calc current frame	
		m_currentFrame += (timeMs - m_timer) * m_framePerSecond/1000.0f;
		
		if ( m_currentFrame > m_totalFrame )
		{
			if ( m_component && 
				m_component->getCurrentAnim() && 
				m_component->getCurrentAnim()->m_loop == false )
			{
				m_currentFrame = m_totalFrame;
			}
			else		
				m_currentFrame = 0;
		}
	}
		
	// update animation translate
	if ( m_enableAnim == true )
	{
		updateAnimation();
		if ( m_component )
			m_component->setCurrentFrame(m_currentFrame);
	}

	m_timer = timeMs;
}

// skin
// skin mesh
void CGameColladaSceneNode::skin()
{	
	if ( ColladaMesh == NULL || ColladaMesh->IsStaticMesh == true )
		return;

	IMeshBuffer *meshBuffer		= ColladaMesh->getMeshBuffer(0);	
	S3DVertex	*vertexBuffer	= (S3DVertex*)meshBuffer->getVertices();
	S3DVertex	*vertex			= vertexBuffer;
	s32			*jointInVertex	= (s32*)ColladaMesh->JointVertexIndex.pointer();
	s32			*jointIndex		= (s32*)ColladaMesh->JointIndex.pointer();

	int nJoint = ColladaMesh->Joints.size();
	CGameColladaMesh::SJoint *arrayJoint = ColladaMesh->Joints.pointer();
	CGameColladaMesh::SJoint *pJoint = arrayJoint;

	// calc joint matrix
	for ( int i = 0; i < nJoint; i++, pJoint++ )
		pJoint->skinningMatrix.setbyproduct( pJoint->node->AbsoluteAnimationMatrix, pJoint->globalInversedMatrix );

	// skinning in vertex
	core::vector3df thisVertexMove, thisNormalMove;
	int nVertex = ColladaMesh->JointVertexIndex.size();
	int id = 0;

	for ( int i = 0; i < nVertex; i++, vertex++ )
	{
		core::vector3df positionCumulator(0.0, 0.0, 0.0);
		core::vector3df normalCumulator(0.0, 0.0, 0.0);
		
		s32 nJointCount = jointInVertex[i];

		for ( int iJoint = 0; iJoint < nJointCount; iJoint++, jointIndex++ )
		{
			u32 boneId		= *jointIndex;
			u32 weightId	= *(++jointIndex);
			
			CGameColladaMesh::SJoint	*pJoint		= &arrayJoint[boneId];
			CGameColladaMesh::SWeight	*pWeight	= &pJoint->weights[weightId];
			
			// transform vertex position
			pJoint->skinningMatrix.transformVect	(thisVertexMove, pWeight->staticPos);

			// transform normal vector
			pJoint->skinningMatrix.rotateVect		(thisNormalMove, pWeight->staticNormal);
			
			positionCumulator	+= thisVertexMove * pWeight->strength;
			normalCumulator		+= thisNormalMove * pWeight->strength;
		}
		
		// apply skin pos & normal
		vertex->Pos = positionCumulator;
		vertex->Normal = normalCumulator;
	}
	
	meshBuffer->recalculateBoundingBox();
	meshBuffer->setDirty( EBT_VERTEX );
	
	ColladaMesh->BoundingBox = meshBuffer->getBoundingBox();
}

// updateAnimation
// calc relative matrix of animation
void CGameColladaSceneNode::updateAnimation()
{
	core::vector3df position;
	core::vector3df scale;
	core::quaternion rotation;
	
	getFrameData( m_currentFrame, position, m_posHint, scale, m_scaleHint, rotation, m_rotHint );

	// rotation
	core::matrix4 mat = rotation.getMatrix();
	
	// position	
	f32 *m1 = mat.pointer();

	m1[0] += position.X*m1[3];
	m1[1] += position.Y*m1[3];
	m1[2] += position.Z*m1[3];

	m1[4] += position.X*m1[7];
	m1[5] += position.Y*m1[7];
	m1[6] += position.Z*m1[7];

	m1[8] += position.X*m1[11];
	m1[9] += position.Y*m1[11];
	m1[10] += position.Z*m1[11];

	m1[12] += position.X*m1[15];
	m1[13] += position.Y*m1[15];
	m1[14] += position.Z*m1[15];
	
	// update current relative matrix
	AnimationMatrix = mat;

}

void CGameColladaSceneNode::getFrameData(f32 frame,
				core::vector3df &position,	s32 &positionHint,
				core::vector3df &scale,		s32 &scaleHint,
				core::quaternion &rotation, s32 &rotationHint)
{
	s32 foundPositionIndex = -1;
	s32 foundScaleIndex = -1;
	s32 foundRotationIndex = -1;
	
	if (PositionKeys.size())
	{
		foundPositionIndex = -1;

		//Test the Hints...
		if (positionHint>=0 && (u32)positionHint < PositionKeys.size())
		{
			//check this hint
			if (positionHint>0 && PositionKeys[positionHint].frame>=frame && PositionKeys[positionHint-1].frame<frame )
				foundPositionIndex=positionHint;
			else if (positionHint+1 < (s32)PositionKeys.size())
			{
				//check the next index
				if ( PositionKeys[positionHint+1].frame>=frame &&
						PositionKeys[positionHint+0].frame<frame)
				{
					positionHint++;
					foundPositionIndex=positionHint;
				}
			}
		}

		//The hint test failed, do a full scan...
		if (foundPositionIndex==-1)
		{
			for (u32 i=0; i<PositionKeys.size(); ++i)
			{
				if (PositionKeys[i].frame >= frame) //Keys should to be sorted by frame
				{
					foundPositionIndex=i;
					positionHint=i;
					break;
				}
			}
		}

		//Do interpolation...
		if ( foundPositionIndex == 0 )
		{
			position = PositionKeys[0].position;
		}
		else if (foundPositionIndex!=-1)
		{
			const SPositionKey& KeyA = PositionKeys[foundPositionIndex];
			const SPositionKey& KeyB = PositionKeys[foundPositionIndex-1];

			const f32 fd1 = frame - KeyA.frame;
			const f32 fd2 = KeyB.frame - frame;
			position = ((KeyB.position-KeyA.position)/(fd1+fd2))*fd1 + KeyA.position;				
		}
	}

	//------------------------------------------------------------

	if (ScaleKeys.size())
	{
		foundScaleIndex = -1;

		//Test the Hints...
		if (scaleHint>=0 && (u32)scaleHint < ScaleKeys.size())
		{
			//check this hint
			if (scaleHint>0 && ScaleKeys[scaleHint].frame>=frame && ScaleKeys[scaleHint-1].frame<frame )
				foundScaleIndex=scaleHint;
			else if (scaleHint+1 < (s32)ScaleKeys.size())
			{
				//check the next index
				if ( ScaleKeys[scaleHint+1].frame>=frame &&
						ScaleKeys[scaleHint+0].frame<frame)
				{
					scaleHint++;
					foundScaleIndex=scaleHint;
				}
			}
		}


		//The hint test failed, do a full scan...
		if (foundScaleIndex==-1)
		{
			for (u32 i=0; i<ScaleKeys.size(); ++i)
			{
				if (ScaleKeys[i].frame >= frame) //Keys should to be sorted by frame
				{
					foundScaleIndex=i;
					scaleHint=i;
					break;
				}
			}
		}

		//Do interpolation...
		if ( foundPositionIndex == 0 )
		{
			scale = ScaleKeys[0].scale;
		}
		else if (foundScaleIndex!=-1)
		{
			const SScaleKey& KeyA = ScaleKeys[foundScaleIndex];
			const SScaleKey& KeyB = ScaleKeys[foundScaleIndex-1];

			const f32 fd1 = frame - KeyA.frame;
			const f32 fd2 = KeyB.frame - frame;
			scale = ((KeyB.scale-KeyA.scale)/(fd1+fd2))*fd1 + KeyA.scale;				
		}
	}

	//-------------------------------------------------------------

	if (RotationKeys.size())
	{
		foundRotationIndex = -1;

		//Test the Hints...
		if (rotationHint>=0 && (u32)rotationHint < RotationKeys.size())
		{
			//check this hint
			if (rotationHint>0 && RotationKeys[rotationHint].frame>=frame && RotationKeys[rotationHint-1].frame<frame )
				foundRotationIndex=rotationHint;
			else if (rotationHint+1 < (s32)RotationKeys.size())
			{
				//check the next index
				if ( RotationKeys[rotationHint+1].frame>=frame &&
						RotationKeys[rotationHint+0].frame<frame)
				{
					rotationHint++;
					foundRotationIndex=rotationHint;
				}
			}
		}


		//The hint test failed, do a full scan...
		if (foundRotationIndex==-1)
		{
			for (u32 i=0; i<RotationKeys.size(); ++i)
			{
				if (RotationKeys[i].frame >= frame) //Keys should be sorted by frame
				{
					foundRotationIndex=i;
					rotationHint=i;
					break;
				}
			}
		}

		//Do interpolation...
		if ( foundPositionIndex == 0 )
		{
			rotation = RotationKeys[0].rotation;
		}
		else if (foundRotationIndex!=-1)
		{
			const SRotationKey& KeyA = RotationKeys[foundRotationIndex];
			const SRotationKey& KeyB = RotationKeys[foundRotationIndex-1];

			const f32 fd1 = frame - KeyA.frame;
			const f32 fd2 = KeyB.frame - frame;
			const f32 t = fd1/(fd1+fd2);

			rotation.slerp(KeyA.rotation, KeyB.rotation, t);				
		}
	}	
}

void CGameColladaSceneNode::render()
{
	// get driver
	IVideoDriver* driver = getSceneManager()->getVideoDriver();	
	IView *pView = getIView();	

	if ( ColladaMesh )
	{		
		bool isTransparentPass = SceneManager->getSceneNodeRenderPass() == scene::ESNRP_TRANSPARENT;
		
		// set transform
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

		for (u32 i=0; i<ColladaMesh->getMeshBufferCount(); ++i)
		{
			scene::IMeshBuffer* mb = ColladaMesh->getMeshBuffer(i);
			if (mb)
			{
				const video::SMaterial& material = mb->getMaterial();

				video::IMaterialRenderer* rnd = driver->getMaterialRenderer(material.MaterialType);
				bool transparent = (rnd && rnd->isTransparent());

				// only render transparent buffer if this is the transparent render pass
				// and solid only in solid pass
				if (transparent == isTransparentPass)
				{
					driver->setMaterial(material);
					driver->drawMeshBuffer(mb);
				}
			}
		}

		Box = ColladaMesh->getBoundingBox();
	}
#ifdef GSANIMATION
	else
	{
		// set identity transform
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix );

		irr::gui::IGUIFont* font = getSceneManager()->getGUIEnvironment()->getBuiltInFont();	

		video::SMaterial debug_mat;
		debug_mat.Lighting = false;
		debug_mat.AntiAliasing = 0;
		debug_mat.ZBuffer = video::ECFN_NEVER;
		driver->setMaterial(debug_mat);
				
		// get current position node
		core::vector3df position = getAbsolutePosition();

		int x, y;
		if ( m_isShowName == true && pView->getScreenCoordinatesFrom3DPosition( position, &x, &y ) == true )
		{
			wchar_t text[1024];
			uiString::copy<wchar_t, const c8>( text, getName() );
			
			// draw bone position
			SColor c = SColor(255,0,0,255);
			driver->draw2DRectangle( c, core::rect<s32>( x - 2, y - 2, x + 2, y + 2 ) ); 

			// draw text
			font->draw( text, core::rect<s32>( x + 10, y, x + 100, y + 50), SColor(255, 255,255,0) );
		}

		// draw parent line
		driver->setMaterial(debug_mat);	
		
		core::list<ISceneNode*>::Iterator i = Children.begin(), end = Children.end();
		while ( i != end )
		{
			driver->draw3DLine( position, (*i)->getAbsolutePosition(), SColor(255,255,0,255) );
			i++;
		}

		Box.MinEdge = core::vector3df(-2, -2, -2);
		Box.MaxEdge = core::vector3df( 2,  2,  2);
	}
#endif

#ifndef GSANIMATION
	if (DebugDataVisible & scene::EDS_BBOX)
#endif
	{		
		// set identity transform
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix );

		video::SMaterial debug_mat;
		debug_mat.Lighting = false;
		debug_mat.AntiAliasing = 0;
		debug_mat.ZBuffer = video::ECFN_NEVER;
		
		driver->setMaterial(debug_mat);

		if ( m_isShowName )
			driver->draw3DBox( getTransformedBoundingBox(), video::SColor(255,0,255,0));
		else
			driver->draw3DBox( getTransformedBoundingBox(), video::SColor(255,255,255,255));
	}
	
}

// clone
// ISceneNode override implement 
ISceneNode* CGameColladaSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
{
	if (!newParent) 
		newParent = Parent;

	if (!newManager)
		newManager = SceneManager;
	
	// clone collada node
	CGameColladaSceneNode *newNode = new CGameColladaSceneNode( newParent, newManager, -1);
		
	newNode->Name = Name;
	newNode->ID = ID;

	newNode->AbsoluteTransformation = AbsoluteTransformation;
	newNode->RelativeTranslation = RelativeTranslation;
	newNode->RelativeRotation = RelativeRotation;
	newNode->RelativeScale = RelativeScale;

	newNode->AutomaticCullingState = AutomaticCullingState;

	newNode->DebugDataVisible = DebugDataVisible;
	newNode->IsVisible = IsVisible;
	newNode->IsDebugObject = IsDebugObject;

	newNode->Box = Box;
	newNode->Material = Material;

	newNode->AnimationMatrix = AnimationMatrix;
	newNode->AbsoluteAnimationMatrix = AbsoluteAnimationMatrix;

	newNode->LocalMatrix = LocalMatrix;
	
	newNode->ColladaMesh = NULL;
	if ( ColladaMesh )
	{
		if ( ColladaMesh->IsStaticMesh == true )
		{
			// static mesh
			newNode->ColladaMesh = ColladaMesh;
			newNode->ColladaMesh->grab();
		}
		else
		{
			// dynamic mesh
			newNode->ColladaMesh = ColladaMesh->clone();
			newNode->ColladaMesh->IsStaticMesh = false;
		}
	}

	newNode->m_isRootColladaNode = m_isRootColladaNode;
	
	newNode->PositionKeys		= PositionKeys;
	newNode->ScaleKeys			= ScaleKeys;
	newNode->RotationKeys		= RotationKeys;

	newNode->m_currentFrame		= m_currentFrame;
	newNode->m_totalFrame		= m_totalFrame;

	newNode->m_framePerSecond	= m_framePerSecond;

	newNode->m_timer			= m_timer;

	newNode->m_posHint			= m_posHint;
	newNode->m_scaleHint		= m_scaleHint;
	newNode->m_rotHint			= m_rotHint;


	int ref = newNode->getReferenceCount();

	return newNode;
}