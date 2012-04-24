#include "stdafx.h"
#include "CColladaMeshComponent.h"
#include "IView.h"

wstring			readId(io::IXMLReader *xmlRead);
void			findNextNoneWhiteSpace(const c8** start);
inline f32		readFloat(const c8** p);

void			readIntsInsideElement(io::IXMLReader* reader, s32* ints, u32 count);
void			readIntsInsideElement(io::IXMLReader* reader, vector<s32>& arrayInt);
void			readFloatsInsideElement(io::IXMLReader* reader, f32* floats, u32 count);
void			readStringInsideElement(io::IXMLReader* reader, vector<std::wstring>& arrayString);

SColorf			readColorNode(io::IXMLReader* reader);
f32				readFloatNode(io::IXMLReader* reader);
core::matrix4	readTranslateNode(io::IXMLReader* reader, bool flip);
core::matrix4	readRotateNode(io::IXMLReader* reader, bool flip);

ITexture*		getTextureFromImage( std::wstring& uri, ArrayEffectParams& listEffectParam);
SBufferParam*	getBufferWithUri( std::wstring& uri, SMeshParam* mesh );
int				getVerticesWithUri( std::wstring& uri, SMeshParam* mesh );
int				getEffectWithUri( std::wstring& uri, ArrayEffects& listEffectParam );
SMeshParam*		getMeshWithUri( std::wstring& uri, ArrayMeshParams& listMeshParam );


CColladaMeshComponent::CColladaMeshComponent( CGameObject *pObj )
	:IObjectComponent( pObj, (int)IObjectComponent::ColladaMesh )
{
	m_colladaNode = NULL;

	m_animeshFile = "";
	m_animFile = "";
	m_animSpeed	= 24.0f;
}

CColladaMeshComponent::~CColladaMeshComponent()
{

}

