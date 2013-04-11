#ifndef _GAMESTRING_H_
#define _GAMESTRING_H_

#include "stdafx.h"

class CGameString
{
public:
	enum ELanguage
	{
		EN = 0,
		FR,
		DE,
		SP,
		IT,
		JP,
		KR,
		RU,
		SC,
		BR,
		TR,
		NumLang
	};
	
	struct STextEntry
	{
		std::string		id;		
		std::string		text[CGameString::NumLang];
	};

protected:

	static std::vector<STextEntry>			s_texts;
	static std::map<std::string, int>		s_textToID;
	static ELanguage						s_currentLang;

public:
	static void			loadString(const char *path);
	static const char*	getString(const char *textID);
	static void			release();

	static void			setLanguage(ELanguage lang);
};

#endif