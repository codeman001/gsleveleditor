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

#include "CGameCamera.h"
#ifdef GSGAMEPLAY
#include "gameLevel/CGameLevel.h"
#endif

#pragma region DAE_PARSE_C_FUNCTION_SUPPORT
//////////////////////////////////////////////////////////
// c function declare
//////////////////////////////////////////////////////////


void			uriToId(std::wstring& str);
std::wstring			readId(io::IXMLReader *xmlRead);
void			findNextNoneWhiteSpace(const c8** start);
inline f32		readFloat(const c8** p);

void			readIntsInsideElement(io::IXMLReader* reader, s32* ints, u32 count);
void			readIntsInsideElement(io::IXMLReader* reader, std::vector<s32>& arrayInt);
void			readFloatsInsideElement(io::IXMLReader* reader, f32* floats, u32 count);
void			readStringInsideElement(io::IXMLReader* reader, std::vector<std::wstring>& arrayString);

SColorf			readColorNode(io::IXMLReader* reader);
f32				readFloatNode(io::IXMLReader* reader);
core::matrix4	readTranslateNode(io::IXMLReader* reader, bool flip);
core::matrix4	readRotateNode(io::IXMLReader* reader, bool flip);
core::matrix4	readScaleNode(io::IXMLReader* reader, bool flip);
core::matrix4	readMatrixNode(io::IXMLReader* reader, bool flip);

ITexture*		getTextureFromImage( std::string& basePath, std::wstring& uri, ArrayEffectParams& listEffectParam, ArrayImages& listImages);
ITexture*		getTextureFromImage( std::string& basePath, std::wstring& id, const ArrayImages& listImages );
std::wstring	getImageWithId( const std::wstring& id, const ArrayImages& listImages );

int				getBufferWithUri( std::wstring& uri, SMeshParam* mesh );
int				getVerticesWithUri( std::wstring& uri, SMeshParam* mesh );
int				getEffectWithUri( std::wstring& uri, ArrayEffects& listEffectParam, ArrayEffects& listMaterial );
int				getMeshWithUri( std::wstring& uri, ArrayMeshParams& listMeshParam );
int				getMeshWithControllerName( std::wstring& controllerName, ArrayMeshParams& listMeshParam );

//////////////////////////////////////////////////////////
// c function implement
//////////////////////////////////////////////////////////

//! changes the XML URI into an internal id
void uriToId(std::wstring& str)
{	
	if (!str.size())
		return;

	if (str[0] == L'#')
		str.erase( str.begin() );
}


std::wstring readId(io::IXMLReader *xmlRead)
{
	std::wstring str = xmlRead->getAttributeValue(L"id");
	if (str.size()==0)
		str = xmlRead->getAttributeValue(L"name");
	
	return str;
}

void findNextNoneWhiteSpace(const c8** start)
{
	const c8* p = *start;

	while(*p && (*p==' ' || *p=='\n' || *p=='\r' || *p=='\t'))
		++p;

	*start = p;
}

inline f32 readFloat(const c8** p)
{
	f32 ftmp;
	*p = core::fast_atof_move(*p, ftmp);
	return ftmp;
}

void readFloatsInsideElement(io::IXMLReader* reader, f32* floats, u32 count)
{
	if (reader->isEmptyElement())
		return;

	while(reader->read())
	{
		if (reader->getNodeType() == io::EXN_TEXT)
		{
			// parse float data
			core::stringc data = reader->getNodeData();
			data.trim();
			const c8* p = &data[0];

			for (u32 i=0; i<count; ++i)
			{
				findNextNoneWhiteSpace(&p);
				if (*p)
					floats[i] = readFloat(&p);
				else
					floats[i] = 0.0f;
			}
		}
		else if (reader->getNodeType() == io::EXN_ELEMENT_END)
			break; // end parsing text
	}
}


//! reads ints from inside of xml element until end of xml element
void readIntsInsideElement(io::IXMLReader* reader, s32* ints, u32 count)
{
	if (reader->isEmptyElement())
		return;

	while(reader->read())
	{
		// TODO: check for comments inside the element
		// and ignore them.

		if (reader->getNodeType() == io::EXN_TEXT)
		{
			// parse float data
			core::stringc data = reader->getNodeData();
			data.trim();
			const c8* p = &data[0];

			for (u32 i=0; i<count; ++i)
			{
				findNextNoneWhiteSpace(&p);
				if (*p)
					ints[i] = (s32)readFloat(&p);
				else
					ints[i] = 0;
			}
		}
		else if (reader->getNodeType() == io::EXN_ELEMENT_END)
			break; // end parsing text
	}
}

//! reads ints from inside of xml element until end of xml element
void readIntsInsideElement(io::IXMLReader* reader, std::vector<s32>& arrayInt)
{
	if (reader->isEmptyElement())
		return;

	while(reader->read())
	{
		// TODO: check for comments inside the element
		// and ignore them.

		if (reader->getNodeType() == io::EXN_TEXT)
		{
			core::stringw data = reader->getNodeData();
			data.trim();
			int len = data.size();

			wchar_t* p = &data[0];
			wchar_t* begin = &data[0];

			int value = 0;
					

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
					swscanf(begin,L"%d", &value);
					arrayInt.push_back( value );					
				}

				p++;
				len--;
				begin = p;
			}

		}
		else
		if (reader->getNodeType() == io::EXN_ELEMENT_END)
			break; // end parsing text
	}
}				

void readStringInsideElement(io::IXMLReader* reader, std::vector<std::wstring>& arrayString)
{
	if (reader->isEmptyElement())
		return;

	while(reader->read())
	{
		// TODO: check for comments inside the element
		// and ignore them.

		if (reader->getNodeType() == io::EXN_TEXT)
		{
			core::stringw data = reader->getNodeData();
			data.trim();
			
			wchar_t* p = &data[0];
			wchar_t* begin = &data[0];

			for (u32 i=0; i< arrayString.size(); ++i)
			{
				while(*p && !(*p==L' ' || *p==L'\n' || *p==L'\r' || *p==L'\t'))
					++p;

				*p = NULL;

				if (*begin)
					arrayString[i] = std::wstring(begin);
				else
					arrayString[i] = std::wstring(L"");

				p++;
				begin = p;
			}
		}
		else
		if (reader->getNodeType() == io::EXN_ELEMENT_END)
			break; // end parsing text
	}
}

video::SColorf readColorNode(io::IXMLReader* reader)
{
	const core::stringc colorNodeName = "color";

	if (reader->getNodeType() == io::EXN_ELEMENT && colorNodeName == reader->getNodeName())
	{
		f32 color[4];
		readFloatsInsideElement(reader,color,4);
		return video::SColorf(color[0], color[1], color[2], color[3]);
	}

	return video::SColorf();
}

f32 readFloatNode(io::IXMLReader* reader)
{
	const core::stringc floatNodeName =        "float";
	
	f32 result = 0.0f;
	if (reader->getNodeType() == io::EXN_ELEMENT && floatNodeName == reader->getNodeName())
	{
		readFloatsInsideElement(reader,&result,1);
	}

	return result;
}

//! reads a <scale> element and its content and creates a matrix from it
core::matrix4 readScaleNode(io::IXMLReader* reader, bool flip)
{
	core::matrix4 mat;
	if (reader->isEmptyElement())
		return mat;

	f32 floats[3];
	readFloatsInsideElement(reader, floats, 3);

	if (flip)
		mat.setScale(core::vector3df(floats[0], floats[2], floats[1]));
	else
		mat.setScale(core::vector3df(floats[0], floats[1], floats[2]));

	return mat;
}

//! reads a <translate> element and its content and creates a matrix from it
core::matrix4 readTranslateNode(io::IXMLReader* reader, bool flip)
{
	core::matrix4 mat;
	if (reader->isEmptyElement())
		return mat;

	f32 floats[3];
	readFloatsInsideElement(reader, floats, 3);

	if (flip)
		mat.setTranslation(core::vector3df(floats[0], floats[2], floats[1]));
	else
		mat.setTranslation(core::vector3df(floats[0], floats[1], floats[2]));

	return mat;
}

//! reads a <rotate> element and its content and creates a matrix from it
core::matrix4 readRotateNode(io::IXMLReader* reader, bool flip)
{
	core::matrix4 mat;
	if (reader->isEmptyElement())
		return mat;

	f32 floats[4];
	readFloatsInsideElement(reader, floats, 4);

	if (!core::iszero(floats[3]))
	{
		core::quaternion q;
		if (flip)
			q.fromAngleAxis(floats[3]*core::DEGTORAD, core::vector3df(floats[0], floats[2], floats[1]));
		else
			q.fromAngleAxis(floats[3]*-core::DEGTORAD, core::vector3df(floats[0], floats[1], floats[2]));
		
		q.normalize();
		return q.getMatrix();
	}
	else
		return core::IdentityMatrix;
}

