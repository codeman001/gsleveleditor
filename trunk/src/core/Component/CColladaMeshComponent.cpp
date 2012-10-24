#include "stdafx.h"
#include "CColladaMeshComponent.h"
#include "IView.h"

#include "SAnimatedMesh.h"
#include "ISkinnedMesh.h"
#include "CSkinnedMesh.h"

#include "CGameAnimatedMeshSceneNode.h"
#include "CGameMeshSceneNode.h"
#include "CGameContainerSceneNode.h"

#include "CBinaryUtils.h"
#include "CDaeUtils.h"

#include "CGameCamera.h"
#ifdef GSGAMEPLAY
#include "gameLevel/CGameLevel.h"
#endif


#pragma region DAE_COLLADA_ANIMATION_IMPLEMENT

//////////////////////////////////////////////////////////
// CColladaAnimation implement
//////////////////////////////////////////////////////////

const float k_defaultAnimFPS = 40.0f;

CColladaAnimation::CColladaAnimation()
{
}

CColladaAnimation::~CColladaAnimation()
{
	// release all anim clip
	std::vector<SColladaAnimClip*>::iterator it = m_colladaAnim.begin(), end = m_colladaAnim.end();
	while ( it != end )
	{
		delete (*it);
		it++;
	}

	m_colladaAnim.clear();
	m_animWithName.clear();
}


// getFrameAtTime
// get a frame at time
bool CColladaAnimation::getRotationFrameID( SColladaNodeAnim* frames, float frame, int *frameRotID, core::quaternion *rotateData )
{	
	int nRotFrames = frames->RotationKeys.size();
		
	int first = 0, last = nRotFrames - 2;
	int mid = 0;
		
	while (first <= last)
	{
		mid = (first + last) / 2;

		if ( frame > frames->RotationKeys[mid].frame && frame > frames->RotationKeys[mid + 1].frame )
			first = mid + 1;
		else if ( frame < frames->RotationKeys[mid].frame )
			last = mid - 1;
		else
		{			
			CGameAnimationTrack::SRotationKey &frame1 = frames->RotationKeys[ mid ];
			CGameAnimationTrack::SRotationKey &frame2 = frames->RotationKeys[ mid + 1 ];

			core::quaternion q1, q2;
			q1 = frame1.rotation;
			q2 = frame2.rotation;

			float f = (frame - frame1.frame)/(frame2.frame - frame1.frame);

			// calc rotate
			rotateData->slerp( q1, q2, f );
	
			// set frame id
			*frameRotID = mid + 1;
			return true;
		}
	}
	*frameRotID = nRotFrames - 1;
	return false;
}

bool CColladaAnimation::getPositionFrameID( SColladaNodeAnim* frames, float frame, int *framePosID, core::vector3df  *positionData )
{
	int nPosFrames = frames->PositionKeys.size();
		
	int first = 0, last = nPosFrames - 2;
	int mid = 0;
		
	while (first <= last)
	{
		mid = (first + last) / 2;

		if ( frame > frames->PositionKeys[mid].frame && frame > frames->PositionKeys[mid + 1].frame )
			first = mid + 1;
		else if ( frame < frames->PositionKeys[mid].frame )
			last = mid - 1;
		else
		{			
			CGameAnimationTrack::SPositionKey &frame1 = frames->PositionKeys[ mid ];
			CGameAnimationTrack::SPositionKey &frame2 = frames->PositionKeys[ mid + 1 ];

			
			core::vector3df v1 = frame1.position;
			core::vector3df v2 = frame2.position;
					
			float f = (frame - frame1.frame)/(frame2.frame - frame1.frame);

			*positionData = v1 + (v2 - v1) * f;			

			// set frame id
			*framePosID = mid + 1;
			return true;
		}
	}

	*framePosID = nPosFrames - 1;
	return false;
}

bool CColladaAnimation::getScaleFrameID( SColladaNodeAnim* frames, float frame, int *frameScaleID, core::vector3df  *scaleData )
{
	int nScaleFrames = frames->ScaleKeys.size();
		
	int first = 0, last = nScaleFrames - 2;
	int mid = 0;
		
	while (first <= last)
	{
		mid = (first + last) / 2;

		if ( frame > frames->ScaleKeys[mid].frame && frame > frames->ScaleKeys[mid + 1].frame )
			first = mid + 1;
		else if ( frame < frames->ScaleKeys[mid].frame )
			last = mid - 1;
		else
		{			
			CGameAnimationTrack::SScaleKey &frame1 = frames->ScaleKeys[ mid ];
			CGameAnimationTrack::SScaleKey &frame2 = frames->ScaleKeys[ mid + 1 ];

			
			core::vector3df v1 = frame1.scale;
			core::vector3df v2 = frame2.scale;
					
			float f = (frame - frame1.frame)/(frame2.frame - frame1.frame);

			*scaleData = v1 + (v2 - v1) * f;			

			// set frame id
			*frameScaleID = mid + 1;
			return true;
		}
	}

	*frameScaleID = nScaleFrames - 1;
	return false;
}