// loadFromFile
// load anim object from dae file
void CColladaMeshComponent::loadFromFile( char *lpFilename )
{
	if ( m_animeshFile == lpFilename )
		return;

	m_animeshFile = lpFilename;

	IrrlichtDevice	*device = getIView()->getDevice();
	IVideoDriver	*driver = getIView()->getDriver();
	io::IFileSystem *fs = device->getFileSystem();

	io::IXMLReader *xmlRead = fs->createXMLReader( lpFilename );

	if ( xmlRead == NULL )
	{
#if defined(GSEDITOR) || defined(GSANIMATION)
		WCHAR appPath[MAX_PATH];
		char  appPathA[MAX_PATH];

		uiApplication::getAppPath(appPath, MAX_PATH);
		uiString::copy<char, WCHAR>( appPathA, appPath  );
								
		std::string path = appPathA;
		path += "\\";
		path += std::string(lpFilename);
								
		xmlRead = fs->createXMLReader( path.c_str() );

		if ( xmlRead == NULL )
			return;
#else
		return;	
#endif
	}

	const std::wstring effectSectionName(L"effect");
	const std::wstring geometrySectionName(L"geometry");
	const std::wstring skinSectionName(L"skin");
	const std::wstring sceneSectionName(L"visual_scene");

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				std::wstring nodeName = xmlRead->getNodeName();

				if ( nodeName == effectSectionName )
				{
					parseEffectNode( xmlRead );
				}
				else if ( nodeName == geometrySectionName )
				{
					parseGeometryNode( xmlRead );
				}
				else if ( nodeName == skinSectionName )
				{
					parseSkinNode( xmlRead );
				}
				else if ( nodeName == sceneSectionName )
				{
					parseSceneNode( xmlRead );
				}
				
			}
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

							SBufferParam *buffer = getBufferWithUri( source, &mesh );
							if ( buffer )
							{
								if ( semantic == L"POSITION" )
								{
									buffer->Type = k_positionBuffer;
									verticesParam.Position = buffer;
								}
								else if ( semantic == L"NORMAL" )
								{
									buffer->Type = k_normalBuffer;
									verticesParam.Normal = buffer;
								}
								else if ( semantic == L"TEXCOORD" )
								{
									buffer->Type = k_texCoordBuffer;
									
									if ( verticesParam.TexCoord1 == NULL )
										verticesParam.TexCoord1 = buffer;
									else if ( verticesParam.TexCoord1 != buffer )
										verticesParam.TexCoord2 = buffer;
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
				
				triangle.NumTri = xmlRead->getAttributeValueAsInt(L"count");

				std::wstring materialName = xmlRead->getAttributeValue(L"material");
				triangle.EffectIndex = getEffectWithUri( materialName, m_listEffects );

				while(xmlRead->read())
				{							
					if (xmlRead->getNodeType() == io::EXN_ELEMENT )
					{
						if ( xmlRead->getNodeName() == inputNode )
						{
							std::wstring source = xmlRead->getAttributeValue(L"source");
							source.erase( source.begin() );

							triangle.VerticesIndex = getVerticesWithUri( source, &mesh );
						}
						else if ( xmlRead->getNodeName() == std::wstring(L"p") && triangle.VerticesIndex != -1 )
						{
							triangle.IndexBuffer = new s32[triangle.NumTri];
							readIntsInsideElement( xmlRead, triangle.IndexBuffer, triangle.NumTri );
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

// parseSkinNode
// parse skin data
void CColladaMeshComponent::parseSkinNode( io::IXMLReader *xmlRead )
{
	std::wstring source = xmlRead->getAttributeValue(L"source");
	source.erase( source.begin() );

	SMeshParam *mesh = getMeshWithUri( source, m_listMesh );
	if ( mesh == NULL )
		return;
	
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

	vector<std::wstring>	nameArray;

	int						numArray = 0;
	float					*jointArray = NULL;
	float					*transformArray = NULL;
	float					*weightArray = NULL;
	
	vector<s32>				vCountArray;
	vector<s32>				vArray;

	while(xmlRead->read())
	{							
		if (xmlRead->getNodeType() == io::EXN_ELEMENT )
		{
			std::wstring node = xmlRead->getNodeName();

			// <source>
			if ( node == sourceNode )
			{
				float *f = NULL;

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
							std::wstring name = xmlRead->getAttributeValue(L"name");
							if ( name == L"TRANSFORM" )
								transformArray = f;
							else if ( name == L"WEIGHT" )
								weightArray = f;
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
	updateJointToMesh( mesh, nameArray, weightArray, transformArray, vCountArray, vArray, true );

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

	SNodeParam *pNode = new SNodeParam();
	
	pNode->Name = xmlRead->getAttributeValue(L"id");

	if ( xmlRead->getAttributeValue(L"sid") )
		pNode->SID	= xmlRead->getAttributeValue(L"sid");	
	
	pNode->Type = xmlRead->getAttributeValue(L"type");
	
	pNode->Parent = parent;
	if ( parent )
		parent->Childs.push_back( pNode );

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
				pNode->Transform *= readTranslateNode(xmlRead, true);
			}
			else if ( xmlRead->getNodeName() == rotateSectionName )
			{
				// mul rotate
				pNode->Transform *= readRotateNode(xmlRead, true);
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
		effect->Mat.Lighting=true;
		effect->Mat.NormalizeNormals=true;
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
											effect->Transparency = colorf.getAlpha();
									}
									else if ( textureNodeName == xmlRead->getNodeName() )
									{
										wstring tname = xmlRead->getAttributeValue(L"texture");
										
										if ( node == diffuseNode )
											effect->Mat.setTexture(0, getTextureFromImage(tname, m_listEffectsParam ));									
										else if ( node == ambientNode )
										{
											// ambient texture: todo later
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
								wstring tname = xmlRead->getAttributeValue(L"texture");
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

	if ( effect->Transparency != 0.0f && effect->Transparency != 1.0f )
	{
		effect->Mat.MaterialType = irr::video::EMT_TRANSPARENT_VERTEX_ALPHA;
		effect->Mat.ZWriteEnable = false;
	}

	effect->Mat.setFlag(video::EMF_BILINEAR_FILTER, true);
	effect->Mat.setFlag(video::EMF_TRILINEAR_FILTER, true);
	effect->Mat.setFlag(video::EMF_ANISOTROPIC_FILTER, true);
}



// loadAnimFile
// load animation bone from dae file
void CColladaMeshComponent::loadAnimFile( char *lpFileName )
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

	m_clipAnimation.clear();
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
					m_clipAnimation[ clip.m_id ] = clip;
					
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
	
}


// parseAnimationNode
// parse anim node
void CColladaMeshComponent::parseAnimationNode( io::IXMLReader *xmlRead )
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

	AnimationFrames& animation = m_jointAnimation[stringBuffer];
		
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

// updateJointToMesh
// update joint
void CColladaMeshComponent::updateJointToMesh( SMeshParam *mesh, vector<wstring>& arrayName, float *arrayWeight, float *arrayTransform, vector<s32>& vCountArray, vector<s32>& vArray, bool flipZ )
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
			newJoint.InvMatrix = mat.getTransposed();

		
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
		}

	}
}

// setAnimation
// apply Animation to skin joint
void CColladaMeshComponent::setAnimation(const char *lpAnimName)
{
	if ( m_colladaNode == NULL )
		return;
	
	//if ( m_animNode->getMesh()->getMeshType() != EAMT_SKINNED )
	//	return;

	ISkinnedMesh* mesh = NULL;//(ISkinnedMesh*)m_animNode->getMesh();
	//if ( mesh == NULL )
	//	return;
	
	core::array<ISkinnedMesh::SJoint*>&	allJoint = mesh->getAllJoints();
	int nJoints = allJoint.size();
	
	// get anim time data
	ClipAnimation::iterator animIt = m_clipAnimation.find( std::string(lpAnimName) );
	if ( animIt == m_clipAnimation.end() )
		return;

	const float defaultFps = 30.0f;
	const float defaultTpf = 1.0f/defaultFps;

	SAnimClip& animClip = animIt->second;
	
	int fromFrame = 0, toFrame = 0;
	core::quaternion q1, q2;
	core::vector3df v1, v2;

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
			AnimationFrames& arrayFrame = it->second;			
						
			ISkinnedMesh::SPositionKey	pos;
			ISkinnedMesh::SRotationKey	rot;

			float currentTime = 0;
			float time	= animClip.m_time;
			float end	= animClip.m_time + animClip.m_duration;
			
			getFrameAtTime( &arrayFrame, time, &fromFrame,	&q1, &v1 );
			getFrameAtTime( &arrayFrame, end,  &toFrame,	&q2, &v2 );
			
			// save frame data			
			animClip.m_frames = (int) ((end - time) * defaultFps);
			
			for ( int i = fromFrame; i <= toFrame; i++ )
			{
				SAnimFrame& animFrame = arrayFrame[i];
				
				pos.position.X = 0;
				pos.position.Y = 0;
				pos.position.Z = 0;

				if ( i == fromFrame )
				{
					currentTime = 0;
					rot.rotation = q1;					
				}
				else if ( i == toFrame )
				{
					rot.rotation = q2;
					currentTime = end - time;				
				}
				else
				{
					currentTime =  animFrame.m_time - time;
					rot.rotation.fromAngleAxis(
							animFrame.m_rotAngle*core::DEGTORAD, 
							core::vector3df(animFrame.m_rotX, animFrame.m_rotZ, animFrame.m_rotY)
						);					
				}
				
				if ( animClip.m_movePosition )
				{
					pos.position.X = animFrame.m_translateY;
					pos.position.Y = animFrame.m_translateZ;
					pos.position.Z = animFrame.m_translateX;
				}


				mat.transformVect( pos.position );

				rot.frame = currentTime * defaultFps;
				pos.frame = currentTime * defaultFps;

				j->RotationKeys.push_back( rot );
				j->PositionKeys.push_back( pos );
			}					

		}		
	}

	// update skin mesh
	mesh->useAnimationFrom( mesh );	

	// update current anim
	m_currentAnim = &animClip;	
}

// getFrameAtTime
// get a frame at time
 bool CColladaMeshComponent::getFrameAtTime( AnimationFrames* frames, float time, int *frameID, core::quaternion *rotateData, core::vector3df *translateData )
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
			SAnimFrame &frame1 = frames->at( mid );
			SAnimFrame &frame2 = frames->at( mid + 1 );

			core::quaternion q1, q2;
			q1.fromAngleAxis( core::DEGTORAD * frame1.m_rotAngle, core::vector3df( frame1.m_rotX, frame1.m_rotZ, frame1.m_rotY ) );
			q2.fromAngleAxis( core::DEGTORAD * frame2.m_rotAngle, core::vector3df( frame2.m_rotX, frame2.m_rotZ, frame2.m_rotY ) );

			float f = (time - frame1.m_time)/(frame2.m_time - frame1.m_time);

			// calc rotate
			rotateData->slerp( q1, q2, f );

			core::vector3df v1(frame1.m_translateY, frame1.m_rotZ, frame1.m_rotX);
			core::vector3df v2(frame2.m_translateY, frame2.m_rotZ, frame2.m_rotX);

			*translateData = v1 + (v2 - v1) * f;

			// set frame id
			*frameID = mid;		
			return true;
		}
	}
	
	*frameID = nFrames-1;
	return false;

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
}

 
// saveData
// save data to serializable
void CColladaMeshComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	pObj->addPath("meshFile", m_animeshFile.c_str(), true);
	pObj->addPath("animFile", m_animFile.c_str(), true);
	pObj->addFloat("animSpeed", m_animSpeed, true );	
}