core::matrix4 readMatrixNode(io::IXMLReader* reader, bool flip)
{
	core::matrix4 mat;
	if (reader->isEmptyElement())
		return mat;

	readFloatsInsideElement(reader, mat.pointer(), 16);
	// put translation into the correct place
	if (flip)
	{
		core::matrix4 mat2(mat, core::matrix4::EM4CONST_TRANSPOSED);
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
		return mat2;
	}
	else
		return core::matrix4(mat, core::matrix4::EM4CONST_TRANSPOSED);
}


std::wstring getImageWithId( const std::wstring& id, const ArrayImages& listImages )
{
	int n = listImages.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listImages[i].id == id )
			return listImages[i].fileName;
	}
	return id;
}

video::ITexture* getTextureFromImage( std::string& basePath, std::wstring& id, const ArrayImages& listImages )
{
	std::wstring textureName = getImageWithId( id, listImages);
	
	std::string path = basePath;

	int i = basePath.length() - 1;
	while ( i > 0 )
	{
		if ( basePath[i] == '\\' || basePath[i] == '/' )
		{
			path = basePath.substr(0, i + 1);
			break;
		}
		i--;
	}

	// try open 1
	wchar_t textureNameW[1024] = {0};
	char textureNameA[1024] = {0};
	uiString::copy<char, const char>( textureNameA, path.c_str() );
	uiString::cat<char, const wchar_t>( textureNameA, textureName.c_str() );
		
	ITexture *tex =	getIView()->getDriver()->getTexture( textureNameA );

	// try open 2
	if ( tex == NULL )
	{
		uiString::getFileName<const wchar_t, wchar_t>( textureName.c_str(), textureNameW );
		textureName = textureNameW;

		uiString::copy<char, const char>( textureNameA, path.c_str() );
		uiString::cat<char, const wchar_t>( textureNameA, textureName.c_str() );

		tex = getIView()->getDriver()->getTexture( textureNameA );
	}
	
	// try open 3
	if ( tex == NULL )
	{
		tex = getIView()->getDriver()->getTexture( getIView()->getPath( textureName.c_str() ) );
	}
	return tex;
}

video::ITexture* getTextureFromImage( std::string& basePath, std::wstring& uri, ArrayEffectParams& listEffectParam, ArrayImages& listImages)
{	
	int n = listEffectParam.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listEffectParam[i].Name == uri )
		{
			if ( listEffectParam[i].InitFromTexture.size() > 0 )
			{
				std::wstring textureName = getImageWithId(listEffectParam[i].InitFromTexture, listImages);
				
				std::string path = basePath;

				int i = basePath.length() - 1;
				while ( i > 0 )
				{
					if ( basePath[i] == '\\' || basePath[i] == '/' )
					{
						path = basePath.substr(0, i + 1);
						break;
					}
					i--;
				}

				// try open 1
				wchar_t textureNameW[1024] = {0};
				char textureNameA[1024] = {0};
				uiString::copy<char, const char>( textureNameA, path.c_str() );
				uiString::cat<char, const wchar_t>( textureNameA, textureName.c_str() );
					
				ITexture *tex =	getIView()->getDriver()->getTexture( textureNameA );
				
				// try open 2
				if ( tex == NULL )
				{
					uiString::getFileName<const wchar_t, wchar_t>( textureName.c_str(), textureNameW );
					textureName = textureNameW;

					uiString::copy<char, const char>( textureNameA, path.c_str() );
					uiString::cat<char, const wchar_t>( textureNameA, textureName.c_str() );

					tex = getIView()->getDriver()->getTexture( textureNameA );
				}
				
				// try open 3
				if ( tex == NULL )
				{
					tex = getIView()->getDriver()->getTexture( getIView()->getPath( textureName.c_str() ) );
				}

				return tex;
			}
			else if ( listEffectParam[i].Source.size() > 0 )
				return getTextureFromImage( basePath,listEffectParam[i].Source, listEffectParam, listImages );

			return NULL;
		}
	}
	return NULL;
}

int getBufferWithUri( std::wstring& uri, SMeshParam* mesh )
{
	int n = mesh->Buffers.size();
	for ( int i =0; i < n; i++ )
	{
		if ( mesh->Buffers[i].Name == uri )
		{
			return i;
		}
	}
	return -1;
}

int getVerticesWithUri( std::wstring& uri, SMeshParam* mesh )
{
	int n = mesh->Vertices.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( mesh->Vertices[i].Name == uri )
		{
			return i;
		}
	}

	return -1;
}

int getEffectWithUri( std::wstring& uri, ArrayEffects& listEffectParam, ArrayEffects& listMaterial )
{
	// search in effect list
	std::wstring fxName = uri + L"-fx";	
	int n = listEffectParam.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listEffectParam[i].Id == fxName )
		{
			return i;
		}
	}	

	// try search in material list
	n = listMaterial.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listMaterial[i].Id == uri )
		{
			// add to list effect
			listEffectParam.push_back( listMaterial[i] );

			// rename to effect style
			SEffect &effect = listEffectParam.back();
			effect.Id = uri + L"-fx";

			return listEffectParam.size() - 1;
		}
	}	

	return -1;
}

int	getMeshWithUri( std::wstring& uri, ArrayMeshParams& listMeshParam )
{
	int n = listMeshParam.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listMeshParam[i].Name == uri )
		{
			return i;
		}
	}

	return -1;
}

int getMeshWithControllerName( std::wstring& controllerName, ArrayMeshParams& listMeshParam )
{
	int n = listMeshParam.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listMeshParam[i].ControllerName == controllerName )
		{
			return i;
		}
	}

	return -1;
}

