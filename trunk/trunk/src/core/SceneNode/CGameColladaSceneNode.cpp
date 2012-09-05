#include "stdafx.h"
#include "CGameObject.h"
#include "CGameColladaSceneNode.h"
#include "CColladaMeshComponent.h"
#include "IView.h"

#pragma region COLLADAMESH
//////////////////////////////////////////////////////////
// CGameColladaMesh implement
//////////////////////////////////////////////////////////

// clone
// clone this mesh
CGameColladaMesh* CGameColladaMesh::clone()
{
	CGameColladaMesh* newMesh = new CGameColladaMesh();

	newMesh->Joints = Joints;	
	newMesh->BoundingBox = BoundingBox;
	newMesh->IsStaticMesh = IsStaticMesh;

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
		//uiString::copy<char, const wchar_t>( nameA, Joints[i].name.c_str() );
        uiString::convertUnicodeToUTF8((unsigned short*)Joints[i].name.c_str(), nameA);
		Joints[i].node = Component->getSceneNodeBySID( nameA ) ;
	}
}

#pragma endregion


#pragma region COLLADAANIMATIONTRACK
//////////////////////////////////////////////////////////
// CGameAnimationTrack implement
//////////////////////////////////////////////////////////

CGameAnimationTrack::CGameAnimationTrack()
{
	m_currentFrame = 0;
	m_totalFrame = 0;
	m_framePerSecond = 30.0f;
	m_speedRatio = 1.0f;

	m_posHint = 0;
	m_scaleHint = 0;
	m_rotHint = 0;

	m_animWeight = 1.0f;

	m_isEnable = false;
	m_isPause = false;
	m_isLoop = true;

	m_endTrack = false;
}

CGameAnimationTrack::~CGameAnimationTrack()
{

}

void CGameAnimationTrack::getFrameData(f32 frame, core::vector3df &position, core::vector3df &scale, core::quaternion &rotation, const core::matrix4& localMatrix)
{	
	s32 foundPositionIndex	= -1;
	s32 foundScaleIndex		= -1;
	s32 foundRotationIndex	= -1;
	
	// get last hint
	s32 positionHint	= m_posHint;
	s32 scaleHint		= m_scaleHint;
	s32 rotationHint	= m_rotHint;

#pragma region QueryPosition
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
	else
	{
		// default position
		position = core::vector3df(0,0,0);
		localMatrix.translateVect( position );
	}

#pragma endregion	


#pragma region QueryScale
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
		if ( foundScaleIndex == 0 )
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
	else
	{
		scale = core::vector3df(1,1,1);
	}
#pragma endregion
	

#pragma region QueryRotation
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
		if ( foundRotationIndex == 0 )
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
	else
	{
		// default rotation
		rotation = core::quaternion(localMatrix);
	}
#pragma endregion

	// save hint
	m_posHint	= positionHint;
	m_scaleHint = scaleHint;
	m_rotHint	= rotationHint;
}

// update
// update per frame
void CGameAnimationTrack::update(float timeStep)
{
	m_totalFrame  = 0;
	if ( RotationKeys.size() > 0 )
		m_totalFrame = RotationKeys.getLast().frame;
	else if ( PositionKeys.size() > 0 )
		m_totalFrame = PositionKeys.getLast().frame;

	if ( m_totalFrame == 0 )
	{
		m_endTrack = true;
		return;
	}

	if ( m_isPause == false )
	{
		// calc current frame	
		m_currentFrame += timeStep * m_framePerSecond/1000.0f * m_speedRatio;
		
		if ( m_currentFrame > m_totalFrame )
		{
			m_currentFrame = m_totalFrame;
			m_endTrack = true;
		}
		else
		{
			m_endTrack = false;
		}

	}	
}

// crossAnimation
// switch to main animation
void CGameAnimationTrack::crossAnimation()
{
	if ( m_isCrossAnim )
	{
		// cross anim
		PositionKeys.clear();		
		RotationKeys.clear();		
		PositionKeys = CrossAnimPositionKeys;	
		RotationKeys = CrossAnimRotationKeys;	
		
		// update cross animation info
		m_isLoop = m_crossAnimationLoop;
		m_currentFrame = 0;
		
		m_totalFrame  = 0;
		if ( RotationKeys.size() > 0 )
			m_totalFrame = RotationKeys.getLast().frame;
		else if ( PositionKeys.size() > 0 )
			m_totalFrame = PositionKeys.getLast().frame;

		// reset value
		m_isCrossAnim = false;
		m_crossAnimationLoop = false;
	}
}

#pragma endregion


#pragma region COLLADAANIMATION

//////////////////////////////////////////////////////////
// CGameAnimation implement
//////////////////////////////////////////////////////////

CGameAnimation::CGameAnimation()
{
	onlyEnableTrack(0);
}

CGameAnimation::~CGameAnimation()
{
}