// clippingDaeAnim
// clip a long clip to many clip
void CColladaAnimation::clipDaeAnim()
{
	if ( m_colladaAnim.size() == 0 )
	{
		// add clip
		SColladaAnimClip *clip = new SColladaAnimClip();

		char name[512];
		uiString::getFileNameNoExt<const char, char>( m_animFileName.c_str(), name );
		clip->animName	= name;
		clip->time		= 0;
		
		float frames = 0.0f;

		std::vector<SColladaNodeAnim*>::iterator iNodeAnim = m_globalClip.animInfo.begin(), iNodeEnd = m_globalClip.animInfo.end();

		while ( iNodeAnim != iNodeEnd )
		{
			SColladaNodeAnim* nodeAnim = (*iNodeAnim);
			SColladaNodeAnim* newNodeAnim = new SColladaNodeAnim();

			*newNodeAnim = *nodeAnim;
			
			if ( nodeAnim->PositionKeys.size() )
			{
				if ( frames < nodeAnim->PositionKeys.getLast().frame )
					frames = nodeAnim->PositionKeys.getLast().frame;
			}
			if ( nodeAnim->RotationKeys.size() )
			{
				if ( frames < nodeAnim->RotationKeys.getLast().frame )
					frames = nodeAnim->RotationKeys.getLast().frame;
			}
			if ( nodeAnim->ScaleKeys.size() )
			{
				if ( frames < nodeAnim->ScaleKeys.getLast().frame )
					frames = nodeAnim->ScaleKeys.getLast().frame;
			}

			clip->animInfo.push_back( newNodeAnim );
			iNodeAnim++;
		}

		// n frames
		clip->duration = frames;

		// add clip
		m_colladaAnim.push_back( clip );
	}
	else
	{
		// loop all animation clip
		std::vector<SColladaAnimClip*>::iterator it = m_colladaAnim.begin(), end = m_colladaAnim.end();
		while ( it != end )
		{
			SColladaAnimClip* clip = (*it);

			// loop all scenenode with anim
			std::vector<SColladaNodeAnim*>::iterator iNodeAnim = m_globalClip.animInfo.begin(), 
				iNodeEnd = m_globalClip.animInfo.end();

			while ( iNodeAnim != iNodeEnd )
			{
				SColladaNodeAnim* nodeAnim = (*iNodeAnim);			

				float currentFrame	= 0;
				float frameBegin	= clip->time;
				float frameEnd		= clip->time + clip->duration;
				
				core::vector3df v1, v2;
				core::quaternion q1, q2;

				int beginID, endID;

#pragma region ClipRotation
				// clip rotation
				getRotationFrameID( nodeAnim, frameBegin,	&beginID,	&q1 );
				getRotationFrameID( nodeAnim, frameEnd,		&endID,		&q2 );
				
				// add clip
				if ( beginID >= 0 && endID >= 0 )
				{
					SColladaNodeAnim *newNodeAnim = new SColladaNodeAnim();
					newNodeAnim->sceneNodeName = nodeAnim->sceneNodeName;

					// add new node anim
					clip->addNodeAnim( newNodeAnim );

					if ( beginID == endID )
					{													
						CGameAnimationTrack::SRotationKey rotKey;
						
						// frame1
						rotKey.frame = 0;
						rotKey.rotation = q1;
						newNodeAnim->RotationKeys.push_back( rotKey );

						// frame2
						rotKey.frame = frameEnd - frameBegin;
						rotKey.rotation = q2;
						newNodeAnim->RotationKeys.push_back( rotKey );				
					}
					else
					{
						for ( int i = beginID; i <= endID; i++ )
						{						
							CGameAnimationTrack::SRotationKey		rotKey;

							if ( i == beginID )
							{
								currentFrame = 0;							
								rotKey.rotation = q1;
							}
							else if ( i == endID )
							{
								currentFrame = frameEnd - frameBegin;
								rotKey.rotation = q2;
							}
							else
							{
								CGameAnimationTrack::SRotationKey&	animFrame = nodeAnim->RotationKeys[i];

								currentFrame =  animFrame.frame - frameBegin;
								rotKey.rotation = animFrame.rotation;
							}				
																			
							// add key frame
							rotKey.frame = currentFrame;
							newNodeAnim->RotationKeys.push_back( rotKey );	
						}
					}
				}
#pragma endregion

#pragma region ClipPosition
				// clip position
				getPositionFrameID( nodeAnim, frameBegin,	&beginID,	&v1 );
				getPositionFrameID( nodeAnim, frameEnd,		&endID,		&v2 );

				if ( beginID >= 0 && endID >= 0 )
				{
					SColladaNodeAnim *newNodeAnim = clip->getAnimOfSceneNode( nodeAnim->sceneNodeName.c_str() );
					if ( newNodeAnim == NULL )
					{
						newNodeAnim = new SColladaNodeAnim();
						newNodeAnim->sceneNodeName = nodeAnim->sceneNodeName;

						// add new node anim
						clip->addNodeAnim( newNodeAnim );
					}

					if ( beginID == endID )
					{													
						CGameAnimationTrack::SPositionKey posKey;
						
						// frame1
						posKey.frame = 0;
						posKey.position = v1;
						newNodeAnim->PositionKeys.push_back( posKey );

						// frame2
						posKey.frame = frameEnd - frameBegin;
						posKey.position = v2;
						newNodeAnim->PositionKeys.push_back( posKey );				
					}
					else
					{
						for ( int i = beginID; i <= endID; i++ )
						{						
							CGameAnimationTrack::SPositionKey		posKey;

							if ( i == beginID )
							{
								currentFrame = 0;							
								posKey.position = v1;
							}
							else if ( i == endID )
							{
								currentFrame = frameEnd - frameBegin;
								posKey.position = v2;
							}
							else
							{
								CGameAnimationTrack::SPositionKey&	animFrame = nodeAnim->PositionKeys[i];

								currentFrame	=  animFrame.frame - frameBegin;
								posKey.position = animFrame.position;
							}				
																			
							// add key frame
							posKey.frame = currentFrame;
							newNodeAnim->PositionKeys.push_back( posKey );	
						}
					}
				}
#pragma endregion

#pragma region ClipScale
				// clip position
				getScaleFrameID( nodeAnim, frameBegin,	&beginID,	&v1 );
				getScaleFrameID( nodeAnim, frameEnd,	&endID,		&v2 );

				if ( beginID >= 0 && endID >= 0 )
				{
					SColladaNodeAnim *newNodeAnim = clip->getAnimOfSceneNode( nodeAnim->sceneNodeName.c_str() );
					if ( newNodeAnim == NULL )
					{
						newNodeAnim = new SColladaNodeAnim();
						newNodeAnim->sceneNodeName = nodeAnim->sceneNodeName;

						// add new node anim
						clip->addNodeAnim( newNodeAnim );
					}

					if ( beginID == endID )
					{													
						CGameAnimationTrack::SScaleKey scaleKey;
						
						// frame1
						scaleKey.frame = 0;
						scaleKey.scale = v1;
						newNodeAnim->ScaleKeys.push_back( scaleKey );

						// frame2
						scaleKey.frame = frameEnd - frameBegin;
						scaleKey.scale = v2;
						newNodeAnim->ScaleKeys.push_back( scaleKey );				
					}
					else
					{
						for ( int i = beginID; i <= endID; i++ )
						{						
							CGameAnimationTrack::SScaleKey		scaleKey;

							if ( i == beginID )
							{
								currentFrame = 0;							
								scaleKey.scale = v1;
							}
							else if ( i == endID )
							{
								currentFrame = frameEnd - frameBegin;
								scaleKey.scale = v2;
							}
							else
							{
								CGameAnimationTrack::SScaleKey&	animFrame = nodeAnim->ScaleKeys[i];

								currentFrame	=	animFrame.frame - frameBegin;
								scaleKey.scale	=	animFrame.scale;
							}				
																			
							// add key frame
							scaleKey.frame = currentFrame;
							newNodeAnim->ScaleKeys.push_back( scaleKey );	
						}
					}
				}			
#pragma endregion

				iNodeAnim++;
			}

			clip->time = 0.0f;
			it++;
		}
	}
	m_globalClip.freeAllNodeAnim();	
}

// parseClipNode
// parse clip time node
void CColladaAnimation::parseClipNode( io::IXMLReader *xmlRead )
{
	char charBuffer[1024];

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

					// add clip
					SColladaAnimClip *clip = new SColladaAnimClip();
					clip->animName = charBuffer;
					clip->time = start*k_defaultAnimFPS;
					clip->duration = (end - start)*k_defaultAnimFPS;

					m_colladaAnim.push_back( clip );
				}
				break;
			}
		case io::EXN_ELEMENT_END:
			{
				core::stringw nodeName = xmlRead->getNodeName();

				if ( core::stringw(L"library_animation_clips") == nodeName )
					return;
			}		
		}
	}
}

