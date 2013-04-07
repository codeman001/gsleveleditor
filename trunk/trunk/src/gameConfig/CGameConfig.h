#ifndef _CGAME_CONFIG_
#define _CGAME_CONFIG_

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

protected:
	std::vector<SGameMatch>	m_gameMatchs;

public:
	CGameConfig();
	~CGameConfig();

	void initConfig();

protected:
	// initLevelConfig
	// load level info
	void initLevelConfig();

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
};

#endif