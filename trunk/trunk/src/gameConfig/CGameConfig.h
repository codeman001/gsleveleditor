#ifndef _CGAME_CONFIG_
#define _CGAME_CONFIG_

#include "stdafx.h"
#include "gameMatch/IGameMatch.h"
#include "uiClass/uiSingleton.h"

class CGameConfig: public uiClass::uiSingleton<CGameConfig>
{
public:
	struct SLevelInfo
	{
		std::wstring	name;
		std::wstring	des;
		std::wstring	resource;
		std::wstring	thumbnail;
	};
	
	struct SGameMatch
	{
		IGameMatch::EGameMatchType	matchType;
		std::vector<SLevelInfo>		levels;
	};

	struct SCharacterLod
	{
		std::string		node;
		float			distance;
	};

	struct SCharacterInfo
	{
		std::string		name;
		std::string		model;
		std::string		anim;

		std::vector<SCharacterLod>	lods;
	};

protected:
	std::vector<SGameMatch>			m_gameMatchs;
	std::vector<SCharacterInfo>		m_gameCharacter;
public:
	CGameConfig();
	~CGameConfig();

	void initConfig();

protected:
	// initLevelConfig
	// load level info
	void initLevelConfig();

	// initCharacterConfig
	// load character
	void initCharacterConfig();

	// parseCharacter
	void parseCharacter(io::IXMLReader* reader);

public:
	// getNumGameMatch
	// get num match detected
	inline int getNumGameMatch()
	{
		return (int)m_gameMatchs.size();
	}

	// getGameMatch
	// get current match
	SGameMatch* getGameMatch(int id)
	{
		return &m_gameMatchs[id];
	}

	// getNumCharacter
	// get num character
	inline int getNumCharacter()
	{
		return (int)m_gameCharacter.size();
	}

	// getCharacterInfo
	// get character
	SCharacterInfo* getCharacterInfo(int id)
	{
		return &m_gameCharacter[id];
	}
};

#endif