// parseAnimationNode
// parse anim node
void CColladaAnimation::parseAnimationNode( io::IXMLReader *xmlRead )
{	
	std::wstring idNodeName = xmlRead->getAttributeValue(L"id");
	
	std::wstring jointName;
	bool isRotation = false;
	bool isTranslate = false;
	bool isScale = false;
	bool isMatrix = false;

	int pos = (int)idNodeName.find( L"-rotation" );
	if ( pos > 0 )
	{
		isRotation = true;
		jointName = idNodeName.substr(0, pos);
	}
	else
	{
		pos = (int)idNodeName.find( L"-translation" );
		if ( pos > 0 )
		{
			isTranslate = true;
			jointName = idNodeName.substr(0, pos);
		}
		else
		{
			pos = (int)idNodeName.find( L"-scale" );
			if ( pos > 0 )
			{
				isScale = true;
				jointName = idNodeName.substr(0, pos);
			}
			else
			{
				pos = (int)idNodeName.find( L"-anim" );
				if ( pos > 0 )
				{
					isMatrix = true;
					jointName = idNodeName.substr(0, pos);
				}
			}
		}
	}

	if ( isRotation == false && isTranslate == false && isScale == false && isMatrix == false )
	{		
		return;
	}

	char stringBuffer[1024];
	uiString::copy<char, const wchar_t>( stringBuffer, jointName.c_str() );
	
	// create anim node
	SColladaNodeAnim *nodeAnim = m_globalClip.getAnimOfSceneNode( stringBuffer );
	if ( nodeAnim == NULL )
	{
		nodeAnim = new SColladaNodeAnim();
		nodeAnim->sceneNodeName = stringBuffer;
	
		// add node anim
		m_globalClip.addNodeAnim( nodeAnim );	
	}

	int		readState = 0;
	
	float	*arrayTime = NULL;
	float	*arrayFloat = NULL;

	std::wstring arrayID;
	int count = 0;

	int nAnimationTags = 1;

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				core::stringw nodeName = xmlRead->getNodeName();

				if ( core::stringw(L"animation") == nodeName )
				{
					nAnimationTags++;
				}
				else if ( core::stringw(L"float_array") == nodeName )
				{
					arrayID = xmlRead->getAttributeValue(L"id");
					count = xmlRead->getAttributeValueAsInt(L"count");

					if ( (int)arrayID.find( L"-input-array" ) > 0 )
					{
						readState = 1;
						arrayTime = new float[count];						
					}
					else if ( 
							(int)arrayID.find( L"-output-array" ) > 0 ||
							(int)arrayID.find( L"-output-transform-array" ) > 0
						)
					{						
						readState = 2;
						arrayFloat = new float[count];
					}
				}
				else if ( core::stringw(L"accessor") == nodeName && arrayFloat != NULL )
				{
					int stride = xmlRead->getAttributeValueAsInt(L"stride");
					int nFrame = count/stride;

					float fvector[4];

					for ( int i = 0; i < nFrame; i++ )
					{						
						if ( isRotation && stride == 4 )
						{				
							if ( m_needFlip == true )
							{
								fvector[0] = arrayFloat[i*4];
								fvector[1] = arrayFloat[i*4 + 2];
								fvector[2] = arrayFloat[i*4 + 1];
								fvector[3] = arrayFloat[i*4 + 3];
							}
							else
							{
								fvector[0] = arrayFloat[i*4];
								fvector[1] = arrayFloat[i*4 + 1];
								fvector[2] = arrayFloat[i*4 + 2];
								fvector[3] = -arrayFloat[i*4 + 3];
							}

							CGameAnimationTrack::SRotationKey key;
							key.frame = arrayTime[i]*k_defaultAnimFPS;
							key.rotation.fromAngleAxis(
									fvector[3] * core::DEGTORAD, 
									core::vector3df(fvector[0], fvector[1], fvector[2])
								);
							nodeAnim->RotationKeys.push_back(key);
						}
						else if ( isTranslate )
						{				
							if ( stride == 3 )
							{
								if ( m_needFlip == true )
								{
									fvector[0] = arrayFloat[i*3];
									fvector[1] = arrayFloat[i*3 + 2];
									fvector[2] = arrayFloat[i*3 + 1];									
								}
								else
								{
									fvector[0] = arrayFloat[i*3];
									fvector[1] = arrayFloat[i*3 + 1];
									fvector[2] = arrayFloat[i*3 + 2];	
								}
					
								CGameAnimationTrack::SPositionKey key;
								key.frame = arrayTime[i]*k_defaultAnimFPS;
								key.position = core::vector3df(fvector[0], fvector[1], fvector[2] );
								nodeAnim->PositionKeys.push_back(key);
							}
							else
								printf("Warning: May be not support stride: %d on translate!\n", stride);
						}
						else if ( isScale )
						{
							if ( stride == 3 )
							{
								if ( m_needFlip == true )
								{
									fvector[0] = arrayFloat[i*3];
									fvector[1] = arrayFloat[i*3 + 2];
									fvector[2] = arrayFloat[i*3 + 1];									
								}
								else
								{
									fvector[0] = arrayFloat[i*3];
									fvector[1] = arrayFloat[i*3 + 1];
									fvector[2] = arrayFloat[i*3 + 2];	
								}

								CGameAnimationTrack::SScaleKey key;
								key.frame = arrayTime[i]*k_defaultAnimFPS;
								key.scale = core::vector3df(fvector[0], fvector[1], fvector[2] );
								nodeAnim->ScaleKeys.push_back(key);
							}
							else
								printf("Warning: May be not support stride: %d on scale!\n", stride);
						}
						else if ( isMatrix && stride == 16 )
						{	
							core::matrix4 mat;
							mat.setM( arrayFloat + i *16 );

							if ( m_needFlip )
							{
								core::matrix4 mat2( mat, core::matrix4::EM4CONST_TRANSPOSED);
								mat2[1]=mat[8];
								mat2[2]=mat[4];
								mat2[4]=mat[2];
								mat2[5]=mat[10];
								mat2[6]=mat[6];
								mat2[8]=mat[1];
								mat2[9]=mat[9];
								mat2[10]=mat[5];
								mat2[12]=mat[3];
								mat2[13]=mat[11];
								mat2[14]=mat[7];
								mat = mat2;
							}
							else
								mat = core::matrix4( mat, core::matrix4::EM4CONST_TRANSPOSED);
														
							CGameAnimationTrack::SRotationKey key;
							key.frame = arrayTime[i]*k_defaultAnimFPS;
							key.rotation = core::quaternion( mat );
							nodeAnim->RotationKeys.push_back(key);

							CGameAnimationTrack::SPositionKey keyPos;
							keyPos.frame = arrayTime[i]*k_defaultAnimFPS;
							keyPos.position = mat.getTranslation();
							nodeAnim->PositionKeys.push_back(keyPos);

							CGameAnimationTrack::SScaleKey keyScale;
							keyScale.frame = arrayTime[i]*k_defaultAnimFPS;
							keyScale.scale = mat.getScale();
							nodeAnim->ScaleKeys.push_back(keyScale);
						}
						else
						{
							printf("Warning: May be not support some animation!\n");
						}

					}

					delete arrayTime;
					arrayTime = NULL;

					delete arrayFloat;
					arrayFloat = NULL;
				}
			}
		case io::EXN_ELEMENT_END:
			{
				core::stringw nodeName = xmlRead->getNodeName();
				if ( core::stringw(L"animation") == nodeName )
				{
					nAnimationTags--;
					if ( nAnimationTags == 0 )
						return;
				}
			}
			break;
		case io::EXN_TEXT:
			{
				if ( readState == 1 || readState == 2 )
				{
					const wchar_t *data = xmlRead->getNodeData();
					
					wchar_t* p = (wchar_t*)data;
					wchar_t* begin = (wchar_t*)data;

					float value = 0;
					int numArrayTime = 0;
					int numArray = 0;


					int len = uiString::length<const wchar_t>( data );
					while ( *p && len > 0 )
					{				
						while(*p && !(*p==L' ' || *p==L'\n' || *p==L'\r' || *p==L'\t'))
						{
							++p;
							len--;
						}

						*p = NULL;

						if (*begin)
						{
							swscanf(begin,L"%f", &value);

							if ( readState == 1 )
							{
								// read time
								arrayTime[numArrayTime++] = value;								
							}
							else if ( readState == 2 )
							{
								// add to list
								arrayFloat[numArray++] = value;
							}
						}		

						p++;
						len--;
						begin = p;
					}		
				}
				readState = 0;
			}
			break;
		}
	}	

}

