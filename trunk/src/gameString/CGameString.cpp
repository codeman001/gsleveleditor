#include "stdafx.h"
#include "IView.h"
#include "CGameString.h"

std::vector<CGameString::STextEntry>	CGameString::s_texts;
std::map<std::string, int>				CGameString::s_textToID;
CGameString::ELanguage					CGameString::s_currentLang = CGameString::EN;

void CGameString::loadString(const char *path)
{
	IrrlichtDevice	*device = getIView()->getDevice();
	io::IFileSystem *fs = device->getFileSystem();
	
	io::IXMLReaderUTF8 *xmlRead = fs->createXMLReaderUTF8( path );

	if ( xmlRead == NULL )
	{
		xmlRead = fs->createXMLReaderUTF8( getIView()->getPath(path) );
		if ( xmlRead == NULL )
			return;
	}
	
	const char* langStr[] = 
	{
		"EN",
		"FR",
		"DE",
		"SP",
		"IT",
		"JP",
		"KR",
		"RU",
		"SC",
		"BR",
		"TR",
		"NumLang"
	};
	
	bool readText = false;
	ELanguage currentLang = NumLang;	
	
	int currentID[NumLang] = {0};

	while ( xmlRead->read() )
	{
		switch (xmlRead->getNodeType())
		{		
		case io::EXN_ELEMENT:
			{
				const char *nodeName = xmlRead->getNodeName();
				if ( nodeName == std::string("id") )
				{
					const char *stringID = xmlRead->getAttributeValue("value");
					
					// add entry
					s_texts.push_back( STextEntry() );
					STextEntry& t = s_texts.back();
					t.id = stringID;
				
					// map string to id
					s_textToID[stringID] = (int)s_texts.size() - 1;

				}
				else if (nodeName == std::string("string"))
				{
					readText = true;
				}
				else
				{
					for (int i = 0; i < (int)NumLang; i++)
					{
						if ( std::string(nodeName) == langStr[i] )
						{
							currentLang = (ELanguage)i;
							break;
						}
					}
				}
				break;
			}
		case io::EXN_ELEMENT_END:
			{
				const char *nodeName = xmlRead->getNodeName();
				if (nodeName == std::string("string"))
				{
					readText = false;
				}
				else
				{
					const char *nodeName = xmlRead->getNodeName();
					for (int i = 0; i < (int)NumLang; i++)
					{
						if ( std::string(nodeName) == langStr[i] )
						{
							currentLang = NumLang;
							break;
						}
					}
				}
			}
			break;
		case io::EXN_TEXT:
			{
				if ( currentLang != NumLang && readText )
				{
					const char *value = xmlRead->getNodeData();
					int len = strlen(value);

					char *textA = new char[len+1];
					strcpy(textA, value);
					uiString::trim<char>(textA);
							
					STextEntry& t = s_texts[ currentID[(int)currentLang] ];
					
					// apply text language
					t.text[(int)currentLang] = std::string(textA);

					// inc id
					currentID[(int)currentLang]++;

					delete textA;
				}
			}
			break;
		}
	}

	xmlRead->drop();
}

void CGameString::setLanguage(ELanguage lang)
{
	s_currentLang = lang;
}

const char*	CGameString::getString(const char *textID)
{
	std::string s(textID);

	if ( s_textToID.find(s) != s_textToID.end() )
	{
		int id = s_textToID[s];
		return s_texts[id].text[s_currentLang].c_str();
	}
	return textID;
}

void CGameString::release()
{
	s_texts.clear();
	s_textToID.clear();
}