#pragma endregion


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
		else
		{
			pos = idNodeName.find( L"-scale" );
			if ( pos > 0 )
			{
				isScale = true;
				jointName = idNodeName.substr(0, pos);
			}
			else
			{
				pos = idNodeName.find( L"-anim" );
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
	int		numArrayTime = 0;

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

void CColladaAnimation::loadDae( char *lpFileName )
{
	IrrlichtDevice	*device = getIView()->getDevice();
	IVideoDriver	*driver = getIView()->getDriver();
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
	for ( int i = 0, n = m_colladaAnim.size(); i < n; i++ )
	{
		SColladaAnimClip *clip = m_colladaAnim[i];
		m_animWithName[ clip->animName ] = clip;
	}
}

void CColladaAnimation::loadDotAnim( char *lpFileName )
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

void CColladaAnimation::loadFile( char *lpFileName )
{
	char ext[10] = {0};
	uiString::getFileNameExt<char, char>( lpFileName, ext );
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

	for ( int i = 0, nNodeAnims = clip->animInfo.size(); i < nNodeAnims; i++ )
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

	for ( int i = 0, n = clip1->animInfo.size(); i < n; i++ )
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

CColladaAnimation* CColladaAnimationFactory::loadAnimation( char *name, char *lpFileName )
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
	IrrlichtDevice	*device = getIView()->getDevice();
	IVideoDriver	*driver = getIView()->getDriver();
	io::IFileSystem *fs = device->getFileSystem();

	io::IXMLReader *xmlRead = fs->createXMLReader( lpFilename );

	if ( xmlRead == NULL )
	{
		xmlRead = fs->createXMLReader( getIView()->getPath(lpFilename) );
		if ( xmlRead == NULL )
			return;
	}

	const std::wstring controllerSectionName(L"controller");
	const std::wstring imageSectionName(L"image");
	const std::wstring materialSectionName(L"material");
	const std::wstring effectSectionName(L"effect");
	const std::wstring geometrySectionName(L"geometry");	
	const std::wstring sceneSectionName(L"visual_scene");	

	bool readLUpAxis = false;
	m_needFlip = false;

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				std::wstring nodeName = xmlRead->getNodeName();

				if ( nodeName == imageSectionName )
				{
					parseImageNode( xmlRead );
				}
				else if ( nodeName == materialSectionName )
				{
					parseMaterialNode( xmlRead );
				}
				else if ( nodeName == effectSectionName )
				{
					parseEffectNode( xmlRead );
				}
				else if ( nodeName == geometrySectionName )
				{
					parseGeometryNode( xmlRead );
				}
				else if ( nodeName == controllerSectionName )
				{				
					parseControllerNode( xmlRead );					
				}
				else if ( nodeName == sceneSectionName )
				{
					parseSceneNode( xmlRead );
				}
				else if ( nodeName == L"up_axis" )
				{
					readLUpAxis = true;
				}
				
			}
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

	// create scene node
	constructScene();

	// clean data
	cleanData();

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



// parseGeometryNode
// parse mesh data
void CColladaMeshComponent::parseGeometryNode( io::IXMLReader *xmlRead )
{
	const std::wstring geometrySectionName(L"geometry");
	const std::wstring sourceNode(L"source");
	const std::wstring verticesNode(L"vertices");
	const std::wstring trianglesNode(L"triangles");
	const std::wstring floatArrayNode(L"float_array");
	const std::wstring inputNode(L"input");
	const std::wstring accessorNode(L"accessor");

	SMeshParam mesh;
	
	mesh.Type = k_mesh;
	mesh.Name = readId( xmlRead );

	while(xmlRead->read())
	{
		if (xmlRead->getNodeType() == io::EXN_ELEMENT)
		{	
			std::wstring nodeName = xmlRead->getNodeName();

			//<source>
			if ( nodeName == sourceNode )
			{
				SBufferParam buffer;

				buffer.Name = readId( xmlRead );
				buffer.Type = k_positionBuffer;
				buffer.ArrayCount = 0;
				buffer.Strike = 0;
				buffer.FloatArray = NULL;

				while(xmlRead->read())
				{		
					if (xmlRead->getNodeType() == io::EXN_ELEMENT )
					{
						if ( xmlRead->getNodeName() == floatArrayNode )
						{
							buffer.ArrayCount = xmlRead->getAttributeValueAsInt(L"count");							
							buffer.FloatArray = new float[ buffer.ArrayCount ];

							readFloatsInsideElement( xmlRead, buffer.FloatArray, buffer.ArrayCount );							
						}
						else if ( xmlRead->getNodeName() == accessorNode )
						{
							buffer.Strike = xmlRead->getAttributeValueAsInt(L"stride");
						}
					}					
					else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
					{
						if ( xmlRead->getNodeName() == sourceNode )
							break;
					}
				}
				
				mesh.Buffers.push_back( buffer );
				
			}
			//<vertices>
			else if ( nodeName == verticesNode )
			{
				SVerticesParam	verticesParam;
				verticesParam.Name = readId( xmlRead );

				while(xmlRead->read())
				{		
					if (xmlRead->getNodeType() == io::EXN_ELEMENT )
					{
						if ( xmlRead->getNodeName() == inputNode )
						{
							std::wstring semantic = xmlRead->getAttributeValue(L"semantic");
							std::wstring source = xmlRead->getAttributeValue(L"source");
							source.erase( source.begin() );

							int bufferID = getBufferWithUri( source, &mesh );
														
							if ( bufferID != -1 )
							{
								SBufferParam *buffer = &mesh.Buffers[bufferID];

								if ( semantic == L"POSITION" )
								{
									buffer->Type = k_positionBuffer;
									verticesParam.PositionIndex = bufferID;
								}
								else if ( semantic == L"NORMAL" )
								{
									buffer->Type = k_normalBuffer;
									verticesParam.NormalIndex = bufferID;
								}
								else if ( semantic == L"TEXCOORD" )
								{
									buffer->Type = k_texCoordBuffer;
									
									if ( verticesParam.TexCoord1Index == -1 )
										verticesParam.TexCoord1Index = bufferID;
									else if ( verticesParam.TexCoord1Index != bufferID )
										verticesParam.TexCoord2Index = bufferID;
								}								
							}

						}
					}
					else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
					{
						if ( xmlRead->getNodeName() == verticesNode )
							break;
					}
				}

				// add vertex param
				mesh.Vertices.push_back( verticesParam );
			}
			//<triangles>
			else if ( nodeName == trianglesNode )
			{
				STrianglesParam triangle;

				// default is 1
				triangle.NumElementPerVertex = 1;

				triangle.NumPolygon = xmlRead->getAttributeValueAsInt(L"count");

				std::wstring materialName = xmlRead->getAttributeValue(L"material");
				triangle.EffectIndex = getEffectWithUri( materialName, m_listEffects, m_listMaterial );

				while(xmlRead->read())
				{							
					if (xmlRead->getNodeType() == io::EXN_ELEMENT )
					{
						if ( xmlRead->getNodeName() == inputNode )
						{
							std::wstring source = xmlRead->getAttributeValue(L"source");
							std::wstring semantic = xmlRead->getAttributeValue(L"semantic");
							
							int offset = 0;							
							offset = xmlRead->getAttributeValueAsInt(L"offset");

							source.erase( source.begin() );
														
							if ( semantic == L"NORMAL" )
							{
								int bufferID =	getBufferWithUri( source, &mesh );			
								mesh.Buffers[bufferID].Type = k_normalBuffer;
								
								if ( triangle.VerticesIndex != -1 )
								{
									mesh.Vertices[ triangle.VerticesIndex ].NormalIndex = bufferID;
									triangle.OffsetNormal = offset;
								}

								triangle.NumElementPerVertex++;
							}
							else if ( semantic == L"TEXCOORD" )
							{
								int bufferID =	getBufferWithUri( source, &mesh );
								mesh.Buffers[bufferID].Type = k_texCoordBuffer;
								if ( triangle.VerticesIndex != -1 )
								{		
									SVerticesParam &verticesParam = mesh.Vertices[ triangle.VerticesIndex ];
									if ( verticesParam.TexCoord1Index == -1 )
									{
										verticesParam.TexCoord1Index = bufferID;
										triangle.OffsetTexcoord1 = offset;
									}
									else if ( verticesParam.TexCoord1Index != bufferID )
									{
										verticesParam.TexCoord2Index = bufferID;
										triangle.OffsetTexcoord2 = offset;
									}

									// hard code to fix many triangle on a mesh
									if ( triangle.OffsetTexcoord1 == 0 )
										triangle.OffsetTexcoord1 = offset;
									else if ( triangle.OffsetTexcoord2 == 0 )
										triangle.OffsetTexcoord2 = offset;

								}

								triangle.NumElementPerVertex++;
							}
							else if ( semantic == L"VERTEX" )
							{
								triangle.VerticesIndex = getVerticesWithUri( source, &mesh );
								triangle.OffsetVertex = offset;
							}
							else
							{
								triangle.NumElementPerVertex++;
							}
						}
						else if ( xmlRead->getNodeName() == std::wstring(L"p") && triangle.VerticesIndex != -1 )
						{
							triangle.IndexBuffer = new s32[triangle.NumPolygon * triangle.NumElementPerVertex * 3];
							readIntsInsideElement( xmlRead, triangle.IndexBuffer, triangle.NumPolygon * triangle.NumElementPerVertex *3 );
						}
					}					
					else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
					{
						if ( xmlRead->getNodeName() == trianglesNode )
							break;
					}				
				}

				// add triangles
				if ( triangle.IndexBuffer != NULL )
					mesh.Triangles.push_back( triangle );
			}
		}
		else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END)
		{		
			if ( xmlRead->getNodeName() == geometrySectionName )
				break;
		}
	}
	
	m_listMesh.push_back( mesh );

}

// parseControllersNode
// parse controllser
void CColladaMeshComponent::parseControllerNode( io::IXMLReader *xmlRead )
{	
	const std::wstring skinSectionName(L"skin");
	const std::wstring controllerSectionName(L"controller");

	std::wstring controllerName = xmlRead->getAttributeValue(L"id");	

	while(xmlRead->read())
	{							
		if (xmlRead->getNodeType() == io::EXN_ELEMENT )
		{
			std::wstring node = xmlRead->getNodeName();

			if ( node == skinSectionName )
			{
				SMeshParam *mesh = parseSkinNode( xmlRead );

				if ( mesh )
					mesh->ControllerName = controllerName;
			}
		}
		else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
		{
			if ( xmlRead->getNodeName() == controllerSectionName )
				break;
		}	
	}
}

// parseSkinNode
// parse skin data
SMeshParam* CColladaMeshComponent::parseSkinNode( io::IXMLReader *xmlRead )
{
	std::wstring source = xmlRead->getAttributeValue(L"source");
	source.erase( source.begin() );

	int meshID = getMeshWithUri( source, m_listMesh );
	if ( meshID == -1 )
		return NULL;
	
	SMeshParam *mesh = &m_listMesh[ meshID ];
	
	const std::wstring paramSectionName(L"param");
	const std::wstring skinSectionName(L"skin");
	const std::wstring jointsSectionName(L"joints");
	const std::wstring weightsSectionName(L"vertex_weights");
	const std::wstring nameArraySectionName(L"Name_array");
	const std::wstring floatArraySectionName(L"float_array");
	const std::wstring intArraySectionName(L"int_array");
	const std::wstring vcountNode(L"vcount");
	const std::wstring vNode(L"v");
	const std::wstring sourceNode(L"source");
	const std::wstring bindShapeMatrix(L"bind_shape_matrix");

	std::vector<std::wstring>	nameArray;

	int						numArray = 0;
	float					*jointArray = NULL;
	float					*transformArray = NULL;
	float					*weightArray = NULL;
	
	std::vector<s32>				&vCountArray	= mesh->JointVertexIndex;
	std::vector<s32>				vArray;

	while(xmlRead->read())
	{							
		if (xmlRead->getNodeType() == io::EXN_ELEMENT )
		{
			std::wstring node = xmlRead->getNodeName();
			
			if ( node == bindShapeMatrix )
			{
				float f[16];
				readFloatsInsideElement(  xmlRead, f, 16 );
				
				core::matrix4 mat;
				mat.setM( f );		
				if (m_needFlip)
				{
					core::matrix4 mat2(mat, core::matrix4::EM4CONST_TRANSPOSED);

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

					mesh->BindShapeMatrix = mat2;
				}
				else
				{					
					mesh->BindShapeMatrix = mat.getTransposed();
				}
			}
			// <source>
			else if ( node == sourceNode )
			{
				float *f = NULL;
			
				std::wstring sourceId = xmlRead->getAttributeValue(L"id");
				
				while(xmlRead->read())
				{
					if (xmlRead->getNodeType() == io::EXN_ELEMENT )
					{
						// <Name_array>
						if ( xmlRead->getNodeName() == nameArraySectionName )
						{
							int count = xmlRead->getAttributeValueAsInt(L"count");
							nameArray.resize( count );
							numArray = count;
							readStringInsideElement( xmlRead, nameArray );
						}
						// <float_array>
						else if ( xmlRead->getNodeName() == floatArraySectionName )
						{
							int count = xmlRead->getAttributeValueAsInt(L"count");
							numArray = count;
							f = new float[count];
							readFloatsInsideElement( xmlRead, f, count );
						}
						// <param>	inside <accessor>
						else if ( xmlRead->getNodeName() == paramSectionName )
						{
							if ( xmlRead->getAttributeValue(L"name") != NULL )
							{
								std::wstring name = xmlRead->getAttributeValue(L"name");
								if ( name == L"TRANSFORM" )
									transformArray = f;
								else if ( name == L"WEIGHT" )
									weightArray = f;
								else
								{
									// delete float buffer on another accessor
									if ( f != NULL )
									{
										delete f;
										f = NULL;
									}
								}
							}
							else
							{
								if ( sourceId.find(L"-Matrices") != std::wstring::npos )
									transformArray = f;
								else if ( sourceId.find(L"-Weights") != std::wstring::npos )
									weightArray = f;
								else
								{
									// delete float buffer on another accessor
									if ( f != NULL )
									{
										delete f;
										f = NULL;
									}
								}
							}
						}
					}				
					else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
					{
						if ( xmlRead->getNodeName() == sourceNode )
							break;
					}
				}
			}
			// <vcount>
			else if ( node == vcountNode )
			{
				readIntsInsideElement( xmlRead, vCountArray );
			}
			// <vNode>
			else if ( node == vNode )
			{
				readIntsInsideElement( xmlRead, vArray );
			}
		}		
		else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
		{
			if ( xmlRead->getNodeName() == skinSectionName )
				break;
		}
	}
	
	mesh->Type = k_skinMesh;
	updateJointToMesh( mesh, nameArray, weightArray, transformArray, vCountArray, vArray, m_needFlip );

	if ( weightArray )
		delete weightArray;

	if ( transformArray )
		delete transformArray;

	return mesh;
}

// parseSceneNode
// parse scene data
void CColladaMeshComponent::parseSceneNode( io::IXMLReader *xmlRead )
{
	const std::wstring sceneSectionName(L"visual_scene");
	const std::wstring nodeSectionName(L"node");

	while(xmlRead->read())
	{			
		std::wstring nodeName =	xmlRead->getNodeName();

		if (xmlRead->getNodeType() == io::EXN_ELEMENT )
		{
			if ( nodeName == nodeSectionName )
			{
				SNodeParam* pNewNode = parseNode( xmlRead, NULL );
				m_listNode.push_back( pNewNode );
			}
		}
		else if ( xmlRead->getNodeType() == io::EXN_ELEMENT_END )
		{
			if ( nodeName == sceneSectionName )
				break;
		}
	}
}

// parseNode
// parse <node> element
SNodeParam* CColladaMeshComponent::parseNode( io::IXMLReader *xmlRead, SNodeParam* parent )
{	
	const std::wstring nodeSectionName(L"node");
	const std::wstring translateSectionName(L"translate");
	const std::wstring rotateSectionName(L"rotate");
	const std::wstring scaleSectionName(L"scale");
	const std::wstring matrixNodeName(L"matrix");

	const std::wstring instanceGeometrySectionName(L"instance_geometry");
	const std::wstring instanceControllerSectionName(L"instance_controller");

	SNodeParam *pNode = new SNodeParam();
	
	pNode->Name = L"";
	if ( xmlRead->getAttributeValue(L"id") )
		pNode->Name = xmlRead->getAttributeValue(L"id");

	if ( xmlRead->getAttributeValue(L"sid") )
		pNode->SID	= xmlRead->getAttributeValue(L"sid");	
	
	pNode->Type = L"NODE";
	if ( xmlRead->getAttributeValue(L"type") )
		pNode->Type = xmlRead->getAttributeValue(L"type");
	
	pNode->Parent = parent;
	if ( parent )
	{
		parent->Childs.push_back( pNode );
		pNode->ChildLevel = parent->ChildLevel+1;
	}
	else
		pNode->ChildLevel = 0;


	while(xmlRead->read())
	{
		if (xmlRead->getNodeType() == io::EXN_ELEMENT )
		{
			if ( xmlRead->getNodeName() == nodeSectionName )
			{
				parseNode( xmlRead, pNode );
			}
			else if ( xmlRead->getNodeName() == translateSectionName )
			{
				// mul translate
				pNode->Transform *= readTranslateNode(xmlRead, m_needFlip);
			}
			else if ( xmlRead->getNodeName() == rotateSectionName )
			{
				// mul rotate
				pNode->Transform *= readRotateNode(xmlRead, m_needFlip);
			}
			else if ( xmlRead->getNodeName() == scaleSectionName )
			{
				// mul scale
				pNode->Transform *= readScaleNode(xmlRead, m_needFlip);
			}			
			else if ( xmlRead->getNodeName() == matrixNodeName)
			{
				// mull matix
				pNode->Transform *= readMatrixNode(xmlRead, m_needFlip);
			}
			else if ( xmlRead->getNodeName() == instanceGeometrySectionName )
			{
				// <instance_geometry url="#MESHNAME">
				pNode->Instance = xmlRead->getAttributeValue(L"url");
				uriToId( pNode->Instance );
			}
			else if ( xmlRead->getNodeName() == instanceControllerSectionName )
			{
				// <instance_controller url="#MESHNAME">
				pNode->Instance = xmlRead->getAttributeValue(L"url");
				uriToId( pNode->Instance );
			}

		}
		else if ( xmlRead->getNodeType() == io::EXN_ELEMENT_END )
		{
			if ( xmlRead->getNodeName() == nodeSectionName )
				break;
		}
	}

	return pNode;
}

// parseImageNode
// parse image
void CColladaMeshComponent::parseImageNode( io::IXMLReader *xmlRead, SImage* image )
{
	if ( image == NULL )
	{		
		m_listImages.push_back( SImage() );
		image = &m_listImages.back();		
	}

	const std::wstring imageSectionName(L"image");

	image->id = xmlRead->getAttributeValue(L"id");
	image->name = xmlRead->getAttributeValue(L"name");
	
	bool readPath = false;

	while(xmlRead->read())
	{
		if (xmlRead->getNodeType() == io::EXN_ELEMENT)
		{			
			std::wstring nodeName = xmlRead->getNodeName();
			if ( nodeName == L"init_from" )
				readPath = true;
		}
		else if ( xmlRead->getNodeType() == io::EXN_TEXT )
		{
			if ( readPath )
				image->fileName = xmlRead->getNodeData();
			readPath = false;
		}
		else if ( xmlRead->getNodeType() == io::EXN_ELEMENT_END )
		{
			std::wstring nodeName = xmlRead->getNodeName();
			if ( nodeName == imageSectionName )
				break;
		}
	}
}


// parseMaterialNode
// parse material
void CColladaMeshComponent::parseMaterialNode( io::IXMLReader *xmlRead, SEffect* effect )
{
	if ( effect == NULL )
	{		
		m_listMaterial.push_back( SEffect() );
		effect = &m_listMaterial.back();

		effect->Id = readId(xmlRead);
		effect->Transparency = 1.f;
		effect->Mat.Lighting = true;
		effect->Mat.NormalizeNormals = true;
		effect->HasAlpha = false;
	}
		
	const std::wstring materialSectionName(L"material");
	const std::wstring initFrom(L"init_from");
	
	bool hasTexture = false;
	bool readInitFrom = 0;

	while(xmlRead->read())
	{
		if (xmlRead->getNodeType() == io::EXN_ELEMENT)
		{		
			// read the first image init as texture
			std::wstring nodeName = xmlRead->getNodeName();
			if ( nodeName == initFrom && hasTexture == false )
			{				
				readInitFrom = 1;
			}
		}
		else if (xmlRead->getNodeType() == io::EXN_TEXT)
		{
			// read texture
			if ( hasTexture == false && readInitFrom == 1 )
			{
				std::wstring tname = xmlRead->getNodeData();
				effect->Mat.setTexture( 0, getTextureFromImage( m_animeshFile, tname, m_listImages ) );
				hasTexture = true;
			}

			readInitFrom = 0;
		}
		else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END)
		{
			// end parse material
			std::wstring nodeName = xmlRead->getNodeName();
			if ( nodeName == materialSectionName )
				break;
		}
	}

}