void CColladaAnimation::loadDae( const char *lpFileName )
{	
	IrrlichtDevice	*device = getIView()->getDevice();
	io::IFileSystem *fs = device->getFileSystem();

	m_animFileName = lpFileName;

	io::IXMLReader *xmlRead = fs->createXMLReader( lpFileName );

	if ( xmlRead == NULL )
	{			
		xmlRead = fs->createXMLReader( getIView()->getPath(lpFileName) );

		if ( xmlRead == NULL )
			return;
	}	

	m_needFlip = false;
	bool readLUpAxis = false;

	std::vector<SColladaAnimClip*> oldAnim = m_colladaAnim;

	m_colladaAnim.clear();	

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				core::stringw nodeName = xmlRead->getNodeName();
				if ( core::stringw(L"library_animation_clips") == nodeName )
				{
					parseClipNode( xmlRead );
				}				
				else if ( core::stringw(L"animation")  == nodeName )
				{					
					parseAnimationNode( xmlRead );				
				}
				else if ( nodeName == L"up_axis" )
				{
					readLUpAxis = true;
				}							
			}
			break;
		case io::EXN_ELEMENT_END:
			{
			}
			break;
		case io::EXN_TEXT:
			{
				if ( readLUpAxis == true )
				{
					std::wstring text = xmlRead->getNodeData();
					if ( text == L"Z_UP" )
						m_needFlip = true;
					else
						m_needFlip = false;

				}
				readLUpAxis = false;
			}
			break;
		}
	}
	
	xmlRead->drop();

	// clip global animation to frame
	clipDaeAnim();
	

	// need index name & anim
	m_animWithName.clear();
	
	m_colladaAnim.insert( m_colladaAnim.begin(), oldAnim.begin(), oldAnim.end() );
	for ( int i = 0, n = (int)m_colladaAnim.size(); i < n; i++ )
	{
		SColladaAnimClip *clip = m_colladaAnim[i];
		m_animWithName[ clip->animName ] = clip;
	}	
}

void CColladaAnimation::loadDotAnim( const char *lpFileName )
{
	// todo load file
	io::IReadFile *file = getIView()->getFileSystem()->createAndOpenFile( lpFileName );
	
	if ( file == NULL )
	{
		file = getIView()->getFileSystem()->createAndOpenFile( getIView()->getPath(lpFileName) );

		if ( file == NULL )
			return;
	}

	// parse binary scene
	CBinaryUtils::getInstance()->setCurrentComponent(NULL);
	CBinaryUtils::getInstance()->loadAnim( file, this );

	// close file
	file->drop();
}

void CColladaAnimation::loadFile( const char *lpFileName )
{
	char ext[10] = {0};
	uiString::getFileNameExt<const char, char>( lpFileName, ext );
	uiString::toLower( ext );
	
	if ( strcmp(ext, "dae") == 0 )
		loadDae( lpFileName );
	else if ( strcmp(ext,"anim") == 0 )
		loadDotAnim( lpFileName );	
}

void CColladaAnimation::removeClip( const std::string& clipName )
{
	SColladaAnimClip *clip = m_animWithName[ clipName ];
	if ( clip == NULL )
		return;

	std::vector<SColladaAnimClip*>::iterator it = m_colladaAnim.begin(), end = m_colladaAnim.end();
	while ( it != end )
	{
		if ( (*it) == clip )
		{
			delete (*it);
			m_colladaAnim.erase(it);
			m_animWithName[ clipName ] = NULL;
			return;
		}
		it++;
	}
}

void CColladaAnimation::cloneAnim( const char *lpAnimName, const char *lpNewAnimName )
{
	SColladaAnimClip *clip = m_animWithName[ lpAnimName ];
	if ( clip == NULL )
		return;

	SColladaAnimClip *newClip = new SColladaAnimClip();
	newClip->animName	= lpNewAnimName;
	newClip->time		= clip->time;
	newClip->duration	= clip->duration;
	newClip->loop		= clip->loop;

	for ( int i = 0, nNodeAnims = (int)clip->animInfo.size(); i < nNodeAnims; i++ )
	{
		SColladaNodeAnim* nodeAnim = new SColladaNodeAnim();
		*nodeAnim = *clip->animInfo[i];
		newClip->animInfo.push_back( nodeAnim );
	}

	removeClip( lpNewAnimName );
	addClip( newClip );
}

void CColladaAnimation::createSynchronizedAnim( const char *lpNewName, const char *lpAnim1, const char *lpAnim2, float w )
{
	SColladaAnimClip *clip1 = m_animWithName[ lpAnim1 ];
	SColladaAnimClip *clip2 = m_animWithName[ lpAnim2 ];
	if ( clip1 == NULL || clip2 == NULL || clip1->duration == 0.0f || clip2->duration == 0.0f )
		return;

	w = core::clamp<float>( w, 0.0f, 1.0f );
	float time1 = clip1->duration;
	float time2 = clip2->duration;
	 	
	// interpolate speed ratio 2 channel
	float time = time1 + ( ( time2 - time1 ) * w );

	SColladaAnimClip *newClip = new SColladaAnimClip();
	newClip->animName	= lpNewName;
	newClip->time		= clip1->time;
	newClip->duration	= time;
	newClip->loop		= true;

	for ( int i = 0, n = (int)clip1->animInfo.size(); i < n; i++ )
	{
		SColladaNodeAnim* nodeAnim1 = clip1->animInfo[i];
		SColladaNodeAnim* nodeAnim2 = clip2->getAnimOfSceneNode( nodeAnim1->sceneNodeName.c_str() );
		
		SColladaNodeAnim* newNode = new SColladaNodeAnim();

		// set name
		newNode->sceneNodeName = nodeAnim1->sceneNodeName;

		// set node info
		if ( nodeAnim2 == NULL )
		{
			float ratio = time/time1;

			for ( int key = 0, nKey = nodeAnim1->PositionKeys.size(); key < nKey; key++ )
			{
				newNode->PositionKeys.push_back( nodeAnim1->PositionKeys[key] );
				newNode->PositionKeys[key].frame = newNode->PositionKeys[key].frame * ratio;
			}

			for ( int key = 0, nKey = nodeAnim1->RotationKeys.size(); key < nKey; key++ )
			{
				newNode->RotationKeys.push_back( nodeAnim1->RotationKeys[key] );
				newNode->RotationKeys[key].frame = newNode->RotationKeys[key].frame * ratio;
			}
		}
		else
		{
			CGameAnimationTrack track1, track2;
			core::vector3df		pos1, pos2, scale1, scale2;
			core::quaternion	rot1, rot2;			

			track1.RotationKeys = nodeAnim1->RotationKeys;
			track1.PositionKeys = nodeAnim1->PositionKeys;
			track1.ScaleKeys = nodeAnim1->ScaleKeys;

			track2.RotationKeys = nodeAnim2->RotationKeys;
			track2.PositionKeys = nodeAnim2->PositionKeys;
			track2.ScaleKeys = nodeAnim2->ScaleKeys;

			CGameAnimationTrack::SRotationKey	rotKey;
			CGameAnimationTrack::SPositionKey	posKey;
			CGameAnimationTrack::SScaleKey		scaleKey;

			float frameSplit = 2.0f;
			float f = 0.0f;
			float ratio = time/time1;

			while ( 1 )
			{				
				track1.getFrameData(f, pos1, scale1, rot1, core::IdentityMatrix);
				track2.getFrameData(time2*f/time1, pos2, scale2, rot2, core::IdentityMatrix);
				
				posKey.frame = f*ratio;
				posKey.position.interpolate(pos1, pos2, w );
				newNode->PositionKeys.push_back( posKey );

				scaleKey.frame = posKey.frame;
				scaleKey.scale.interpolate(scale1, scale2, w);
				newNode->ScaleKeys.push_back( scaleKey );

				rotKey.frame = posKey.frame;
				rotKey.rotation.slerp( rot1, rot2, w);				
				newNode->RotationKeys.push_back( rotKey );				

				if ( f == time1 )
					break;

				f += frameSplit;

				if ( f > time1 )
					f = time1;
			}		
		}

		// add node anim
		newClip->addNodeAnim( newNode );
	}
	
	removeClip( lpNewName );
	addClip( newClip );
}

