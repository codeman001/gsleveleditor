#include "stdafx.h"
#include "IView.h"
#include "CGameConfig.h"

CGameConfig::CGameConfig()
{
}

CGameConfig::~CGameConfig()
{
}

void CGameConfig::initConfig()
{
	initLevelConfig();
	initCharacterConfig();
	initWeaponConfig();
}

void CGameConfig::initLevelConfig()
{
	const char *lpFilename = "data/config/levelConfig.xml";

	IrrlichtDevice	*device = getIView()->getDevice();
	io::IFileSystem *fs = device->getFileSystem();
	
	io::IXMLReader *xmlRead = fs->createXMLReader( lpFilename );

	if ( xmlRead == NULL )
	{
		xmlRead = fs->createXMLReader( getIView()->getPath(lpFilename) );
		if ( xmlRead == NULL )
			return;
	}

	const std::wstring	gameMatch(L"match");
	const std::wstring	level(L"level");
	
	// clear old value
	m_gameMatchs.clear();

	std::wstring				match;
	IGameMatch::EGameMatchType	gameMatchType = IGameMatch::MatchCount;

	for (int i = 0; i < (int)IGameMatch::MatchCount; i++)
	{
		m_gameMatchs.push_back( SGameMatch() );

		SGameMatch& m = m_gameMatchs.back();
		m.matchType = (IGameMatch::EGameMatchType)i;
	}

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				if ( xmlRead->getNodeName() == gameMatch )
				{
					if ( xmlRead->getAttributeValue(L"type") != NULL )
					{
						match			= xmlRead->getAttributeValue(L"type");
						gameMatchType	= IGameMatch::MatchCount;

						char matchA[64] = {0};
						uiString::convertUnicodeToUTF8(match.c_str(), matchA);

						for (int i = 0; i < (int)IGameMatch::MatchCount; i++)
						{
							if ( strcmp(matchA, IGameMatch::s_stringOfMatch[i]) == 0 )
								gameMatchType = (IGameMatch::EGameMatchType)i;
						}
					}
				}
				else if ( xmlRead->getNodeName() == level )
				{
					for (int i = 0; i < (int)m_gameMatchs.size(); i++)
					{
						if ( m_gameMatchs[i].matchType == gameMatchType )
						{
							m_gameMatchs[i].levels.push_back(SLevelInfo());
							SLevelInfo& lv = m_gameMatchs[i].levels.back();
							
							if ( xmlRead->getAttributeValue(L"name") )
								lv.name		= xmlRead->getAttributeValue(L"name");

							if ( xmlRead->getAttributeValue(L"des") )
								lv.des		= xmlRead->getAttributeValue(L"des");
							
							if ( xmlRead->getAttributeValue(L"resource") )
								lv.resource = xmlRead->getAttributeValue(L"resource");

							if ( xmlRead->getAttributeValue(L"thumbnail") )
								lv.thumbnail = xmlRead->getAttributeValue(L"thumbnail");

						}
					}
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

// parseCharacter
void CGameConfig::parseCharacter(io::IXMLReader* xmlRead)
{
	m_gameCharacter.push_back( SCharacterInfo() );
	SCharacterInfo& charInfo = m_gameCharacter.back();

	int readState = 0;

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				const wchar_t* nodeName = xmlRead->getNodeName();
				
				if ( nodeName == std::wstring(L"name") )
					readState = 1;
				else if ( nodeName == std::wstring(L"model") )
					readState = 2;
				else if ( nodeName == std::wstring(L"anim") )
					readState = 3;
				else if ( nodeName == std::wstring(L"node") )
				{
					const wchar_t* node = xmlRead->getAttributeValue(L"name");
					const wchar_t* distance = xmlRead->getAttributeValue(L"distance");
					
					char nodeA[512], distanceA[512];
					float dis = 0;
					uiString::convertUnicodeToUTF8(node, nodeA);
					uiString::convertUnicodeToUTF8(distance, distanceA);
					sscanf(distanceA, "%f", &dis);

					charInfo.lods.push_back( SCharacterLod() );
					SCharacterLod& lod = charInfo.lods.back();
					lod.node = nodeA;
					lod.distance = dis;
				}
			}
			break;
		case io::EXN_ELEMENT_END:
			{
				const wchar_t* nodeName = xmlRead->getNodeName();
				if ( nodeName == std::wstring(L"character") )
					return;
			}
			break;
		case io::EXN_TEXT:
			{
				const wchar_t* text = xmlRead->getNodeData();
				char textA[512];
				uiString::convertUnicodeToUTF8(text, textA);

				if ( readState == 1 )				
					charInfo.name	= std::string(textA);
				else if ( readState == 2 )
					charInfo.model	= std::string(textA);
				else if ( readState == 3 )
					charInfo.anim	= std::string(textA);

				readState = 0;
			}
			break;
		}
	}
}