// parseEffectNode
// parse effect material node
void CColladaMeshComponent::parseEffectNode( io::IXMLReader *xmlRead, SEffect* effect )
{
	if ( effect == NULL )
	{		
		m_listEffects.push_back( SEffect() );
		effect = &m_listEffects.back();

		effect->Id = readId(xmlRead);
		effect->Transparency = 1.f;
		effect->Mat.Lighting = true;
		effect->Mat.NormalizeNormals = true;
		effect->HasAlpha = false;
		effect->TransparentAddColor = false;
	}

	const std::wstring constantNode(L"constant");
	const std::wstring lambertNode(L"lambert");
	const std::wstring phongNode(L"phong");
	const std::wstring blinnNode(L"blinn");
	const std::wstring bumpNode(L"bump");
	const std::wstring emissionNode(L"emission");
	const std::wstring ambientNode(L"ambient");
	const std::wstring diffuseNode(L"diffuse");
	const std::wstring specularNode(L"specular");
	const std::wstring shininessNode(L"shininess");
	const std::wstring reflectiveNode(L"reflective");
	const std::wstring reflectivityNode(L"reflectivity");
	const std::wstring transparentNode(L"transparent");
	const std::wstring transparencyNode(L"transparency");
	const std::wstring indexOfRefractionNode(L"index_of_refraction");	
	const std::wstring effectSectionName(L"effect");	
	const std::wstring profileCOMMONSectionName(L"profile_COMMON");
	const std::wstring techniqueNodeName(L"technique");
	const std::wstring colorNodeName(L"color");
	const std::wstring floatNodeName(L"float");
	const std::wstring textureNodeName(L"texture");
	const std::wstring extraNodeName(L"extra");
	const std::wstring newParamName(L"newparam");

	const std::wstring initFromNode(L"init_from");
	const std::wstring sourceNode(L"source");

	bool hasBumpMapping = false;

	bool hasLoadedDiffuseTex = false;

	while(xmlRead->read())
	{
		if (xmlRead->getNodeType() == io::EXN_ELEMENT)
		{			
			std::wstring nodeName = xmlRead->getNodeName();
			
			if ( nodeName == profileCOMMONSectionName || nodeName == techniqueNodeName )
			{
				parseEffectNode(xmlRead,effect);
			}
			else if ( nodeName == newParamName )
			{
				int m_readState = 0;
					
				SEffectParam effectParam;
				
				if ( xmlRead->getAttributeValue(L"sid") )
					effectParam.Name = xmlRead->getAttributeValue(L"sid");
				
				while(xmlRead->read())
				{
					std::wstring node = xmlRead->getNodeName();

					if (xmlRead->getNodeType() == io::EXN_ELEMENT)
					{
						if (node == initFromNode)
							m_readState = 1;
						else if ( node == sourceNode )
							m_readState = 2;

					}
					else if (xmlRead->getNodeType() == io::EXN_TEXT)
					{
						if ( m_readState == 1 )
							effectParam.InitFromTexture = xmlRead->getNodeData();
						else if ( m_readState == 2 )
							effectParam.Source = xmlRead->getNodeData();

						m_readState = 0;
					}
					else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END)
					{
						if ( nodeName == newParamName )
							break;
					}
				}

				// add to list effect params
				m_listEffectsParam.push_back( effectParam );

			}
			else if ( constantNode == nodeName || lambertNode == nodeName || phongNode == nodeName || blinnNode == nodeName || bumpNode == nodeName )
			{
				// set phong shading
				effect->Mat.setFlag(irr::video::EMF_GOURAUD_SHADING, phongNode == nodeName || blinnNode == nodeName);
								
				// read color type
				while(xmlRead->read())
				{
					if (xmlRead->getNodeType() == io::EXN_ELEMENT)
					{
						std::wstring node = xmlRead->getNodeName();

						if ( emissionNode == node || ambientNode == node || diffuseNode == node || 
							 specularNode == node || reflectiveNode == node || transparentNode == node )
						{

							// read transparent type
							if ( node == transparentNode )
							{
								if ( xmlRead->getAttributeValue(L"opaque") )
								{
									if ( std::wstring(L"RGB_ZERO") == xmlRead->getAttributeValue(L"opaque") )
									{
										effect->TransparentAddColor = true;
									}
								}
							}

							// read color
							while(xmlRead->read())
							{
								if (xmlRead->getNodeType() == io::EXN_ELEMENT) 									
								{
									if ( colorNodeName == xmlRead->getNodeName() )
									{
										const video::SColorf colorf = readColorNode( xmlRead );
										const video::SColor color = colorf.toSColor();

										if (emissionNode == node)
											effect->Mat.EmissiveColor = color;
										else if (ambientNode == node)
											effect->Mat.AmbientColor = color;
										else if (diffuseNode == node)
											effect->Mat.DiffuseColor = color;
										else if (specularNode == node)
											effect->Mat.SpecularColor = color;
										else if (transparentNode == node)
										{
											effect->Transparency = colorf.getAlpha();
											if ( effect->Transparency != 1.0f )
												effect->HasAlpha = true;
										}
									}
									else if ( textureNodeName == xmlRead->getNodeName() )
									{
										std::wstring tname = xmlRead->getAttributeValue(L"texture");
										
										if ( node == diffuseNode )
										{
											if ( hasLoadedDiffuseTex == false )
											{
												ITexture *tex = getTextureFromImage( m_animeshFile, tname, m_listEffectsParam, m_listImages );
												if ( tex == NULL )
													tex = getTextureFromImage( m_animeshFile, tname, m_listImages );
												if ( tex )
													effect->Mat.setTexture(0, tex);
												hasLoadedDiffuseTex = true;
											}
										}
										else if ( node == ambientNode )
										{
											// ambient lightmap texture: todo later
										}
										else if ( node == transparentNode )
										{
											// alpha node
											effect->HasAlpha = true;
										}
									}								
								}
								else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
								{
									if ( node == xmlRead->getNodeName() )
										break;
								}
							}
						}
						else if ( textureNodeName == node )
						{						
							if ( nodeName == bumpNode )
							{
								std::wstring tname = xmlRead->getAttributeValue(L"texture");
								// bump mapping texture: todo later
							}
						}
						else if (shininessNode == node || reflectivityNode == node || transparencyNode == node || indexOfRefractionNode == node )
						{
							// float or param types
							while( xmlRead->read() )
							{
								if (xmlRead->getNodeType() == io::EXN_ELEMENT )
								{
									if ( floatNodeName == xmlRead->getNodeName() )
									{
										f32 f = readFloatNode(xmlRead);
										if (shininessNode == node)
											effect->Mat.Shininess = f;
										else if (transparencyNode == node)
											effect->Transparency *= f;
									}
								}
								else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
								{
									if ( node == xmlRead->getNodeName() )
										break;
								}
							}
						}						
					}
					else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END )
					{
						if ( constantNode == xmlRead->getNodeName() || 
							lambertNode == xmlRead->getNodeName() ||
							phongNode == xmlRead->getNodeName() || 
							blinnNode == xmlRead->getNodeName() || 
							bumpNode == xmlRead->getNodeName() )
							break;
					}
				}
			}
			else if ( extraNodeName == nodeName )
				parseEffectNode(xmlRead, effect);
		}
		else if (xmlRead->getNodeType() == io::EXN_ELEMENT_END)
		{
			std::wstring nodeName = xmlRead->getNodeName();

			if (effectSectionName == nodeName)
				break;			
			else if (profileCOMMONSectionName == nodeName)
				break;
			else if (techniqueNodeName == nodeName)
				break;
			else if (extraNodeName == nodeName)
				break;
		}
	}


	if (effect->Mat.AmbientColor == video::SColor(0) && effect->Mat.DiffuseColor != video::SColor(0))
		effect->Mat.AmbientColor = effect->Mat.DiffuseColor;
	
	if (effect->Mat.DiffuseColor == video::SColor(0) && effect->Mat.AmbientColor != video::SColor(0))
		effect->Mat.DiffuseColor = effect->Mat.AmbientColor;

	if ( effect->HasAlpha == true )
	{
		if ( effect->Transparency != 1.0f )
		{
			effect->Mat.MaterialType = irr::video::EMT_TRANSPARENT_VERTEX_ALPHA;
			effect->Mat.ZWriteEnable = false;
		}
		else
		{
			if ( effect->TransparentAddColor )
				effect->Mat.MaterialType = irr::video::EMT_TRANSPARENT_ADD_COLOR;
			else
				effect->Mat.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
		}
		effect->Mat.BackfaceCulling = false;
		effect->Mat.FrontfaceCulling = false;
	}	

	effect->Mat.setFlag(video::EMF_BILINEAR_FILTER, true);
	effect->Mat.setFlag(video::EMF_TRILINEAR_FILTER, true);
	effect->Mat.setFlag(video::EMF_ANISOTROPIC_FILTER, true);
	effect->Mat.Shininess = 0.0f;
}