#pragma endregion


#pragma region DAE_COLLADA_ANIMATION_FACTORY_IMPLEMENT
//////////////////////////////////////////////////////////
// CColladaAnimationFactory implement
//////////////////////////////////////////////////////////

CColladaAnimationFactory::CColladaAnimationFactory()
{
}

CColladaAnimationFactory::~CColladaAnimationFactory()
{
	freeAllAnimationPackage();
}

CColladaAnimation* CColladaAnimationFactory::loadAnimation( const char *name, const char *lpFileName )
{
	CColladaAnimation *colladaAnim = m_animPackage[name];
	
	if ( colladaAnim == NULL )
		colladaAnim = new CColladaAnimation();

	colladaAnim->loadFile( lpFileName );

	m_animPackage[name] = colladaAnim;
	return colladaAnim;
}

// freeAllAnimationPackage
// release all package animation
void CColladaAnimationFactory::freeAllAnimationPackage()
{
	std::map<std::string, CColladaAnimation*>::iterator i = m_animPackage.begin(), end = m_animPackage.end();
	while ( i != end )
	{
		delete (*i).second;
		i++;
	}
	m_animPackage.clear();
}

#pragma endregion



//////////////////////////////////////////////////////////
// CColladaCache implement
//////////////////////////////////////////////////////////

std::map<std::string, CGameColladaContainerSceneNode*>	CColladaCache::s_nodeCache;


//////////////////////////////////////////////////////////
// CColladaMeshComponent implement
//////////////////////////////////////////////////////////

CColladaMeshComponent::CColladaMeshComponent( CGameObject *pObj )
	:IObjectComponent( pObj, (int)IObjectComponent::ColladaMesh )
{
	m_colladaNode = NULL;

	m_animeshFile = "";

	m_currentAnim = NULL;
	m_colladaAnimation = NULL;	

	m_isPauseAnim[0]	= false;
	m_isPauseAnim[1]	= false;
	m_pauseAtFrame[0]	= 0.0f;
	m_pauseAtFrame[1]	= 0.0f;
}

CColladaMeshComponent::~CColladaMeshComponent()
{
	if ( m_colladaNode )
		m_colladaNode->setOwner( NULL );
}


// init
// run when init object
void CColladaMeshComponent::initComponent()
{
}

// updateComponent
// update object by frame
void CColladaMeshComponent::updateComponent()
{
	// update gameplay lod geometry 
	updateLod();
}

 
// saveData
// save data to serializable
void CColladaMeshComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	pObj->addPath("meshFile", m_animeshFile.c_str(), true);
	pObj->addString("defaultNode", m_defaultNodeString.c_str(), true);
}

// loadData
// load data to serializable
void CColladaMeshComponent::loadData( CSerializable* pObj )
{		
	pObj->nextRecord();

	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();
	
	// clear array node list
	m_defaultNode.clear();

	// read mesh file
	char *lpFilename = pObj->readString();
	
	// read default node
	m_defaultNodeString = pObj->readString();

	// begin parse file
	loadFromFile( lpFilename );

	// init default node
	CGameColladaSceneNode* defaultNode = m_mapNode[m_defaultNodeString];
	if ( defaultNode )
		m_defaultNode.push_back( defaultNode );
}


#pragma region PARSE_DAE_DATA

// loadFromFile
// load anim object from dae file
void CColladaMeshComponent::loadFromFile( const char *lpFilename )
{
	if ( m_animeshFile == lpFilename )
		return;

	m_animeshFile = lpFilename;

	// search in cache	
	CGameColladaContainerSceneNode* cacheNode = CColladaCache::getNodeInCache( m_animeshFile );
	if ( cacheNode != NULL )
	{
		initFromNode( cacheNode );
		return;
	}

	char ext[10] = {0};
	uiString::getFileNameExt<const char, char>( lpFilename, ext );
	uiString::toLower( ext );
	
	if ( strcmp(ext, "dae") == 0 )
		loadDae( lpFilename );
	else if ( strcmp(ext,"scene") == 0 )
		loadScene( lpFilename );
}


void CColladaMeshComponent::loadDae( const char *lpFilename )
{	
	// todo load scene
	ISceneManager *smgr = getIView()->getSceneMgr();

	m_colladaNode = new CGameColladaContainerSceneNode
		(
			m_gameObject,
			m_gameObject->getParentSceneNode(),
			smgr,
			(s32)m_gameObject->getID()
		);
	
	m_gameObject->m_node = m_colladaNode;
	
	// parse binary scene
	CDaeUtils::getInstance()->setCurrentComponent(this);
	CDaeUtils::getInstance()->loadFile( lpFilename, m_gameObject );
	

	// cache node
	if ( m_colladaNode != NULL && CColladaCache::getNodeInCache(m_animeshFile) == NULL )
	{
		CColladaCache::cacheNode( m_animeshFile, m_colladaNode );
	}

#ifdef GSEDITOR
	if ( m_gameObject->m_node )
	{
		ISceneManager *smgr = getIView()->getSceneMgr();

		// add collision
		ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( m_gameObject->m_node );
		m_gameObject->m_node->setTriangleSelector(selector);
		selector->drop();
	}
#endif
	
}



void CColladaMeshComponent::loadScene( const char *lpFilename )
{
	// todo load file
	io::IReadFile *file = getIView()->getFileSystem()->createAndOpenFile( lpFilename );
	if ( file == NULL )
	{
		file = getIView()->getFileSystem()->createAndOpenFile( getIView()->getPath(lpFilename) );

		if ( file == NULL )
			return;
	}

	// todo load scene
	ISceneManager *smgr = getIView()->getSceneMgr();

	m_colladaNode = new CGameColladaContainerSceneNode
		(
			m_gameObject,
			m_gameObject->getParentSceneNode(),
			smgr,
			(s32)m_gameObject->getID()
		);
	
	m_gameObject->m_node = m_colladaNode;
	
	// parse binary scene
	CBinaryUtils::getInstance()->setCurrentComponent(this);
	CBinaryUtils::getInstance()->loadFile( file, m_gameObject );



	// close file
	file->drop();

	// cache node
	if ( m_colladaNode != NULL && CColladaCache::getNodeInCache(m_animeshFile) == NULL )
	{
		CColladaCache::cacheNode( m_animeshFile, m_colladaNode );
	}

#ifdef GSEDITOR
	if ( m_gameObject->m_node )
	{
		ISceneManager *smgr = getIView()->getSceneMgr();

		// add collision
		ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( m_gameObject->m_node );
		m_gameObject->m_node->setTriangleSelector(selector);
		selector->drop();
	}
#endif
}


// initFromNode
// init cache from node
struct SGroupNode
{
	ISceneNode *colladaParent;
	ISceneNode *initChild;

	SGroupNode( ISceneNode* p, ISceneNode* child )
	{
		colladaParent = p;
		initChild = child;
	}
};

