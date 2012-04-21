#include "stdafx.h"
#include "CAnimMeshComponent.h"
#include "IView.h"

CAnimMeshComponent::CAnimMeshComponent( CGameObject *pObj )
	:IObjectComponent( pObj, (int)IObjectComponent::AnimMesh )
{
	m_animNode = NULL;
	m_animSpeed = 30.0f;	
}

CAnimMeshComponent::~CAnimMeshComponent()
{
}

// loadFromFile
// load anim object from file
void CAnimMeshComponent::loadFromFile( char *lpFilename )
{
	if ( m_animeshFile == lpFilename )
		return;

	m_animeshFile = lpFilename;

	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	ISceneManager *smgr = getIView()->getSceneMgr();
	
	// load mesh
	IAnimatedMesh *animMesh = smgr->getMesh( lpFilename );
	if ( animMesh == NULL )
		return;

	// turn off lighting
	int numMesh = animMesh->getMeshBufferCount();
	for ( int i = 0; i < numMesh; i++ )
		animMesh->getMeshBuffer(i)->getMaterial().Lighting = m_gameObject->isLighting();
	
	// create scene node
	m_animNode = new CGameAnimatedMeshSceneNode( m_gameObject, animMesh, m_gameObject->getParentSceneNode(), smgr );	
	m_animNode->setAnimationSpeed( m_animSpeed );	

#ifdef GSEDITOR	
	// add collision
	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox(m_animNode);
	m_animNode->setTriangleSelector(selector);
	selector->drop();
#endif

	// set node
	m_gameObject->m_node = m_animNode;

	// set visibke
	m_gameObject->m_node->setVisible( true );	

	// update position
	m_gameObject->updateNodePosition();
	m_gameObject->updateNodeRotation();
}

// loadAnimFile
// load animation bone from dae file
void CAnimMeshComponent::loadAnimFile( char *lpFileName )
{
	if ( m_animFile == lpFileName )
		return;

	m_animFile = lpFileName;
	
	IrrlichtDevice	*device = getIView()->getDevice();
	IVideoDriver	*driver = getIView()->getDriver();
	io::IFileSystem *fs = device->getFileSystem();

	io::IXMLReader *xmlRead = fs->createXMLReader( lpFileName );

	if ( xmlRead == NULL )
	{
#if defined(GSEDITOR) || defined(GSANIMATION)
		WCHAR appPath[MAX_PATH];
		char  appPathA[MAX_PATH];

		uiApplication::getAppPath(appPath, MAX_PATH);
		uiString::copy<char, WCHAR>( appPathA, appPath  );
								
		std::string path = appPathA;
		path += "\\";
		path += std::string(lpFileName);
								
		xmlRead = fs->createXMLReader( path.c_str() );

		if ( xmlRead == NULL )
			return;
#else
		return;	
#endif
	}
	
	char charBuffer[1024];	

	m_animationData.clear();
	m_animationName.clear();
	m_jointAnimation.clear();

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				core::stringw nodeName = xmlRead->getNodeName();

				// read anamation clip
				if ( core::stringw(L"animation_clip") == nodeName )
				{
					const wchar_t *idClip = xmlRead->getAttributeValue(L"id");
					uiString::copy<char, const wchar_t>( charBuffer, idClip );

					float start = xmlRead->getAttributeValueAsFloat(L"start");
					float end	= xmlRead->getAttributeValueAsFloat(L"end");

					SAnimClip	clip;

					clip.m_id = charBuffer;
					clip.m_time	= start;
					clip.m_duration = end - start;
					clip.m_loop = true;

					// add animation data
					m_animationData[ clip.m_id ] = clip;
					
					m_animationName.push_back( clip.m_id );
				}
				else if ( core::stringw(L"animation")  == nodeName )
				{					
					parseAnimationNode( xmlRead );				
				}
			}
			break;
		case io::EXN_ELEMENT_END:
			{
			}
			break;
		case io::EXN_TEXT:
			{
			}
			break;
		}
	}
	
	xmlRead->drop();
	
	applyAnimation();
}