// getFrameData
// get anim at frame
void CGameAnimation::getFrameData( core::vector3df &position, core::vector3df &scale, core::quaternion &rotation, const core::matrix4& localMatrix)
{
	position.set(0.0f, 0.0f, 0.0f);
	scale.set(1.0f,1.0f,1.0f);
	rotation.set(0.0f, 0.0f, 0.0f, 1.0f);	


	core::vector3df		currentPosTrack,	posTrack;
	core::vector3df		currentScaleTrack,	scaleTrack;
	core::quaternion	currentRotTrack,	rotTrack;
	bool first = true;

	for ( int i = 0; i < MAX_ANIMTRACK; i++ )
	{
		if ( m_animTrack[i].isEnable() == true )
		{
			m_animTrack[i].getFrameData
				(
					m_animTrack[i].getCurrentFrame(), 
					posTrack, 
					scaleTrack, 
					rotTrack, 
					localMatrix 
				);

			if ( first == true )
			{
				currentPosTrack		= posTrack;
				currentScaleTrack	= scaleTrack;
				currentRotTrack		= rotTrack;

				position	= currentPosTrack;
				scale		= currentScaleTrack;
				rotation	= currentRotTrack;

				first = false;
			}
			else
			{
				float w = m_animTrack[i].getAnimWeight();
				w = core::clamp<float>(w, 0.0f, 1.0f);
				
				position.interpolate( posTrack, currentPosTrack, w );
				scale.interpolate( scaleTrack, currentScaleTrack, w );
				rotation.slerp( currentRotTrack, rotTrack, w );

				currentPosTrack		= position;
				currentScaleTrack	= scale;
				currentRotTrack		= rotation;
			}
		}
	}

	if ( first )
	{
		// no enable animation
		position	= core::vector3df();
		localMatrix.transformVect( position );
		scale		= core::vector3df(1,1,1);
		rotation	= core::quaternion( localMatrix );
	}

}

// synchronizedTimeScale
// sync speed of 2 track
// weight: 0.0f -> 1.0f
void CGameAnimation::synchronizedByTimeScale()
{	
#if 1
	std::vector<int>	listTrackSync;

	// add track 0
	listTrackSync.push_back(0);
	m_animTrack[0].setSpeedRatio(1.0f);

	for ( int i = 1; i < MAX_ANIMTRACK; i++ )
	{
		if ( m_animTrack[i].isEnable() == true )
		{
			float time1 = m_animTrack[0].getTotalFrame();
			float time2 = m_animTrack[i].getTotalFrame();
			
			float weight = m_animTrack[i].getAnimWeight();
			weight = core::clamp<float>(weight, 0.0f, 1.0f);

			// sync frame
			float frame1 = m_animTrack[0].getCurrentFrame();
			float frame2 = time2*frame1/time1;
			m_animTrack[i].setCurrentFrame( frame2 );
						
			// interpolate speed ratio 2 channel
			float ratio = time2 + ( (time1 - time2) * weight );

			// sync speed
			for (int j = 0, n = listTrackSync.size(); j < n; j++ )
			{
				float animTime		= m_animTrack[j].getTotalFrame();
				float animRatio		= time2 + ( (animTime - time2) * weight );
				float animWeight	= m_animTrack[j].getAnimWeight();

				m_animTrack[j].setSpeedRatio( animTime/animRatio );
				m_animTrack[j].setAnimWeight( animWeight*weight );
			}

			m_animTrack[i].setSpeedRatio( time2/ratio );
			m_animTrack[i].setAnimWeight( 1.0f - weight );

			// add track i
			listTrackSync.push_back(i);
		}
	}
#else
	if ( m_animTrack[0].isEnable() == false || m_animTrack[1].isEnable() == false ||
		m_animTrack[0].getTotalFrame() == 0.0f || m_animTrack[1].getTotalFrame() == 0.0f )
	{
		m_animTrack[0].setSpeedRatio( 1.0f );
		m_animTrack[1].setSpeedRatio( 1.0f );
		return;
	}

	float weight = m_animTrack[1].getAnimWeight();
	weight = core::clamp<float>( weight, 0.0f, 1.0f );
	float time1 = m_animTrack[0].getTotalFrame();
	float time2 = m_animTrack[1].getTotalFrame();
	 
	// sync frame
	float frame1 = m_animTrack[0].getCurrentFrame();
	float frame2 = time2*frame1/time1;
	m_animTrack[1].setCurrentFrame( frame2 );

	// interpolate speed ratio 2 channel	
	float ratio = time2 + ( ( time1 - time2 ) * weight );

	// sync speed
	m_animTrack[0].setSpeedRatio( time1/ratio );
	m_animTrack[0].setAnimWeight( weight );

	m_animTrack[1].setSpeedRatio( time2/ratio );
	m_animTrack[1].setAnimWeight( 1.0f - weight );
#endif
}

// update
// update per frame
void CGameAnimation::update(float timeStep)
{
	bool startNewLoop = true;

	for ( int i = 0; i < MAX_ANIMTRACK; i++ )
	{
		if ( m_animTrack[i].isEnable() )
		{
			m_animTrack[i].update( timeStep );

			// the new track need wait another end track
			if ( m_animTrack[i].isEndTrack() == false )
				startNewLoop = false;
		}
	}

	// we will synchronized all animation loop
	if ( startNewLoop )
	{
		for ( int i = 0; i < MAX_ANIMTRACK; i++ )
		{
			if ( m_animTrack[i].isEnable() )
			{
				if ( m_animTrack[i].isLoop() )
					m_animTrack[i].setCurrentFrame( 0.0f );
				else if ( m_animTrack[i].isCrossAnim() )
					m_animTrack[i].crossAnimation();
			}
		}
	}
}