void CColladaMeshComponent::initFromNode( CGameColladaContainerSceneNode* node )
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	m_colladaNode = new CGameColladaContainerSceneNode
		(
			m_gameObject,
			m_gameObject->getParentSceneNode(),
			smgr,
			(s32)m_gameObject->getID()
		);
	
	m_gameObject->m_node = m_colladaNode;

	std::queue< SGroupNode > queueNode;
	std::vector< CGameColladaMesh* > listSkinMesh;

	const core::list<ISceneNode*>& childs = node->getChildren();
	core::list<ISceneNode*>::ConstIterator it = childs.begin(), end = childs.end();
	while ( it != end )
	{
		queueNode.push( SGroupNode(m_colladaNode, (*it) ) );
		it++;
	}	
	
	while ( queueNode.size() )
	{
		SGroupNode groupNode = queueNode.front();
		queueNode.pop();

		// clone new node
		CGameColladaSceneNode *newNode = (CGameColladaSceneNode*)groupNode.initChild->clone( groupNode.colladaParent, smgr );
		newNode->setComponent( this );
						
		// store name this node
		std::string name = groupNode.initChild->getName();
		if ( name.length() > 0 )
			m_mapNode[ name ] = newNode;
        
		// store sid this node
		name = ((CGameColladaSceneNode*)groupNode.initChild)->getSIDName();
		if ( name.length() > 0 )
		{
			m_sidNode[name] = newNode;
			newNode->setSIDName( name );
		}

		// push skin mesh need update
		CGameColladaMesh* mesh = newNode->getMesh();
		if ( mesh && mesh->IsStaticMesh == false )
		{	
			mesh->Component = this;
			listSkinMesh.push_back( mesh );
		}
	
		if ( mesh )
			m_colladaNode->addBoundingMeshNode( newNode );

		const core::list<ISceneNode*>& childs = groupNode.initChild->getChildren();
		core::list<ISceneNode*>::ConstIterator it = childs.begin(), end = childs.end();
		while ( it != end )
		{
			queueNode.push( SGroupNode(newNode,(*it)) );
			it++;
		}

#ifdef GSANIMATION
		if ( newNode->getMesh() == NULL )
		{
			// add collision
			ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( newNode );
			newNode->setTriangleSelector(selector);
			selector->drop();
		}
#endif

		newNode->drop();	
	}

	// need map scenenode to joint
	std::vector<CGameColladaMesh*>::iterator iMesh = listSkinMesh.begin(), iMeshEnd = listSkinMesh.end();
	while ( iMesh != iMeshEnd )
	{
		(*iMesh)->updateJoint();
		iMesh++;
	}

#ifdef GSEDITOR
	if ( m_gameObject->m_node )
	{
		ISceneManager *smgr = getIView()->getSceneMgr();

		// add collision
		ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( m_gameObject->m_node );
		m_gameObject->m_node->setTriangleSelector(selector);
		selector->drop();
	}
#endif

}


// getChildsOfSceneNode
// find all childs of scene node
void CColladaMeshComponent::getChildsOfSceneNode( const char *name, std::vector<CGameColladaSceneNode*>& listChilds )
{
	CGameColladaSceneNode *rootNode = getSceneNode( name );
	if ( rootNode == NULL )
		return;
	
	std::stack<ISceneNode*>	stackNodes;
	stackNodes.push( rootNode );

	while ( stackNodes.size() )
	{
		// get current node
		ISceneNode* node = stackNodes.top();
		stackNodes.pop();

		// add childs to list
		if ( node != rootNode )
			listChilds.push_back( (CGameColladaSceneNode*)node );

		// continue get all childs node
		const core::list<ISceneNode*> childs = node->getChildren();
		core::list<ISceneNode*>::ConstIterator i = childs.begin(), end = childs.end();
		while ( i != end )
		{
			stackNodes.push( (*i) );
			i++;
		}

	}

}

// setCrossFadeAnimation
// crossfade current animation to new animation
void CColladaMeshComponent::setCrossFadeAnimation(const char *lpAnimName, int trackChannel, float nFrames, bool loop)
{
	if ( m_colladaNode == NULL )
		return;

	SColladaAnimClip *animClip = m_colladaAnimation->getAnim( lpAnimName );
	if ( animClip == NULL )
		return;
	
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		const std::string& nodeName = (*i).first;
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}

		// get current frame data
		core::vector3df		currentPos;
		core::vector3df		currentScale;
		core::quaternion	currentRotate;
		CGameAnimationTrack *track = j->getAnimation()->getTrack(trackChannel);

		track->getFrameData( track->getCurrentFrame(), currentPos, currentScale, currentRotate, j->LocalMatrix );

		CGameAnimationTrack::SRotationKey rot;
		rot.frame		= 0;
		rot.rotation	= currentRotate;

		CGameAnimationTrack::SPositionKey pos;
		pos.frame		= 0;
		pos.position	= currentPos;

		// clear old key frame		
		track->clearAllKeyFrame();
		track->setLoop( false );

		// todo add animation key
		SColladaNodeAnim* anim = animClip->getAnimOfSceneNode( nodeName.c_str() );

		if ( anim )
		{			
			int nRotKey = anim->RotationKeys.size();
			if ( nRotKey > 0 )
			{
				// set current animation
				track->RotationKeys.push_back( rot );

				rot.frame		= nFrames;
				rot.rotation	= anim->RotationKeys[0].rotation;
				track->RotationKeys.push_back( rot );

				// set cross animation
				track->CrossAnimRotationKeys.set_used( nRotKey );
				for ( int i = 0; i < nRotKey; i++ )
				{
					track->CrossAnimRotationKeys[i] = anim->RotationKeys[i];
				}
			}

			int nPosKey = anim->PositionKeys.size();
			if ( nPosKey > 0 )
			{
				// set current animation
				track->PositionKeys.push_back( pos );

				pos.frame		= nFrames;
				pos.position	= anim->PositionKeys[0].position;
				track->PositionKeys.push_back( pos );

				// set cross animation
				track->CrossAnimPositionKeys.set_used( nPosKey );
				for ( int i = 0; i < nPosKey; i++ )
				{
					track->CrossAnimPositionKeys[i] = anim->PositionKeys[i];
				}
			}

			// enable cross animation
			track->enableCrossAnimation( loop );
		}

		i++;
	}
			
	// current anim crossfade clip
	m_currentAnim = animClip;
	m_currentAnim->loop = false;	// hardcode to turn of loop on current anim!	
}