// parseAnimationNode
// parse anim node
void CAnimMeshComponent::parseAnimationNode( io::IXMLReader *xmlRead )
{	
	std::wstring idNodeName = xmlRead->getAttributeValue(L"id");
	
	std::wstring jointName;
	bool isRotation = false;
	bool isTranslate = false;

	int pos = idNodeName.find( L"-rotation" );
	if ( pos > 0 )
	{
		isRotation = true;
		jointName = idNodeName.substr(0, pos);
	}
	else
	{
		pos = idNodeName.find( L"-translation" );
		if ( pos > 0 )
		{
			isTranslate = true;
			jointName = idNodeName.substr(0, pos);
		}
	}

	if ( isRotation == false && isTranslate == false )
		return;

	char stringBuffer[1024];
	uiString::copy<char, const wchar_t>( stringBuffer, jointName.c_str() );

	ArrayAnimationFrame& animation = m_jointAnimation[stringBuffer];
		
	int readState = 0;

	wstring arrayID;
	int count = 0;

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				core::stringw nodeName = xmlRead->getNodeName();

				if ( core::stringw(L"float_array") == nodeName )
				{
					arrayID = xmlRead->getAttributeValue(L"id");
					count = xmlRead->getAttributeValueAsInt(L"count");

					if ( (int)arrayID.find( L"-input-array" ) > 0 )
						readState = 1;
					else if ( (int)arrayID.find( L"-output-array" ) > 0 )
					{
						if ( isRotation )
							count = count/4;
						if ( isTranslate )
							count = count/3;

						readState = 2;
					}
				}
			}
		case io::EXN_ELEMENT_END:
			{
				core::stringw nodeName = xmlRead->getNodeName();
				if ( core::stringw(L"animation") == nodeName )
					return;
			}
			break;
		case io::EXN_TEXT:
			{
				if ( readState == 1 || readState == 2 )
				{
					const wchar_t *data = xmlRead->getNodeData();
					const wchar_t *p = data;
					
					char number[512];
					int i = 0;
					int frame = 0;

					float	arrayFloat[10];
					int		numArray = 0;
						
					while ( *p != NULL )
					{
						if ( *p == L' ' || *(p+1) == NULL  )
						{
							if ( *p == L' '  )
								number[i++] = NULL;
							else
							{
								number[i++] = (char) *p;
								number[i++] = NULL;
							}

							
							float f;
							sscanf(number,"%f", &f);

							// add to list
							arrayFloat[numArray++] = f;
					
							if ( (int)animation.size() < count )
								animation.push_back( SAnimFrame() );

							SAnimFrame& frameData = animation[frame];

							// read time
							if ( readState == 1 )
							{
								frameData.m_time = f;
								
								numArray = 0;
								frame++;
							}
							else
							{
								if ( isRotation && numArray == 4 )
								{				
									frameData.m_rotX = arrayFloat[0];
									frameData.m_rotY = arrayFloat[1];
									frameData.m_rotZ = arrayFloat[2];
									frameData.m_rotAngle = arrayFloat[3];
																			
									numArray = 0;
									frame++;
								}
								else if ( isTranslate && numArray == 3 )
								{
									frameData.m_translateX = arrayFloat[0];
									frameData.m_translateY = arrayFloat[1];
									frameData.m_translateZ = arrayFloat[2];									

									numArray = 0;
									frame++;
								}
							}

							i = 0;
							number[i] = NULL;
						}
						else
							number[i++] = (char) *p;

						p++;
					
					}
				}
				readState = 0;
			}
			break;
		}
	}
}