#pragma endregion


#pragma region SKINTECHNICAL

//////////////////////////////////////////////////////////
// CSkinTechincalCallBack implement
//////////////////////////////////////////////////////////

static s32 s_skinTechnical = -1;
static CGameColladaSceneNode	*s_sceneNodeRendering = NULL;
static SMaterial				s_materialRendering;

class CSkinTechincalCallBack : public video::IShaderConstantSetCallBack
{
private:
	int textureID;

public:
    CSkinTechincalCallBack()
    {
		// default use TEXTURE0
		textureID = 0;
    }

    virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
    {		
		video::IVideoDriver* driver = services->getVideoDriver();			
		
		core::matrix4 worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(ETS_WORLD);
		
		// projection
		services->setVertexShaderConstant("uMvpMatrix", worldViewProj.pointer(), 16);

		// bone
		if ( services->setVertexShaderConstant("uBoneMatrix", s_sceneNodeRendering->BoneMatrix, 16*MAX_BONEMATRIX ) == false )
			services->setVertexShaderConstant("uBoneMatrix[0]", s_sceneNodeRendering->BoneMatrix, 16*MAX_BONEMATRIX );		

		// texture
		services->setPixelShaderConstant("uTextureUnit0", (float*)&textureID, 1);		

		// material		
		SColorf ColorMaterial   = s_materialRendering.AmbientColor;
		services->setPixelShaderConstant("uMaterialAmbient", (float*)&ColorMaterial, 4);

		ColorMaterial   = s_materialRendering.EmissiveColor;
		services->setPixelShaderConstant("uMaterialEmission", (float*)&ColorMaterial, 4);

		ColorMaterial   = s_materialRendering.DiffuseColor;
		services->setPixelShaderConstant("uMaterialDiffuse", (float*)&ColorMaterial, 4);

		ColorMaterial   = s_materialRendering.SpecularColor;
		services->setPixelShaderConstant("uMaterialSpecular", (float*)&ColorMaterial, 4);

		float shininess  = s_materialRendering.Shininess;
		services->setPixelShaderConstant("uMaterialShininess", (float*)&shininess, 1);	
		
		//lighting
		u32 cnt = driver->getDynamicLightCount();
			
		core::matrix4 invWorld;
		driver->getTransform(ETS_WORLD).getInverse(invWorld);

		// support
		const int kDirectionLight	= 2;
		const int kPointLight		= 4;

		int nLightDirection = 0;
		int nLightPoint		= 0;
		
		SVec4			lightDirection		[MAX_LIGHTS];
		SColorf			lightAmbient		[MAX_LIGHTS];
		SColorf			lightDiffuse		[MAX_LIGHTS];
		SColorf			lightSpecular		[MAX_LIGHTS];
		
		SVec4			pointLightPosition	[MAX_POINTLIGHTS];
		SColorf			pointLightAmbient	[MAX_POINTLIGHTS];
		SColorf			pointLightDiffuse	[MAX_POINTLIGHTS];
		SColorf			pointLightSpecular	[MAX_POINTLIGHTS];
		core::vector3df pointLightAttenuation[MAX_POINTLIGHTS];
		SColorf			ambientColor;

		for ( size_t i = 0; i < 8; ++i )
		{
			if ( i < cnt )
			{
				video::SLight light;
				light = driver->getDynamicLight( i );

				switch ( light.Type )
				{
					case ELT_DIRECTIONAL:
						if ( nLightDirection < kDirectionLight )
						{
							invWorld.rotateVect(( f32* )&lightDirection[nLightDirection],light.Direction );
							lightAmbient[nLightDirection]		= light.AmbientColor;
							lightDiffuse[nLightDirection]		= light.DiffuseColor;
							lightSpecular[nLightDirection]		= light.SpecularColor;

							nLightDirection++;
						}
						break;				
					case ELT_POINT:
						if ( nLightPoint < kPointLight )
						{
							invWorld.transformVect(( f32* )&pointLightPosition[nLightPoint],light.Position );
							pointLightAttenuation[nLightPoint]	= light.Attenuation;
							pointLightAmbient[nLightPoint]		= light.AmbientColor;
							pointLightDiffuse[nLightPoint]		= light.DiffuseColor;
							pointLightSpecular[nLightPoint]		= light.SpecularColor;
							pointLightAttenuation[nLightPoint]	= light.Attenuation;

							nLightPoint++;
						}
						break;
					default:
						break;
				}				
			}			
		}
		
		// direction light 1
		services->setPixelShaderConstant( "uLightDirection1",	(float*)&lightDirection[0],	4 );		
		services->setPixelShaderConstant( "uLightDiffuse1",		(float*)&lightDiffuse[0],	4 );
		services->setPixelShaderConstant( "uLightSpecular1",	(float*)&lightSpecular[0],	4 );

		// direction light 2
		services->setPixelShaderConstant( "uLightDirection2",	(float*)&lightDirection[1],	4 );
		services->setPixelShaderConstant( "uLightDiffuse2",		(float*)&lightDiffuse[1],	4 );
		services->setPixelShaderConstant( "uLightSpecular2",	(float*)&lightSpecular[1],	4 );

		// point light 1
		services->setPixelShaderConstant( "uPointLightPosition1",		(float*)&pointLightPosition[0],		4 );
		services->setPixelShaderConstant( "uPointLightDiffuse1",		(float*)&pointLightDiffuse[0],		4 );
		services->setPixelShaderConstant( "uPointLightSpecular1",		(float*)&pointLightSpecular[0],		4 );
		services->setPixelShaderConstant( "uPointLightAttenuation1",	(float*)&pointLightAttenuation[0],	4 );

		// point light 2
		services->setPixelShaderConstant( "uPointLightPosition2",		(float*)&pointLightPosition[1],		4 );
		services->setPixelShaderConstant( "uPointLightDiffuse2",		(float*)&pointLightDiffuse[1],		4 );
		services->setPixelShaderConstant( "uPointLightSpecular2",		(float*)&pointLightSpecular[1],		4 );
		services->setPixelShaderConstant( "uPointLightAttenuation2",	(float*)&pointLightAttenuation[1],	4 );

		// point light 3
		services->setPixelShaderConstant( "uPointLightPosition3",		(float*)&pointLightPosition[2],		4 );
		services->setPixelShaderConstant( "uPointLightDiffuse3",		(float*)&pointLightDiffuse[2],		4 );
		services->setPixelShaderConstant( "uPointLightSpecular3",		(float*)&pointLightSpecular[2],		4 );
		services->setPixelShaderConstant( "uPointLightAttenuation3",	(float*)&pointLightAttenuation[2],	4 );


		ambientColor = getIView()->getSceneMgr()->getAmbientLight();
		services->setPixelShaderConstant( "uAmbientColor", (float*)&ambientColor, 4 );
    }

};

