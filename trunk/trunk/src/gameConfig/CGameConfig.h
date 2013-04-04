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
	void initLevelConfig();
};

#endif