// initCharacterConfig
// load character
void CGameConfig::initCharacterConfig()
{
	const char *lpFilename = "data/config/characterConfig.xml";

	IrrlichtDevice	*device = getIView()->getDevice();
	io::IFileSystem *fs = device->getFileSystem();
	
	io::IXMLReader *xmlRead = fs->createXMLReader( lpFilename );

	if ( xmlRead == NULL )
	{
		xmlRead = fs->createXMLReader( getIView()->getPath(lpFilename) );
		if ( xmlRead == NULL )
			return;
	}

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				const wchar_t* nodeName = xmlRead->getNodeName();
				if ( nodeName == std::wstring(L"character") )
				{
					parseCharacter(xmlRead);
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

// initWeaponConfig
// load weapon info
void CGameConfig::initWeaponConfig()
{
	const char *lpFilename = "data/config/weaponConfig.xml";

	IrrlichtDevice	*device = getIView()->getDevice();
	io::IFileSystem *fs = device->getFileSystem();
	
	io::IXMLReader *xmlRead = fs->createXMLReader( lpFilename );

	if ( xmlRead == NULL )
	{
		xmlRead = fs->createXMLReader( getIView()->getPath(lpFilename) );
		if ( xmlRead == NULL )
			return;
	}

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				const wchar_t* nodeName = xmlRead->getNodeName();
				if ( std::wstring(nodeName) == L"weapon" )
					parseWeapon(xmlRead);
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

// parseWeapon
void CGameConfig::parseWeapon(io::IXMLReader* xmlRead)
{
	m_gameWeapon.push_back(SWeaponInfo());	
	SWeaponInfo& weapon = m_gameWeapon.back();

	int readState = 0;
	const wchar_t* type = xmlRead->getAttributeValue(L"type");

	char typeA[512];
	uiString::convertUnicodeToUTF8(type, typeA);
	weapon.type = typeA;

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				const wchar_t* nodeName = xmlRead->getNodeName();
				
				if ( nodeName == std::wstring(L"name") )
					readState = 1;
				else if ( nodeName == std::wstring(L"model") )
					readState = 2;
				else if ( nodeName == std::wstring(L"snap") )
					readState = 3;
				else if ( nodeName == std::wstring(L"param") )
				{
					const wchar_t* name		= xmlRead->getAttributeValue(L"name");
					const wchar_t* value	= xmlRead->getAttributeValue(L"value");
					
					char nameA[512];
					char valueA[512];

					uiString::convertUnicodeToUTF8(name, nameA);
					uiString::convertUnicodeToUTF8(value, valueA);

					weapon.params.push_back( SParam(nameA, valueA) );
				}
			}
			break;
		case io::EXN_ELEMENT_END:
			{
				const wchar_t* nodeName = xmlRead->getNodeName();
				if ( nodeName == std::wstring(L"weapon") )
					return;
			}
			break;
		case io::EXN_TEXT:
			{
				const wchar_t* text = xmlRead->getNodeData();
				char textA[512];
				uiString::convertUnicodeToUTF8(text, textA);
				
				if ( readState == 1 )
					weapon.name = textA;
				else if ( readState == 2 )
					weapon.model = textA;
				else if ( readState == 3 )
					weapon.snap = textA;

				readState = 0;
			}
			break;
		}
	}
}

// getWeaponByName
// get weapon by name
CGameConfig::SWeaponInfo* CGameConfig::getWeaponByName(const char* name)
{
	std::string wpName = name;

	for (int i = 0, n = (int)m_gameWeapon.size(); i < n; i++)
	{
		if ( m_gameWeapon[i].name == wpName )
			return &m_gameWeapon[i];
	}

	return NULL;
}