#pragma endregion


//////////////////////////////////////////////////////////
// CGameColladaSceneNode implement
//////////////////////////////////////////////////////////

CGameColladaSceneNode::CGameColladaSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, bool hardwareSkinning)
	:ISceneNode( parent, mgr, id )
{	
	m_isRootColladaNode = true;
	m_enableAnim = true;	
	m_timer = 0;
	
	m_isSkydome = false;
	m_terrainNode = false;
	m_hideTerrainNode = false;	

	ColladaMesh = NULL;	
	m_component = NULL;
	
	m_animationCallback		= NULL;
	

#ifdef GSANIMATION
	m_isShowName = false;
	m_renderRotate = 0;
	m_isHardwareSkinning = false;
#else	
	#ifdef _IRR_COMPILE_WITH_OGLES1_
		m_isHardwareSkinning = false;
	#else
		m_isHardwareSkinning = hardwareSkinning;
	#endif

#endif

	Box.MinEdge = core::vector3df(-2, -2, -2);
	Box.MaxEdge = core::vector3df( 2,  2,  2);
}

CGameColladaSceneNode::~CGameColladaSceneNode()
{
	if ( ColladaMesh )
		ColladaMesh->drop();	
}

// setColladaMesh
// Set current collada mesh
void CGameColladaSceneNode::setColladaMesh(CGameColladaMesh* mesh)
{
	if ( ColladaMesh )
		ColladaMesh->drop();

	ColladaMesh = mesh;
	Box = ColladaMesh->getBoundingBox();

	if ( ColladaMesh )
		ColladaMesh->grab();

	// config shader if use hardware for skinning
	if ( m_isHardwareSkinning && mesh->IsStaticMesh == false )
	{		
		if ( s_skinTechnical == -1 )
		{
			ISceneManager *mgr = getIView()->getSceneMgr();
			CSkinTechincalCallBack* skinTechnicalCB = new CSkinTechincalCallBack();

			s_skinTechnical = mgr->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles
			(
				getIView()->getPath("shader/extern/skinCharacter.vsh"), "vertexMain",	video::EVST_VS_1_1,
				getIView()->getPath("shader/extern/skinCharacter.fsh"), "pixelMain",	video::EPST_PS_1_1,
				skinTechnicalCB, 
				video::EMT_SOLID
			);

			skinTechnicalCB->drop();
		}

		for (u32 i = 0; i < ColladaMesh->getMeshBufferCount(); i++ )
		{
			ColladaMesh->getMeshBuffer(i)->getMaterial().MaterialType = (E_MATERIAL_TYPE)s_skinTechnical;
			ColladaMesh->getMeshBuffer(i)->setHardwareMappingHint( EHM_STATIC );
		}		

	}
}

// setSkydome
// set scenenode is skydome
void CGameColladaSceneNode::setSkydome( bool b )
{
	m_isSkydome = b;

	for (u32 i=0; i<ColladaMesh->getMeshBufferCount(); ++i)
	{
		scene::IMeshBuffer* mb = ColladaMesh->getMeshBuffer(i);
		if (mb)
		{
			video::SMaterial& material = mb->getMaterial();
			
			if ( m_isSkydome )
			{
				material.ZBuffer = false;
				material.ZWriteEnable = false;
			}
			else
			{
				material.ZBuffer = true;
				material.ZWriteEnable = true;
			}
		}
	}
}

void CGameColladaSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		if ( m_isSkydome == true )
			SceneManager->registerNodeForRendering(this, ESNRP_SKY_BOX);
		else
		{	
			if ( ColladaMesh )
			{
				bool addToTransparent = false;
				bool addToSolid = false;

				for (u32 i=0; i<ColladaMesh->getMeshBufferCount(); ++i)
				{
					scene::IMeshBuffer* mb = ColladaMesh->getMeshBuffer(i);
					if (mb)
					{
						const video::SMaterial& material = mb->getMaterial();
						if ( material.isTransparent() && addToTransparent == false )
						{
							SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT );
							addToTransparent = true;
						}
						else if ( addToSolid == false )
						{
							SceneManager->registerNodeForRendering(this, ESNRP_SOLID );
							addToSolid = true;
						}
					}
				}
			}
#ifdef GSANIMATION
			else
				SceneManager->registerNodeForRendering(this);
#endif
		}
	}
	ISceneNode::OnRegisterSceneNode();
	
}

void CGameColladaSceneNode::updateAbsolutePosition()
{
	core::matrix4 RelativeMatrix;
	RelativeMatrix.setbyproduct_nocheck(getRelativeTransformation(),AnimationMatrix);
	
	// calc absolute transform
	AbsoluteTransformation.setbyproduct_nocheck(Parent->getAbsoluteTransformation(),RelativeMatrix);

	// calc absolute animation
	if ( m_isRootColladaNode == true )
		AbsoluteAnimationMatrix = RelativeMatrix;
	else
		AbsoluteAnimationMatrix.setbyproduct_nocheck(((CGameColladaSceneNode*)Parent)->AbsoluteAnimationMatrix, RelativeMatrix);
}


// enableAnimOnAllChild
// enable/disable anim on all child
void CGameColladaSceneNode::enableAnimOnAllChild( bool b )
{
	std::stack<CGameColladaSceneNode*>	myStack;
	myStack.push( this );
	while ( myStack.size() > 0 )
	{
		CGameColladaSceneNode *n = myStack.top();
		myStack.pop();

		n->setEnableAnim( true );

		const core::list<ISceneNode*>& childs = (core::list<ISceneNode*>)n->getChildren();
		core::list<ISceneNode*>::ConstIterator i = childs.begin(), end = childs.end();
		while ( i != end )
		{
			myStack.push( (CGameColladaSceneNode*)(*i) );
			i++;
		}
	}
}

void CGameColladaSceneNode::OnAnimate(u32 timeMs)
{		
	// reset timer
	if ( m_timer == 0 )
	{
		m_timer = timeMs;		
	}
	else
	{
		// update animation translate
		if ( m_enableAnim == true )
		{	
			// update anim track
			float timeStep = (float)(timeMs - m_timer);
			m_gameAnimation.update( timeStep );			

			// update anim animation
			updateAnimation();	
		}
		
		// save current time
		m_timer = timeMs;
	}	

	// animate this node with all animators
	ISceneNodeAnimatorList::Iterator ait = Animators.begin(), end = Animators.end();
	while (ait != end)
	{				
		(*ait)->animateNode(this, timeMs);
		++ait;
	}

	// update absolute position
	updateAbsolutePosition();

}