// applyAnimation
// apply Animation to skin joint
void CAnimMeshComponent::applyAnimation()
{
	if ( m_animNode == NULL )
		return;
	
	if ( m_animNode->getMesh()->getMeshType() != EAMT_SKINNED )
		return;

	ISkinnedMesh* mesh = (ISkinnedMesh*)m_animNode->getMesh();
	if ( mesh == NULL )
		return;
	
	core::array<ISkinnedMesh::SJoint*>&	allJoint = mesh->getAllJoints();
	int nJoints = allJoint.size();
	
	float deltaTime = 1.0f/m_animSpeed;
	int frame = 0;

	// get anim time data
	const SAnimClip& animClip = m_animationData["walk"];

	// get frame
	SAnimFrame animFrame;
	int atFrameID = 0;

	for ( int i = 0; i < nJoints; i++ )
	{
		ISkinnedMesh::SJoint* j = allJoint[i];
				
		j->PositionKeys.clear();
		j->ScaleKeys.clear();
		j->RotationKeys.clear();
				
		// get local matrix of skin joint
		const core::matrix4& mat =	j->LocalMatrix;	

		// get joint node
		JointAnimation::iterator it = m_jointAnimation.find( std::string(j->Name.c_str()) );
		if ( it != m_jointAnimation.end() )
		{
			ArrayAnimationFrame& arrayFrame = it->second;			
						
			ISkinnedMesh::SPositionKey	pos;
			ISkinnedMesh::SRotationKey	rot;

			frame = 0;

			float time	= animClip.m_time;
			float end	= animClip.m_time + animClip.m_duration;

			while ( time < end )
			{
				getFrameAtTime( &arrayFrame, time, &animFrame, &atFrameID );


				rot.frame = (float)frame;
				rot.rotation.fromAngleAxis(
						animFrame.m_rotAngle*core::DEGTORAD, 
						core::vector3df(animFrame.m_rotX, animFrame.m_rotZ, animFrame.m_rotY)
					);						

				pos.frame = (float)frame;
				pos.position.X = 0;//animFrame.m_translateY;
				pos.position.Y = 0;//animFrame.m_translateZ;
				pos.position.Z = 0;//animFrame.m_translateX;

				mat.transformVect( pos.position );

				j->RotationKeys.push_back( rot );
				j->PositionKeys.push_back( pos );

				time = time + deltaTime;
				frame++;
			}					

		}		
	}

	// update skin mesh
	mesh->useAnimationFrom( mesh );
	m_animNode->setFrameLoop(0, frame);

	// update current anim
	m_currentAnim = animClip;	
	m_currentAnim.m_frames = frame;
}

// getFrameAtTime
// get a frame at time
 bool CAnimMeshComponent::getFrameAtTime( ArrayAnimationFrame* frames, float time, SAnimFrame* outFrame, int *frameID )
{
	int nFrames = frames->size();

	int first = 0, last = nFrames - 2;
	int mid = 0;
		
	while (first <= last) 
	{
		mid = (first + last) / 2;

		if ( time > frames->at(mid).m_time && time > frames->at(mid + 1).m_time )
			first = mid + 1;
		else if ( time < frames->at(mid).m_time )
			last = mid - 1;
		else
		{
			SAnimFrame frame = frames->at(mid);
			SAnimFrame& nextFrame = frames->at(mid + 1);
			
			float dt = (nextFrame.m_time - frame.m_time);
			float t = time - frame.m_time;
			float f = t/dt;

			// set frame id
			*frameID = mid;

			// LINEAR calc
			frame.m_time		= time;

			frame.m_rotX		= frame.m_rotX;// + (nextFrame.m_rotX - frame.m_rotX)*f;
			frame.m_rotY		= frame.m_rotY;// + (nextFrame.m_rotY - frame.m_rotY)*f;
			frame.m_rotZ		= frame.m_rotZ;// + (nextFrame.m_rotZ - frame.m_rotZ)*f;
			frame.m_rotAngle	= frame.m_rotAngle;// + (nextFrame.m_rotAngle - frame.m_rotAngle)*f;
			
			frame.m_translateX	= frame.m_translateX + (nextFrame.m_translateX - frame.m_translateX)*f;
			frame.m_translateY	= frame.m_translateY + (nextFrame.m_translateY - frame.m_translateY)*f;
			frame.m_translateZ	= frame.m_translateZ + (nextFrame.m_translateZ - frame.m_translateZ)*f;

			*outFrame = frame;
			return true;
		}
	}

	*outFrame = frames->at(nFrames-1);
	*frameID = nFrames-1;

	return false;

}


// init
// run when init object
void CAnimMeshComponent::initComponent()
{

}

// updateObject
// update object by frame
void CAnimMeshComponent::updateComponent()
{
	
}

// saveData
// save data to serializable
void CAnimMeshComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	pObj->addPath("meshFile", m_animeshFile.c_str(), true);
	pObj->addFloat("animSpeed", m_animSpeed, true );
}

// loadData
// load data to serializable
void CAnimMeshComponent::loadData( CSerializable* pObj )
{		
	pObj->nextRecord();

	// read mesh file
	char *string = pObj->readString();
	loadFromFile( string );

	// read anim speed
	m_animSpeed = pObj->readFloat();
	if ( m_animNode )
		m_animNode->setAnimationSpeed( m_animSpeed );
}