#pragma endregion

#pragma region CONSTRUCT_DAE_DATA

// constructMeshBuffer
// create mesh buffer
void CColladaMeshComponent::constructMeshBuffer( SMeshParam *mesh, STrianglesParam* tri, IMeshBuffer *buffer, bool flip )
{
	SColladaMeshBuffer *mbuffer = (SColladaMeshBuffer*) buffer;
					
	SVerticesParam	*vertices = &mesh->Vertices[ tri->VerticesIndex ];
	
	SEffect	*effect = NULL;
	SBufferParam *position	= NULL;
	SBufferParam *normal	= NULL;
	SBufferParam *texCoord1 = NULL;
	SBufferParam *texCoord2 = NULL;
		
	if ( tri->EffectIndex != -1 )
		effect = &m_listEffects[ tri->EffectIndex ];

	if ( vertices->PositionIndex == -1 )
		return;

	// position buffer
	position = &mesh->Buffers[vertices->PositionIndex];

	if ( vertices->NormalIndex != -1 )
		normal = &mesh->Buffers[vertices->NormalIndex];

	if ( vertices->TexCoord1Index != -1 )
		texCoord1 =	&mesh->Buffers[vertices->TexCoord1Index];

	if ( vertices->TexCoord2Index != -1 )
		texCoord2 =	&mesh->Buffers[vertices->TexCoord2Index];

	// alloc vertex
	int vertexCount = position->ArrayCount/3;
	mbuffer->Vertices.reallocate(mbuffer->Vertices.size() + vertexCount);
		
	core::array<u16> indices;

	for ( int i = 0; i < vertexCount; i++ )
	{
		video::S3DVertex vtx;
		vtx.Color = SColor(0xFFFFFFFF);

		int vIndex = i;
		int idx = vIndex * position->Strike;

		// set position
		vtx.Pos.X = position->FloatArray[idx+0];
		if (flip)
		{
			vtx.Pos.Z = position->FloatArray[idx+1];
			vtx.Pos.Y = position->FloatArray[idx+2];
		}
		else
		{
			vtx.Pos.Y = position->FloatArray[idx+1];
			vtx.Pos.Z = position->FloatArray[idx+2];
		}

		if ( tri->NumElementPerVertex == 1 )
		{
			// set normal
			if ( normal != NULL )
			{		
				int idx = vIndex * normal->Strike;

				vtx.Normal.X = normal->FloatArray[idx+0];
				if (flip)
				{
					vtx.Normal.Z = normal->FloatArray[idx+1];
					vtx.Normal.Y = normal->FloatArray[idx+2];
				}
				else
				{
					vtx.Normal.Y = normal->FloatArray[idx+1];
					vtx.Normal.Z = normal->FloatArray[idx+2];
				}
			}

			// set texcoord
			if ( texCoord1 ) 
			{
				idx = vIndex * texCoord1->Strike;
				if ( texCoord1 != NULL )
				{
					vtx.TCoords.X = texCoord1->FloatArray[idx+0];
					vtx.TCoords.Y = texCoord1->FloatArray[idx+1];
				}
			}
		}
		mbuffer->Vertices.push_back( vtx );
	}
		
	indices.set_used( tri->NumPolygon * 3 );
	

	int totalElement = tri->NumPolygon * tri->NumElementPerVertex * 3;	
	int index = 0;
	for ( int i = 0; i < totalElement; i+= tri->NumElementPerVertex)
	{
		indices[index] = tri->IndexBuffer[i];
	
		if ( tri->NumElementPerVertex != 1 )
		{
			video::S3DVertex &vtx = mbuffer->Vertices[ tri->IndexBuffer[i] ];
			if ( normal != NULL )
			{
				int idx = tri->IndexBuffer[i + tri->OffsetNormal] * normal->Strike;
				vtx.Normal.X = normal->FloatArray[idx];
				if (flip)
				{
					vtx.Normal.Z = normal->FloatArray[idx+1];
					vtx.Normal.Y = normal->FloatArray[idx+2];
				}
				else
				{
					vtx.Normal.Y = normal->FloatArray[idx+1];
					vtx.Normal.Z = normal->FloatArray[idx+2];
				}
			}

			if ( texCoord1 != NULL ) 
			{
				int idx = tri->IndexBuffer[i + tri->OffsetTexcoord1] * texCoord1->Strike;
				vtx.TCoords.X = texCoord1->FloatArray[idx];
				vtx.TCoords.Y = 1.0f - texCoord1->FloatArray[idx+1];
			}
		}

		index++;
	}


	// it's just triangles
	u32 nPoly = indices.size()/3;

	for (u32 i = 0; i < nPoly; i++)
	{
		u32 ind = i * 3;

		if (flip)
		{
			mbuffer->Indices.push_back(indices[ind+2]);
			mbuffer->Indices.push_back(indices[ind+1]);
			mbuffer->Indices.push_back(indices[ind+0]);
		}
		else
		{
			mbuffer->Indices.push_back(indices[ind+0]);
			mbuffer->Indices.push_back(indices[ind+1]);
			mbuffer->Indices.push_back(indices[ind+2]);
		}
	}
	
	
	// we need optimize buffer
	// todo remove unused vertex
	int *mapIdx = new int[ mbuffer->getVertexCount() ];
	for ( int i = 0, nVertex = mbuffer->getVertexCount(); i < nVertex; i++ )
		mapIdx[i] = -1;	

	core::array<S3DVertex>	vertexBuffer;
	core::array<u16>		indexBuffer;
	
	for ( int i = 0, nIndex = mbuffer->getIndexCount(); i < nIndex; i++ )
	{
		int idx = mbuffer->Indices[i];
		S3DVertex& v = mbuffer->Vertices[idx];

		if ( mapIdx[idx] == -1 )
		{
			vertexBuffer.push_back(v);
			
			int vertexID = vertexBuffer.size() - 1;
			indexBuffer.push_back( vertexID );
			mapIdx[idx] = vertexID;
		}
		else
		{
			indexBuffer.push_back( mapIdx[idx] );
		}
	}

	delete mapIdx;

	mbuffer->Indices = indexBuffer;
	mbuffer->Vertices = vertexBuffer;

	// set material
	if ( effect )
	{
		mbuffer->getMaterial() = effect->Mat;		
	}

	// calc normal vector
	if (!normal)
		getIView()->getSceneMgr()->getMeshManipulator()->recalculateNormals(mbuffer, true);

	// recalculate bounding box
	mbuffer->recalculateBoundingBox();
}