// skin
// skin mesh
void CGameColladaSceneNode::skin()
{
	if ( ColladaMesh == NULL || ColladaMesh->IsStaticMesh == true || IsVisible == false )
		return;	
	
	// array joint
	int nJoint = ColladaMesh->Joints.size();	
	CGameColladaMesh::SJoint	*arrayJoint = ColladaMesh->Joints.pointer();
	CGameColladaMesh::SJoint	*pJoint = NULL;

	// calc joint matrix
	for ( int i = 0; i < nJoint; i++)
	{
		pJoint = &arrayJoint[i];
		core::matrix4 mat;
		mat.setbyproduct( pJoint->node->AbsoluteAnimationMatrix, pJoint->globalInversedMatrix );
		pJoint->skinningMatrix.setbyproduct( mat, ColladaMesh->BindShapeMatrix );	

		// set bone matrix
		memcpy(BoneMatrix + i*16, pJoint->skinningMatrix.pointer(), 16*sizeof(float));
	}
	
	if ( m_isHardwareSkinning == false )
	{
		int lastVertexTransform = 0;
		int jointIndex = 0;

		// skinning all mesh buffer
		int nMeshBuffer = ColladaMesh->getMeshBufferCount();
		for ( int mesh = 0; mesh < nMeshBuffer; mesh++ )
		{
			SColladaMeshBuffer *meshBuffer	= (SColladaMeshBuffer*)ColladaMesh->getMeshBuffer(mesh);			
			
			// get vertex buffer
			S3DVertexSkin	*vertex	= (S3DVertexSkin*)meshBuffer->getVertices();
			
			// skinning in vertex
			core::vector3df thisVertexMove, thisNormalMove;
				
			// get vertex position
			int begin	= meshBuffer->beginVertex;
			int end		= meshBuffer->endVertex;
			if ( begin < 0 )
				begin = 0;
			if ( end < 0 || end > (int)meshBuffer->getVertexCount() )
				end = meshBuffer->getVertexCount() - 1;

			if ( begin < lastVertexTransform )
				begin = lastVertexTransform;

			// seek to vertex buffer
			vertex += begin;

			core::vector3df positionCumulator;
			core::vector3df normalCumulator;
			core::matrix4	netMatrix;

			// skinning
			for ( int i = begin; i <= end; i++, vertex++ )
			{
				/*
				pJoint = &arrayJoint[ (int)vertex->BoneIndex.X ];
				netMatrix = pJoint->skinningMatrix * vertex->BoneWeight.X;

				pJoint = &arrayJoint[ (int)vertex->BoneIndex.Y ];
				netMatrix += pJoint->skinningMatrix * vertex->BoneWeight.Y;

				pJoint = &arrayJoint[ (int)vertex->BoneIndex.Z ];
				netMatrix += pJoint->skinningMatrix * vertex->BoneWeight.Z;

				pJoint = &arrayJoint[ (int)vertex->BoneIndex.W ];
				netMatrix += pJoint->skinningMatrix * vertex->BoneWeight.W;

				netMatrix.transformVect( vertex->Pos, vertex->StaticPos );
				netMatrix.rotateVect( vertex->Normal, vertex->StaticNormal );
				*/
		
				positionCumulator.set(0,0,0);
				normalCumulator.set(0,0,0);

				// bone 0
				if ( vertex->BoneWeight.X > 0.0f )
				{
					pJoint = &arrayJoint[ (int)vertex->BoneIndex.X ];
					pJoint->skinningMatrix.transformVect	(thisVertexMove, vertex->StaticPos);
					pJoint->skinningMatrix.rotateVect		(thisNormalMove, vertex->StaticNormal);
					thisVertexMove *= vertex->BoneWeight.X;
					thisNormalMove *= vertex->BoneWeight.X;
					positionCumulator	+= thisVertexMove;
					normalCumulator		+= thisNormalMove;
				}

				// bone 1
				if ( vertex->BoneWeight.Y > 0.0f )
				{
					pJoint = &arrayJoint[ (int)vertex->BoneIndex.Y ];
					pJoint->skinningMatrix.transformVect	(thisVertexMove, vertex->StaticPos);
					pJoint->skinningMatrix.rotateVect		(thisNormalMove, vertex->StaticNormal);
					thisVertexMove *= vertex->BoneWeight.Y;
					thisNormalMove *= vertex->BoneWeight.Y;
					positionCumulator	+= thisVertexMove;
					normalCumulator		+= thisNormalMove;
				}

				// bone 2
				if ( vertex->BoneWeight.Z > 0.0f )
				{
					pJoint = &arrayJoint[ (int)vertex->BoneIndex.Z ];
					pJoint->skinningMatrix.transformVect	(thisVertexMove, vertex->StaticPos);
					pJoint->skinningMatrix.rotateVect		(thisNormalMove, vertex->StaticNormal);
					thisVertexMove *= vertex->BoneWeight.Z;
					thisNormalMove *= vertex->BoneWeight.Z;
					positionCumulator	+= thisVertexMove;
					normalCumulator		+= thisNormalMove;
				}

				// bone 3
				if ( vertex->BoneWeight.W > 0.0f )
				{
					pJoint = &arrayJoint[ (int)vertex->BoneIndex.W ];
					pJoint->skinningMatrix.transformVect	(thisVertexMove, vertex->StaticPos);
					pJoint->skinningMatrix.rotateVect		(thisNormalMove, vertex->StaticNormal);
					thisVertexMove *= vertex->BoneWeight.W;
					thisNormalMove *= vertex->BoneWeight.W;
					positionCumulator	+= thisVertexMove;
					normalCumulator		+= thisNormalMove;
				}

				// apply skin pos & normal
				vertex->Pos		= positionCumulator;
				vertex->Normal	= normalCumulator;			
			}
			
			lastVertexTransform = end + 1;

			meshBuffer->recalculateBoundingBox();
			meshBuffer->setDirty( EBT_VERTEX );	

			if ( mesh == 0 )
				ColladaMesh->BoundingBox = meshBuffer->getBoundingBox();
			else
				ColladaMesh->BoundingBox.addInternalBox(meshBuffer->getBoundingBox());

		}	// for all mesh buffer

	}	// if hardware	
}

// getCurrentFrameData
// get current frame
void CGameColladaSceneNode::getCurrentFrameData( core::vector3df& position, core::quaternion& rotation, core::vector3df& scale )
{
	m_gameAnimation.getFrameData( position, scale, rotation, LocalMatrix );
}