void CColladaMeshComponent::setCrossFadeAnimation(const char *lpAnimName, std::vector<CGameColladaSceneNode*>& listNodes, int trackChannel, float nFrames, bool loop)
{
	if ( m_colladaNode == NULL )
		return;

	SColladaAnimClip *animClip = m_colladaAnimation->getAnim( lpAnimName );
	if ( animClip == NULL )
		return;
	
	std::vector<CGameColladaSceneNode*>::iterator i = listNodes.begin(), end = listNodes.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i);
						
		if ( j == NULL )
		{
			i++;
			continue;
		}

		std::string nodeName = j->getName();

		// get current frame data
		core::vector3df		currentPos;
		core::vector3df		currentScale;
		core::quaternion	currentRotate;
		CGameAnimationTrack *track = j->getAnimation()->getTrack(trackChannel);

		track->getFrameData( track->getCurrentFrame(), currentPos, currentScale, currentRotate, j->LocalMatrix );

		CGameAnimationTrack::SRotationKey rot;
		rot.frame		= 0;
		rot.rotation	= currentRotate;

		CGameAnimationTrack::SPositionKey pos;
		pos.frame		= 0;
		pos.position	= currentPos;

		// clear old key frame		
		track->clearAllKeyFrame();
		track->setLoop( false );

		// todo add animation key
		SColladaNodeAnim* anim = animClip->getAnimOfSceneNode( nodeName.c_str() );

		if ( anim )
		{			
			int nRotKey = anim->RotationKeys.size();
			if ( nRotKey > 0 )
			{
				// set current animation
				track->RotationKeys.push_back( rot );

				rot.frame		= nFrames;
				rot.rotation	= anim->RotationKeys[0].rotation;
				track->RotationKeys.push_back( rot );

				// set cross animation
				track->CrossAnimRotationKeys.set_used( nRotKey );
				for ( int i = 0; i < nRotKey; i++ )
				{
					track->CrossAnimRotationKeys[i] = anim->RotationKeys[i];
				}
			}

			int nPosKey = anim->PositionKeys.size();
			if ( nPosKey > 0 )
			{
				// set current animation
				track->PositionKeys.push_back( pos );

				pos.frame		= nFrames;
				pos.position	= anim->PositionKeys[0].position;
				track->PositionKeys.push_back( pos );

				// set cross animation
				track->CrossAnimPositionKeys.set_used( nPosKey );
				for ( int i = 0; i < nPosKey; i++ )
				{
					track->CrossAnimPositionKeys[i] = anim->PositionKeys[i];
				}
			}
			
			// enable cross animation
			track->enableCrossAnimation( loop );
		}

		i++;
	}
			
	// current anim crossfade clip
	m_currentAnim = animClip;
	m_currentAnim->loop = false;	// hardcode to turn of loop on current anim!	
}


// setAnimation
// apply Animation to skin joint
void CColladaMeshComponent::setAnimation(const char *lpAnimName, int trackChannel, bool loop)
{
	if ( m_colladaNode == NULL )
		return;
	
	SColladaAnimClip *animClip = m_colladaAnimation->getAnim( lpAnimName );
	if ( animClip == NULL )
		return;
	
	animClip->loop = loop;

	// set current anim clip
	m_currentAnim = animClip;

	// loop all node
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		const std::string& nodeName = (*i).first;
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		// clear old key frame
		CGameAnimationTrack *track = j->getAnimation()->getTrack( trackChannel );
		track->clearAllKeyFrame();
						
		// todo add animation key
		SColladaNodeAnim* anim = animClip->getAnimOfSceneNode( nodeName.c_str() );

		if ( anim )
		{
			// add rotation key
			int nRotKey = anim->RotationKeys.size();
			if ( nRotKey > 0 )
			{
				track->RotationKeys.set_used( nRotKey );
				for ( int i = 0; i < nRotKey; i++ )
				{
					track->RotationKeys[i] = anim->RotationKeys[i];
				}
			}

			// add position key
			int nPosKey = anim->PositionKeys.size();
			if ( nPosKey > 0 )
			{
				track->PositionKeys.set_used( nPosKey );
				for ( int i = 0; i < nPosKey; i++ )
				{
					track->PositionKeys[i] = anim->PositionKeys[i];
				}
			}

			// add scale key
			int nScaleKey = anim->ScaleKeys.size();
			if ( nScaleKey > 0 )
			{
				track->ScaleKeys.set_used( nScaleKey );
				for ( int i = 0; i < nScaleKey; i++ )
				{
					track->ScaleKeys[i] = anim->ScaleKeys[i];
				}
			}

			track->setLoop( loop );
		}

		// next node
		i++;
	}
}

// setAnimation
// apply Animation to array of skin joint
void CColladaMeshComponent::setAnimation(const char *lpAnimName, std::vector<CGameColladaSceneNode*>& listNodes, int trackChannel, bool loop )
{
	if ( m_colladaNode == NULL )
		return;
	
	SColladaAnimClip *animClip = m_colladaAnimation->getAnim( lpAnimName );
	if ( animClip == NULL )
		return;
	
	animClip->loop = loop;
	
	// set current anim clip
	m_currentAnim = animClip;

	std::vector<CGameColladaSceneNode*>::iterator i = listNodes.begin(), end = listNodes.end();
	while ( i != end )
	{		
		CGameColladaSceneNode* j = (*i);
				
		if ( j == NULL )
		{
			i++;
			continue;
		}

		// clear old key frame
		CGameAnimationTrack *track = j->getAnimation()->getTrack( trackChannel );
		track->clearAllKeyFrame();							

		// todo add animation key
		SColladaNodeAnim* anim = animClip->getAnimOfSceneNode( j->getName() );

		if ( anim )
		{
			int nRotKey = anim->RotationKeys.size();
			for ( int i = 0; i < nRotKey; i++ )
			{
				track->RotationKeys.push_back( anim->RotationKeys[i] );	
			}

			int nPosKey = anim->PositionKeys.size();
			for ( int i = 0; i < nPosKey; i++ )
			{
				track->PositionKeys.push_back( anim->PositionKeys[i] );
			}

			int nScaleKey = anim->ScaleKeys.size();
			for ( int i = 0; i < nScaleKey; i++ )
			{
				track->ScaleKeys.push_back( anim->ScaleKeys[i] );
			}			

			track->setLoop( loop );
		}

		// next node
		i++;
	}
}

// synchronizedByTimeScale
// sync 2 animation
void CColladaMeshComponent::synchronizedByTimeScale()
{
	// loop all node
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		j->getAnimation()->synchronizedByTimeScale();

		i++;
	}
}

void CColladaMeshComponent::synchronizedByTimeScale(std::vector<CGameColladaSceneNode*>& listNodes)
{
	std::vector<CGameColladaSceneNode*>::iterator i = listNodes.begin(), end = listNodes.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i);
		j->getAnimation()->synchronizedByTimeScale();
		i++;
	}
}

// pauseAtFrame
// pause anim at frame id
void CColladaMeshComponent::pauseAtFrame( float frame, int trackChannel)
{
	m_isPauseAnim[trackChannel]		= true;
	m_pauseAtFrame[trackChannel]	= frame;

	// loop all node
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation()->getTrack( trackChannel );

		track->setCurrentFrame( frame );
		track->setPause( true );

		i++;
	}
}
	
// getCurrentFrame
// get current frame of anim
float CColladaMeshComponent::getCurrentFrame(int trackChannel)
{
	// loop all node
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation()->getTrack( trackChannel );

		if ( track->getTotalFrame() > 0 )
			return track->getCurrentFrame();

		i++;
	}

	return 0.0f;
}

float CColladaMeshComponent::getCurrentFrame(std::vector<CGameColladaSceneNode*>& listNodes, int trackChannel)
{
	std::vector<CGameColladaSceneNode*>::iterator i = listNodes.begin(), end = listNodes.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i);
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation()->getTrack( trackChannel );

		if ( track->getTotalFrame() > 0 )
			return track->getCurrentFrame();


		i++;
	}
	return 0.0f;
}

// get current anim speed
float CColladaMeshComponent::getAnimSpeed(int trackChannel)
{
	// loop all node
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation()->getTrack( trackChannel );

		if ( track->getTotalFrame() > 0 )
			return track->getSpeedRatio();

		i++;
	}
	return 1.0f;
}

float CColladaMeshComponent::getAnimSpeed(std::vector<CGameColladaSceneNode*>& listNodes, int trackChannel)
{
	std::vector<CGameColladaSceneNode*>::iterator i = listNodes.begin(), end = listNodes.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i);
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation()->getTrack( trackChannel );

		if ( track->getTotalFrame() > 0 )
			return track->getSpeedRatio();


		i++;
	}
	return 1.0f;
}