void CColladaMeshComponent::constructSkinMeshBuffer( SMeshParam *mesh,	STrianglesParam* tri, IMeshBuffer *buffer, bool flip )
{
	SColladaSkinMeshBuffer *mbuffer = (SColladaSkinMeshBuffer*) buffer;
					
	SVerticesParam	*vertices = &mesh->Vertices[ tri->VerticesIndex ];
	
	SEffect	*effect = NULL;
	SBufferParam *position	= NULL;
	SBufferParam *normal	= NULL;
	SBufferParam *texCoord1 = NULL;
	SBufferParam *texCoord2 = NULL;
		
	if ( tri->EffectIndex != -1 )
		effect = &m_listEffects[ tri->EffectIndex ];

	if ( vertices->PositionIndex == -1 )
		return;

	// position buffer
	position = &mesh->Buffers[vertices->PositionIndex];

	if ( vertices->NormalIndex != -1 )
		normal = &mesh->Buffers[vertices->NormalIndex];

	if ( vertices->TexCoord1Index != -1 )
		texCoord1 =	&mesh->Buffers[vertices->TexCoord1Index];

	if ( vertices->TexCoord2Index != -1 )
		texCoord2 =	&mesh->Buffers[vertices->TexCoord2Index];

	// alloc vertex
	int vertexCount = position->ArrayCount/3;
	mbuffer->Vertices.reallocate(mbuffer->Vertices.size() + vertexCount);
		
	core::array<u16> indices;

	for ( int i = 0; i < vertexCount; i++ )
	{
		video::S3DVertexSkin vtx;
		vtx.Color = SColor(0xFFFFFFFF);

		int vIndex = i;
		int idx = vIndex * position->Strike;

		// set position
		vtx.Pos.X = position->FloatArray[idx+0];
		if (flip)
		{
			vtx.Pos.Z = position->FloatArray[idx+1];
			vtx.Pos.Y = position->FloatArray[idx+2];
		}
		else
		{
			vtx.Pos.Y = position->FloatArray[idx+1];
			vtx.Pos.Z = position->FloatArray[idx+2];
		}

		if ( tri->NumElementPerVertex == 1 )
		{
			// set normal
			if ( normal != NULL )
			{		
				int idx = vIndex * normal->Strike;

				vtx.Normal.X = normal->FloatArray[idx+0];
				if (flip)
				{
					vtx.Normal.Z = normal->FloatArray[idx+1];
					vtx.Normal.Y = normal->FloatArray[idx+2];
				}
				else
				{
					vtx.Normal.Y = normal->FloatArray[idx+1];
					vtx.Normal.Z = normal->FloatArray[idx+2];
				}
			}

			// set texcoord
			if ( texCoord1 ) 
			{
				idx = vIndex * texCoord1->Strike;
				if ( texCoord1 != NULL )
				{
					vtx.TCoords.X = texCoord1->FloatArray[idx+0];
					vtx.TCoords.Y = texCoord1->FloatArray[idx+1];
				}
			}
		}		
		mbuffer->Vertices.push_back( vtx );
	}
		
	indices.set_used( tri->NumPolygon * 3 );

	int totalElement = tri->NumPolygon * tri->NumElementPerVertex * 3;	
	int index = 0;
	for ( int i = 0; i < totalElement; i+= tri->NumElementPerVertex)
	{
		indices[index] = tri->IndexBuffer[i];	

		if ( tri->NumElementPerVertex != 1 )
		{
			video::S3DVertexSkin &vtx = mbuffer->Vertices[ tri->IndexBuffer[i] ];
			if ( normal != NULL )
			{
				int idx = tri->IndexBuffer[i + tri->OffsetNormal] * normal->Strike;
				vtx.Normal.X = normal->FloatArray[idx];
				if (flip)
				{
					vtx.Normal.Z = normal->FloatArray[idx+1];
					vtx.Normal.Y = normal->FloatArray[idx+2];
				}
				else
				{
					vtx.Normal.Y = normal->FloatArray[idx+1];
					vtx.Normal.Z = normal->FloatArray[idx+2];
				}
			}

			if ( texCoord1 != NULL ) 
			{
				int idx = tri->IndexBuffer[i + tri->OffsetTexcoord1] * texCoord1->Strike;
				vtx.TCoords.X = texCoord1->FloatArray[idx];
				vtx.TCoords.Y = 1.0f - texCoord1->FloatArray[idx+1];
			}
		}

		index++;
	}


	// it's just triangles
	u32 nPoly = indices.size()/3;

	for (u32 i = 0; i < nPoly; i++)
	{
		u32 ind = i * 3;

		if (flip)
		{
			mbuffer->Indices.push_back(indices[ind+2]);
			mbuffer->Indices.push_back(indices[ind+1]);
			mbuffer->Indices.push_back(indices[ind+0]);
		}
		else
		{
			mbuffer->Indices.push_back(indices[ind+0]);
			mbuffer->Indices.push_back(indices[ind+1]);
			mbuffer->Indices.push_back(indices[ind+2]);
		}
	}
		
	// set material
	if ( effect )
	{
		mbuffer->getMaterial() = effect->Mat;		
	}

	// calc normal vector
	if (!normal)
		getIView()->getSceneMgr()->getMeshManipulator()->recalculateNormals(mbuffer, true);

	// recalculate bounding box
	mbuffer->recalculateBoundingBox();
}