// loadData
// load data to serializable
void CColladaMeshComponent::loadData( CSerializable* pObj )
{		
	pObj->nextRecord();

	// read mesh file
	char *string = pObj->readString();
	loadFromFile( string );

	// read anim file
	string = pObj->readString();
	loadAnimFile( string );


	// read anim speed
	m_animSpeed = pObj->readFloat();	
}




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
		else
		if (reader->getNodeType() == io::EXN_ELEMENT_END)
			break; // end parsing text
	}
}

//! reads ints from inside of xml element until end of xml element
void readIntsInsideElement(io::IXMLReader* reader, vector<s32>& arrayInt)
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

			int value = 0;
					

			while ( *p )
			{
				while(*p && !(*p==L' ' || *p==L'\n' || *p==L'\r' || *p==L'\t'))
					++p;

				*p = NULL;

				if (*begin)
				{
					uiString::parseToInt<wchar_t>( begin, &value );
					arrayInt.push_back( value );
				}				

				p++;
				begin = p;
			}

		}
		else
		if (reader->getNodeType() == io::EXN_ELEMENT_END)
			break; // end parsing text
	}
}				

void readStringInsideElement(io::IXMLReader* reader, vector<std::wstring>& arrayString)
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

video::ITexture* getTextureFromImage( std::wstring& uri, ArrayEffectParams& listEffectParam)
{	
	int n = listEffectParam.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listEffectParam[i].Name == uri )
		{
			if ( listEffectParam[i].InitFromTexture.size() > 0 )
			{
				std::wstring textureName = listEffectParam[i].InitFromTexture;
				
				char textureNameA[1024];
				uiString::copy<char, const wchar_t>( textureNameA, textureName.c_str() );
					
				return getIView()->getDriver()->getTexture( textureNameA );
			}
			else if ( listEffectParam[i].Source.size() > 0 )
				return getTextureFromImage( listEffectParam[i].Source, listEffectParam );

			return NULL;
		}
	}
	return NULL;
}

SBufferParam* getBufferWithUri( std::wstring& uri, SMeshParam* mesh )
{
	int n = mesh->Buffers.size();
	for ( int i =0; i < n; i++ )
	{
		if ( mesh->Buffers[i].Name == uri )
		{
			return &mesh->Buffers[i];
		}
	}
	return NULL;
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

int getEffectWithUri( std::wstring& uri, ArrayEffects& listEffectParam )
{
	std::wstring fxName = uri + L"-fx";

	int n = listEffectParam.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listEffectParam[i].Id == fxName )
		{
			return i;
		}
	}
	return -1;
}

SMeshParam*	getMeshWithUri( std::wstring& uri, ArrayMeshParams& listMeshParam )
{
	int n = listMeshParam.size();
	for ( int i = 0; i < n; i++ )
	{
		if ( listMeshParam[i].Name == uri )
		{
			return &listMeshParam[i];
		}
	}

	return NULL;
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
			q.fromAngleAxis(floats[3]*core::DEGTORAD, core::vector3df(floats[0], floats[1], floats[2]));
		return q.getMatrix();
	}
	else
		return core::IdentityMatrix;
}