// setCurrentFrame	
void CColladaMeshComponent::setCurrentFrame(float f, int trackChannel)
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation()->getTrack( trackChannel );

		track->setCurrentFrame(f);
		i++;
	}
}

// resumeAnim
// resume animation
void CColladaMeshComponent::resumeAnim(int trackChannel)
{
	m_isPauseAnim[trackChannel]		= false;

	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
				
		if ( j == NULL )
		{
			i++;
			continue;
		}
		
		CGameAnimationTrack *track = j->getAnimation()->getTrack( trackChannel );

		track->setPause( false );
		i++;
	}
}

// setAnimWeight
void CColladaMeshComponent::setAnimWeight(float w, int trackChannel)
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
		if ( j != NULL )
			j->getAnimation()->getTrack(trackChannel)->setAnimWeight(w);
		i++;
	}
}

// setAnimWeight
// set anim weight on list bone node
void CColladaMeshComponent::setAnimWeight(std::vector<CGameColladaSceneNode*>& listNodes, float w, int trackChannel)
{
    std::vector<CGameColladaSceneNode*>::iterator i = listNodes.begin(), end = listNodes.end();
    while ( i != end )
    {
        (*i)->getAnimation()->getTrack(trackChannel)->setAnimWeight(w);
        i++;
    }
}

// setAnimSpeed
void CColladaMeshComponent::setAnimSpeed(float s, int trackChannel)
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
		if ( j != NULL )
			j->getAnimation()->getTrack(trackChannel)->setSpeedRatio(s);
		i++;
	}
}

// setAnimSpeed
// set anim speed on list bone node
void CColladaMeshComponent::setAnimSpeed(std::vector<CGameColladaSceneNode*>& listNodes, float s, int trackChannel)
{
    std::vector<CGameColladaSceneNode*>::iterator i = listNodes.begin(), end = listNodes.end();
    while ( i != end )
    {
        (*i)->getAnimation()->getTrack(trackChannel)->setSpeedRatio(s);
        i++;
    }
}

void CColladaMeshComponent::enableAnimTrackChannel( int trackChannel, bool b)
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
		if ( j != NULL )
			j->getAnimation()->getTrack(trackChannel)->setEnable( b );		
		i++;
	}
}


void CColladaMeshComponent::enableAnimTrackChannel( std::vector<CGameColladaSceneNode*>& listNodes, int trackChannel, bool b)
{
    std::vector<CGameColladaSceneNode*>::iterator i = listNodes.begin(), end = listNodes.end();
    while ( i != end )
    {
        (*i)->getAnimation()->getTrack(trackChannel)->setEnable(b);
        i++;
    }    
}

void CColladaMeshComponent::onlyEnableAnimTrackChannel( int trackChannel )
{
	std::map<std::string, CGameColladaSceneNode*>::iterator i = m_mapNode.begin(), end = m_mapNode.end();
	while ( i != end )
	{
		CGameColladaSceneNode* j = (*i).second;
		if ( j != NULL )
			j->getAnimation()->onlyEnableTrack( trackChannel );
		i++;
	}
}

void CColladaMeshComponent::onlyEnableAnimTrackChannel( std::vector<CGameColladaSceneNode*>& listNodes, int trackChannel )
{
    std::vector<CGameColladaSceneNode*>::iterator i = listNodes.begin(), end = listNodes.end();
    while ( i != end )
    {
        (*i)->getAnimation()->onlyEnableTrack(trackChannel);
        i++;
    }    
}


// saveSceneToBinary
// save collada mesh info to binary file
void CColladaMeshComponent::saveSceneToBinary( const char *lpFileName )
{
	io::IFileSystem *fileSystem = getIView()->getSceneMgr()->getFileSystem();
	io::IWriteFile *file = fileSystem->createAndWriteFile( io::path(lpFileName) );
	
	if ( file == NULL )
		file = fileSystem->createAndWriteFile( io::path( getIView()->getPath(lpFileName) ));

	if ( file )
	{
		CBinaryUtils::getInstance()->setCurrentComponent(this);
		CBinaryUtils::getInstance()->saveCollada( file, m_gameObject );	
		file->drop();
	}
}

// saveAnimToBinary
// save animation track to binary file
void CColladaMeshComponent::saveAnimToBinary( const char *lpFileName )
{
	io::IFileSystem *fileSystem = getIView()->getSceneMgr()->getFileSystem();
	io::IWriteFile *file = fileSystem->createAndWriteFile( io::path(lpFileName) );

	if ( file == NULL )
		file = fileSystem->createAndWriteFile( io::path( getIView()->getPath(lpFileName) ));

	if ( file )
	{
		CBinaryUtils::getInstance()->setCurrentComponent(this);
		CBinaryUtils::getInstance()->saveAnimation( file, m_colladaAnimation );
		file->drop();
	}
}

//////////////////////////////////////////////////////////
// CColladaMeshComponent gameplay function implement
//////////////////////////////////////////////////////////


// sortLodData
// sort lod data by distance
void CColladaMeshComponent::sortLodData()
{
	int len = (int)m_colladaLodNode.size();
	
	SColladaLodNode obj;
		
	for (int i = 1; i <= len - 1; i++)
	{
		obj = m_colladaLodNode[i];
		
		int j = i - 1;
		int done = 0;

		do
		{
			if ( m_colladaLodNode[j].distance > obj.distance )
			{
				m_colladaLodNode[j + 1] = m_colladaLodNode[j];

				j = j - 1;
				if ( j < 0 )
					done = 1;
			}
			else
				done = 1;
		}
		while (done == 0);
		
		m_colladaLodNode[j + 1] = obj;		
	}
}

// clearLodData
// clear all lod data
void CColladaMeshComponent::clearLodData()
{
	CColladaMeshComponent* collada = (CColladaMeshComponent*) m_gameObject->getComponent( IObjectComponent::ColladaMesh );
	
	if ( collada != NULL )
	{
		for ( int i = 0, nLod = (int)m_colladaLodNode.size(); i < nLod; i++ )
		{
			SColladaLodNode& lod = m_colladaLodNode[i];
			CGameColladaSceneNode* node = collada->getSceneNode( lod.node.c_str() );
			node->setVisible( true );
		}
	}

	m_colladaLodNode.clear();
}

// addLodData
// set lod data
void CColladaMeshComponent::addLodData( float distanceCam, const char* node )
{
	SColladaLodNode lod;
	lod.distance = distanceCam;
	lod.node = node;

	m_colladaLodNode.push_back( lod );
	sortLodData();
}

// updateLod
// update lod mesh
void CColladaMeshComponent::updateLod()
{
	if ( m_colladaLodNode.size() == 0 )
		return;

#ifdef GSGAMEPLAY
	CGameCamera* cam = CGameLevel::getCurrentLevel()->getCamera();
#else
	CGameCamera* cam = getIView()->getActiveCamera();
#endif

	if ( cam == NULL )
		return;

	float distance = m_gameObject->getPosition().getDistanceFrom( cam->getPosition() );
	bool hasLod = false;

	for ( int nLod = (int)m_colladaLodNode.size(), i = 0; i < nLod; i++ )
	{
		SColladaLodNode& lod = m_colladaLodNode[i];
		CGameColladaSceneNode* node = getSceneNode( lod.node.c_str() );
		
		if ( node )
		{
			if ( distance < lod.distance )				
			{
				if ( hasLod == false )
				{
					// we find the current lod
					node->setVisible( true );
					hasLod = true;
				}
				else
				{
					node->setVisible( false );
				}
			}
			else
				node->setVisible( false );
		}

	}
}