// updateAnimation
// calc relative matrix of animation
void CGameColladaSceneNode::updateAnimation()
{
	core::vector3df position;
	core::vector3df scale;
	core::quaternion rotation;
	
	m_gameAnimation.getFrameData( position, scale, rotation, LocalMatrix );

	// run callback
	if ( m_animationCallback )
		m_animationCallback->_onUpdateFrameData( this,  position, scale, rotation );

	// rotation
	core::matrix4 mat;
	rotation.getMatrix_transposed(mat);
	
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

void CGameColladaSceneNode::render()
{
	// get driver
	IVideoDriver* driver = getSceneManager()->getVideoDriver();	
	IView *pView = getIView();	

	// set current render
	s_sceneNodeRendering = this;

	if ( ColladaMesh )
	{		

#ifdef GSGAMEPLAY
		// do not render terrain node, 
		// but we still setvisible for run function OnAnimate(update position)
		if ( m_hideTerrainNode == false )
#endif
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
						s_materialRendering = material;

						driver->setMaterial(material);						
						driver->drawMeshBuffer(mb);
					}
				}
			}

		}

		Box = ColladaMesh->getBoundingBox();

		// draw normals
		/*
		video::SMaterial m;
		m.Lighting = false;
		m.AntiAliasing=0;
		driver->setMaterial(m);

		core::vector3df normalizedNormal;
		const f32 DebugNormalLength = SceneManager->getParameters()->getAttributeAsFloat(DEBUG_NORMAL_LENGTH);
		const video::SColor DebugNormalColor = SColor(255,255,0,0);

		for (u32 g=0; g< ColladaMesh->getMeshBufferCount(); ++g)
		{
			const scene::IMeshBuffer* mb = ColladaMesh->getMeshBuffer(g);
			const u32 vSize = video::getVertexPitchFromType(mb->getVertexType());
			const video::S3DVertex* v = ( const video::S3DVertex*)mb->getVertices();
			const bool normalize = mb->getMaterial().NormalizeNormals;

			for (u32 i=0; i != mb->getVertexCount(); ++i)
			{
				normalizedNormal = v->Normal;
				if (normalize)
					normalizedNormal.normalize();

				driver->draw3DLine(v->Pos, v->Pos + (normalizedNormal * DebugNormalLength), DebugNormalColor);

				v = (const video::S3DVertex*) ( (u8*) v+vSize );
			}
		}
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		*/
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

		float size = 2.0f;

		Box.MinEdge = core::vector3df(-size, -size, -size);
		Box.MaxEdge = core::vector3df( size,  size,  size);

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

#ifdef GSANIMATION
		if ( m_isShowName )
		{
			driver->setTransform(video::ETS_WORLD, AbsoluteTransformation );
			driver->draw3DBox( getBoundingBox(), video::SColor(255,255,255,0));
			
			// circle point
			core::vector3df circle[20];			

			// draw rotY
			if ( (m_renderRotate & 1) == 1 )
			{
				buildRotPoint( circle, 20, 20.0f, 0 );
				renderListPoint( circle, 20, SColor(255,0,255,0) );
			}

			// draw rotX
			if ( (m_renderRotate & 2) == 2 )
			{
				buildRotPoint( circle, 20, 30.0f, 1 );
				renderListPoint( circle, 20, SColor(255,255,0,0) );
			}

			// draw rotZ
			if ( (m_renderRotate & 4) == 4 )
			{
				buildRotPoint( circle, 20, 40.0f, 2 );
				renderListPoint( circle, 20, SColor(255,0,0,255) );
			}

			renderOxyz();
		}
		else
		{
			driver->setTransform(video::ETS_WORLD, AbsoluteTransformation );
			driver->draw3DBox( getBoundingBox(), video::SColor(255,255,255,255));
		}
#else
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation );
		driver->draw3DBox( getBoundingBox(), video::SColor(255,255,255,255));
#endif
		//core::matrix4 mat = GlobalInversedMatrix;
		//mat.makeInverse();
		//driver->setTransform(video::ETS_WORLD, mat );
		//driver->draw3DBox( getBoundingBox(), video::SColor(255,100,100,100));
	}
	
}

#ifdef GSANIMATION

// renderOxyz
// draw oxyz	
void CGameColladaSceneNode::renderOxyz()
{
	IVideoDriver* driver = getSceneManager()->getVideoDriver();	
	
	core::vector3df ox(50,0,0);
	core::vector3df oy(0,60,0);
	core::vector3df oz(0,0,70);


	// set transform
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation );
	driver->draw3DLine( core::vector3df(), ox, SColor(255,255,0,0) );
	driver->draw3DLine( core::vector3df(), oy, SColor(255,0,255,0) );
	driver->draw3DLine( core::vector3df(), oz, SColor(255,0,0,255) );
	
}

// getHitState
// get hit on rotate
// return -1: none
// return 0: y
// return 1: x
// return 2: z
int CGameColladaSceneNode::getHitState( int x, int y )
{
	// circle point
	core::vector3df circle[20];			

	// draw rotY
	buildRotPoint( circle, 20, 20.0f, 0 );
	if ( isHitOnListPoint( circle, 20, x,y ) == true )
		return 0;

	// draw rotX
	buildRotPoint( circle, 20, 30.0f, 1 );
	if ( isHitOnListPoint( circle, 20, x,y ) == true )
		return 1;

	// draw rotZ
	buildRotPoint( circle, 20, 40.0f, 2 );
	if ( isHitOnListPoint( circle, 20, x,y ) == true )
		return 2;

	return -1;
}
	

// setRenderRotateFlag
// set flag
void CGameColladaSceneNode::setRenderRotateFlag( int flag )
{
	m_renderRotate = flag;
}

// renderListPoint
// draw a point to multi point
void CGameColladaSceneNode::renderListPoint(core::vector3df *point, int nPoint, SColor color)
{
	IVideoDriver* driver = getSceneManager()->getVideoDriver();

	driver->draw3DLine( point[0], point[nPoint-1], color );
	for ( int i = 0; i < nPoint - 1; i++ )
		driver->draw3DLine( point[i], point[i+1], color );
}


