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
						uiString::convertUnicodeToUTF8((unsigned short*)match.c_str(), matchA);

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