// constructSkinMesh
// apply bone to vertex
void CColladaMeshComponent::constructSkinMesh( SMeshParam *meshParam, CGameColladaMesh *mesh )
{		
	char sidName[1024];

	int nJoint = meshParam->Joints.size();
	
	// set up joint
	// --------------------------
	for ( int i = 0; i < nJoint; i++ )
	{						
		mesh->Joints.push_back( CGameColladaMesh::SJoint() );		
		CGameColladaMesh::SJoint& newJoint = mesh->Joints.getLast();

		SJointParam& joint = meshParam->Joints[i];

		// set joint name
		uiString::copy<char, const wchar_t>( sidName, joint.Name.c_str() );
		newJoint.name = joint.Name;
		newJoint.node = m_sidNode[ std::string(sidName) ];

		// set invert matrix
		newJoint.globalInversedMatrix = joint.InvMatrix;
		newJoint.node->GlobalInversedMatrix = joint.InvMatrix;
	}

	// setup vertex bone buffer
	// ----------------------------
	SMeshBufferSkin *skinBuffer = (SMeshBufferSkin*)mesh->getMeshBuffer(0);
	int numVertex = skinBuffer->getVertexCount();
	
	int *nBoneCount = new int[numVertex];
	memset(nBoneCount, 0, sizeof(int)*numVertex);

	for ( int i = 0, n = (int)meshParam->JointIndex.size(); i < n; i+=2 )
	{
		int boneID		= meshParam->JointIndex[i];
		int weightID	= meshParam->JointIndex[i + 1];

		SJointParam& joint		= meshParam->Joints[boneID];
		SWeightParam& weight	= joint.Weights[weightID];
		
		int nBone = nBoneCount[ weight.VertexID ]++;	

		video::S3DVertexSkin *vertex = NULL;			
		for (int i = 0, n = mesh->getMeshBufferCount(); i < n; i++ )
		{
			SColladaMeshBuffer *buffer = (SColladaMeshBuffer*)mesh->getMeshBuffer(i);
			vertex = (video::S3DVertexSkin*)buffer->getVertices();
			
			float* boneIndex		= (float*)&(vertex[ weight.VertexID ].BoneIndex);
			float* boneWeight		= (float*)&(vertex[ weight.VertexID ].BoneWeight);

			// only support 4bones affect on 1vertex
			if ( nBone >= 4 )
			{
				int		minIndex = 0;
				float	minWeight = boneWeight[0];
				
				for ( int i = 1; i < 4; i++ )
				{
					if ( boneWeight[i] < minWeight )
					{
						minIndex = i;
						minWeight = boneWeight[i];
						break;
					}					
				}

				if ( weight.Strength > minWeight )
				{
					boneIndex[minIndex]		= (float)boneID;
					boneWeight[minIndex]	= weight.Strength;
				}
			}
			else
			{								
				boneIndex[nBone]		= (float)boneID;
				boneWeight[nBone]		= weight.Strength;
			}

			vertex[ weight.VertexID ].StaticPos		= vertex[ weight.VertexID ].Pos;
			vertex[ weight.VertexID ].StaticNormal	= vertex[ weight.VertexID ].Normal;
		}
	}

	delete nBoneCount;


	// we need optimize buffer
	// todo remove unused vertex
	for ( int i =0, nBuffer = mesh->getMeshBufferCount(); i < nBuffer; i++ )
	{
		SMeshBufferSkin *mbuffer = (SMeshBufferSkin*)mesh->getMeshBuffer(i);

		int *mapIdx = new int[ mbuffer->getVertexCount() ];
		for ( int i = 0, nVertex = mbuffer->getVertexCount(); i < nVertex; i++ )
			mapIdx[i] = -1;	

		core::array<S3DVertexSkin>	vertexBuffer;
		core::array<u16>			indexBuffer;
		
		for ( int i = 0, nIndex = mbuffer->getIndexCount(); i < nIndex; i++ )
		{
			int idx = mbuffer->Indices[i];
			S3DVertexSkin& v = mbuffer->Vertices[idx];

			if ( mapIdx[idx] == -1 )
			{
				vertexBuffer.push_back(v);
				
				int vertexID = vertexBuffer.size() - 1;
				indexBuffer.push_back( vertexID );
				mapIdx[idx] = vertexID;
			}
			else
			{
				indexBuffer.push_back( mapIdx[idx] );
			}
		}

		delete mapIdx;

		mbuffer->Indices = indexBuffer;
		mbuffer->Vertices = vertexBuffer;
	}
}