// buildRotPoint
// build list 
// type = 0: rotY, 1: rotX, 2: rotZ
void CGameColladaSceneNode::buildRotPoint( core::vector3df *point, int nPoint, float radius, int type )
{
	float	rad = 0;
	float	radInc = core::PI * 2/nPoint;
	
	core::vector3df	point1;

	for ( int i = 0; i < nPoint; i++ )
	{		
		if ( type == 0 )
		{
			// rotY
			point[i].Y = 0;
			point[i].X = radius * sin( rad );
			point[i].Z = radius * cos( rad );
		}
		else if ( type == 1 )
		{
			// rotX
			point[i].Y = radius * sin( rad );
			point[i].X = 0;
			point[i].Z = radius * cos( rad );
		}
		else
		{
			// rotZ
			point[i].Y = radius * sin( rad );
			point[i].X = radius * cos( rad );
			point[i].Z = 0;
		}

		rad = rad + radInc;		
	}
}

// isHitOnListPoint
// check hit on list point
bool CGameColladaSceneNode::isHitOnListPoint(core::vector3df *point, int nPoint, int x, int y)
{
	IView *pView = getIView();

	core::vector3df p1 = point[0];
	core::vector3df p2 = point[nPoint-1];

	int x1, y1, x2, y2;
	
	AbsoluteTransformation.transformVect( p1 );
	AbsoluteTransformation.transformVect( p2 );

	pView->getScreenCoordinatesFrom3DPosition( p1, &x1, &y1 );
	pView->getScreenCoordinatesFrom3DPosition( p2, &x2, &y2 );
	if ( isLineHit( x1, y1, x2, y2, x, y ) == true )
		return true;

	for ( int i = 0; i < nPoint - 1; i++ )
	{
		p1 = point[i];
		p2 = point[i+1];
		
		AbsoluteTransformation.transformVect( p1 );
		AbsoluteTransformation.transformVect( p2 );

		pView->getScreenCoordinatesFrom3DPosition( p1, &x1, &y1 );
		pView->getScreenCoordinatesFrom3DPosition( p2, &x2, &y2 );

		if ( isLineHit( x1, y1, x2, y2, x, y ) == true )
			return true;
	}

	return false;
}

bool CGameColladaSceneNode::isLineHit( int X1, int Y1, int X2, int Y2, int X, int Y )
{
	float x1 = (float)X1;
	float y1 = (float)Y1;

	float x2 = (float)X2;
	float y2 = (float)Y2;

	float xHit = (float)X;
	float yHit = (float)Y;
	
	const float constSelect = 5.0f;

	if ( x1 > x2 )
	{
		if ( xHit < (x2 - constSelect) || xHit > (x1 + constSelect) )
			return false;
	}
	else
	{
		if ( xHit < (x1 - constSelect) || xHit > (x2 + constSelect) )
			return false;
	}
	
	if ( y1 > y2 )
	{
		if ( yHit < (y2 - constSelect) || yHit > (y1 + constSelect) )
			return false;
	}
	else
	{
		if ( yHit < (y1 - constSelect) || yHit > (y2 + constSelect) )
			return false;
	}


	core::vector2df v( x2 - x1, y2 - y1 );
	core::vector2df w( xHit - x1, yHit - y1 );

	#define dot(u,v)   ((float)u.dotProduct(v))
	#define norm(v)    sqrtf(dot(v,v))
			
	float c1 = dot(w,v);
	if ( c1 <= 0 )
	{
		core::vector2df r( xHit - x1, yHit - y1 );
		float distance = norm(r);
		
		if ( distance <= constSelect )
			return true;
	}

	float c2 = dot(v,v);	
	
	if ( c2 <= c1 )
	{
		core::vector2df r( xHit - x2, yHit - y2 );
		float distance = norm(r);
		
		if ( distance <= constSelect )
			return true;
	}

	float b = c1 / c2;
		
	core::vector2df Pb( x1 + b*v.X, y1 + b*v.Y );

	// Khoang cach giua point va edge
	core::vector2df r( xHit - Pb.X, yHit - Pb.Y );
	double distance = norm(r);				

	#undef dot
	#undef norm
	
	if ( distance <= constSelect )
		return true;

	return false;
}

#endif

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
			if ( m_isHardwareSkinning == true && newNode->m_isHardwareSkinning == true )
			{
				// static mesh
				newNode->ColladaMesh = ColladaMesh;
				newNode->ColladaMesh->grab();
			}
			else
			{
				// dynamic mesh
				CGameColladaMesh *mesh = ColladaMesh->clone();
				
				// update hardware skinning
				newNode->setColladaMesh( mesh );
			}
		}
	}

	newNode->m_isRootColladaNode = m_isRootColladaNode;	

	newNode->m_timer			= m_timer;
	newNode->m_isSkydome		= m_isSkydome;
	newNode->m_terrainNode		= m_terrainNode;
	newNode->m_hideTerrainNode	= m_hideTerrainNode;	

	int ref = newNode->getReferenceCount();
	
	return newNode;
}