void CColladaMeshComponent::constructScene()
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	// clear all map
	m_mapNode.clear();

	// create new scene node
	m_colladaNode = new CGameColladaContainerSceneNode( 
			m_gameObject, 
			m_gameObject->getParentSceneNode(),
			smgr,
			m_gameObject->getID() 
		);
	

	// collada node
	m_gameObject->m_node = m_colladaNode;


	std::list<SNodeParam*>	stackScene;
	std::list<SNodeParam*>	listScene;

	int nNode = m_listNode.size();
	for ( int i = 0; i < nNode; i++ )
	{
		SNodeParam* root = m_listNode[i];
		stackScene.push_back( root );
	}

	while ( stackScene.size() )
	{
		SNodeParam *node = stackScene.back();
		
		// save to list bone prefab
		listScene.push_back( node );
		
		// to do create node
		char name[1024];
		uiString::copy<char, const wchar_t>( name, node->Name.c_str() );

		
		// create new scene node
		ISceneNode *parent = m_colladaNode;
		if ( node->Parent && node->Parent->SceneNode )
			parent = node->Parent->SceneNode;

		// crate new scene node
		CGameColladaSceneNode *colladaSceneNode = new CGameColladaSceneNode( parent, smgr, -1 );
		colladaSceneNode->setComponent( this );

		colladaSceneNode->setName( name );
		
		if ( node->Parent == NULL )
			colladaSceneNode->setRootColladaNode( true );
		else
			colladaSceneNode->setRootColladaNode( false );

		// get position from transform		
		node->SceneNode = colladaSceneNode;
		
		// store this node
		m_mapNode[name] = colladaSceneNode;

		// store joint sid node
		if ( node->SID.size() > 0 )
		{
			uiString::copy<char, const wchar_t>( name, node->SID.c_str() );
			m_sidNode[name] = colladaSceneNode;
			colladaSceneNode->setSIDName( name );
		}

		// set relative position		
		colladaSceneNode->setLocalMatrix( node->Transform );
		
		
		// construct geometry & controller in node
		if ( node->Instance.size() > 0 )
		{
			int meshID = getMeshWithUri( node->Instance, m_listMesh );
			if ( meshID == -1 )			
				meshID = getMeshWithControllerName( node->Instance, m_listMesh );

			if ( meshID != -1 )
			{
				SMeshParam *pMesh = &m_listMesh[meshID];

				CGameColladaMesh *pColladaMesh = new CGameColladaMesh();

				// need store component
				pColladaMesh->Component = this;
				pColladaMesh->BindShapeMatrix = pMesh->BindShapeMatrix;

				// add mesh buffer to skin mesh
				int nBuffer = pMesh->Triangles.size();


				for ( int i = 0; i < nBuffer; i++ )
				{
					STrianglesParam& tri = pMesh->Triangles[i];
					
					// create mesh buffer
					IMeshBuffer* meshBuffer = NULL;										
			
					if ( pMesh->Type == k_skinMesh )
					{
						meshBuffer = new SColladaSkinMeshBuffer();
						constructSkinMeshBuffer( pMesh, &tri, meshBuffer, m_needFlip );
						SColladaSkinMeshBuffer *mesh = (SColladaSkinMeshBuffer*)meshBuffer;						
					}
					else
					{
						meshBuffer = new SColladaMeshBuffer();
						constructMeshBuffer( pMesh, &tri, meshBuffer, m_needFlip );
						SColladaMeshBuffer *mesh = (SColladaMeshBuffer*)meshBuffer;						
					}

					// add mesh buffer								
					pColladaMesh->addMeshBuffer( meshBuffer );
					pColladaMesh->recalculateBoundingBox();

					meshBuffer->drop();
				}
				
				if ( pMesh->Type == k_skinMesh )				
					pColladaMesh->IsStaticMesh = false;
				else
				{
					pColladaMesh->IsStaticMesh = true;
					pColladaMesh->setHardwareMappingHint( EHM_STATIC );
				}

				// set mesh for scene node
				colladaSceneNode->setColladaMesh( pColladaMesh );
								
				pColladaMesh->drop();
			}

			// add update bounding box with this child
			if ( colladaSceneNode->ColladaMesh )
				((CGameColladaContainerSceneNode*)m_gameObject->m_node)->addBoundingMeshNode( colladaSceneNode );

		}
#ifdef GSANIMATION
		else
		{
			// add collision
			ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( colladaSceneNode );
			colladaSceneNode->setTriangleSelector(selector);
			selector->drop();
		}
#endif

		// pop stack
		stackScene.erase( --stackScene.end() );
		
		// add child to continue loop
		int nChild = (int)node->Childs.size();
		for ( int i = 0; i < nChild; i++ )
		{			
			SNodeParam *childNode = node->Childs[i];

			// set parent
			childNode->Parent = node;
			stackScene.push_back( childNode );		
		}
	}

	// clear node data
	std::list<SNodeParam*>::iterator i = listScene.begin(), end = listScene.end();
	while ( i != end )
	{
		SNodeParam *pNode = (*i);
		
		// apply skin
		CGameColladaMesh* pMesh = pNode->SceneNode->getMesh();
		if ( pMesh != NULL && pMesh->IsStaticMesh == false )
		{
			int meshID = getMeshWithControllerName( pNode->Instance, m_listMesh );
			constructSkinMesh( &m_listMesh[meshID], pMesh);			
		}

		// clear node
		pNode->Joint = NULL;		
		pNode->SceneNode->drop();
		pNode->SceneNode = NULL;
		i++;
	}
	listScene.clear();
}


// cleanData
// free all data from parse dae
void CColladaMeshComponent::cleanData()
{
	m_listEffectsParam.clear();
	m_listEffects.clear();	
	m_listImages.clear();
	m_listMaterial.clear();

	ArrayMeshParams::iterator i = m_listMesh.begin(), end = m_listMesh.end();
	while ( i != end )
	{
		SMeshParam& mesh = (*i);
		
		int n = (int)mesh.Buffers.size();
		int j = 0;

		for ( j = 0; j < n; j++ )
			delete mesh.Buffers[j].FloatArray;

		n = (int)mesh.Triangles.size();
		for ( j = 0; j < n; j++ )
			delete mesh.Triangles[j].IndexBuffer;

		i++;
	}
	m_listMesh.clear();

	for ( int j = 0; j < (int)m_listNode.size(); j++ )
	{
		SNodeParam* pNode = m_listNode[j];
		
		std::stack<SNodeParam*>	stackNode;
		stackNode.push( pNode );
		while ( stackNode.size() )
		{
			pNode = stackNode.top();
			stackNode.pop();

			for ( int i = 0; i < (int)pNode->Childs.size(); i++ )
				stackNode.push( pNode->Childs[i] );

			delete pNode;
		}		
	}
	m_listNode.clear();
}


#pragma endregion


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
			m_gameObject->getID()
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
			m_gameObject->getID()
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

// updateJointToMesh
// update joint
void CColladaMeshComponent::updateJointToMesh( SMeshParam *mesh, std::vector<std::wstring>& arrayName, float *arrayWeight, float *arrayTransform, std::vector<s32>& vCountArray, std::vector<s32>& vArray, bool flipZ )
{
	int numJoint = (int)arrayName.size();

	// get array float
	f32* f = (f32*)arrayTransform;

	for ( int i = 0; i < numJoint; i++ )
	{
		SJointParam newJoint;

		newJoint.Name = arrayName[i];

		core::matrix4 mat;
		mat.setM( f + i*16 );
		
		if (flipZ)
		{
			core::matrix4 mat2(mat, core::matrix4::EM4CONST_TRANSPOSED);

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

			newJoint.InvMatrix = mat2;
		}
		else
		{
			newJoint.InvMatrix = mat.getTransposed();
		}
		
		// add joint to controller
		mesh->Joints.push_back( newJoint );
	}
	
	// set vertex weight
	int nVertex = (int)vCountArray.size();
	int id = 0;
	
	for ( int i = 0; i < nVertex; i++ )
	{
		// num of bone in vertex
		int nBone = vCountArray[i];

		// loop on bone in vertex		
		for ( int iBone = 0; iBone < nBone; iBone++, id+=2 )
		{
			u32 boneId		= vArray[id];
			u32 weightId	= vArray[id + 1];
			f32 f			= arrayWeight[weightId];
			
			SWeightParam weightParam;

			weightParam.VertexID = i;
			weightParam.Strength = f;

			// add weight on bone
			mesh->Joints[boneId].Weights.push_back( weightParam );
			
			mesh->JointIndex.push_back( boneId );
			mesh->JointIndex.push_back( mesh->Joints[boneId].Weights.size() - 1 );
		}

	}
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
		const std::string& nodeName = (*i).first;
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
		const std::string& nodeName = (*i).first;
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
		const std::string& nodeName = (*i).first;
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
		const std::string& nodeName = (*i).first;
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
		const std::string& nodeName = (*i).first;
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
		const std::string& nodeName = (*i).first;
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
		for ( int i = 0, nLod = m_colladaLodNode.size(); i < nLod; i++ )
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

	for ( int nLod = m_colladaLodNode.size(), i = 0; i < nLod; i++ )
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